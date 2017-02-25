#include "Renderer/OpenGL/OpenGLRenderer.h"

#include "Util/FileUtils.h"
#include "Util/StringUtils.h"

#include <Buffer/DataBufferView.h>
#include <Buffer/DataBufferArrayView.h>

#include "Renderer/Mesh.h"
#include "Renderer/Model.h"
#include "Renderer/ModelInstance.h"
#include "Renderer/Camera.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/RenderOptions.h"

#include "Renderer/OpenGL/OpenGLRenderMesh.h"
#include "Renderer/OpenGL/OpenGLRenderModel.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Renderer/OpenGL/OpenGLFrameBuffer.h"
#include "Renderer/OpenGL/OpenGLTextureBuffer.h"

namespace {

	std::vector<GLenum> textureUnits{ GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5, GL_TEXTURE6, GL_TEXTURE7, GL_TEXTURE8 };

	std::unordered_map<RenderResourceID, OpenGLRenderMesh>    meshes;
	std::unordered_map<RenderResourceID, OpenGLShader>        shaders;
	std::unordered_map<RenderResourceID, OpenGLRenderModel>   models;
	std::unordered_map<RenderResourceID, OpenGLTextureBuffer> textures;
	std::unordered_map<RenderResourceID, OpenGLFrameBuffer>   frameBuffers;

	void addLightUniforms(OpenGLShader& shader, std::vector<PointLight>& lights)
	{
		for (int i = 0; i < lights.size(); i++) {
			PointLight& light = lights[i];
			shader.setUniform3f(Util::String::Format("pointLights[%d].position", i), light.position());
			shader.setUniform3f(Util::String::Format("pointLights[%d].intensity", i), light.intensity());
			shader.setUniform1f(Util::String::Format("pointLights[%d].power", i), light.power());
		}
	}

	void bindTextures(OpenGLShader& shader, std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> modelTextures)
	{
		int textureUnit = 0;
		for (auto& texture : modelTextures) {
			glActiveTexture(textureUnits[textureUnit]);
			glBindTexture(GL_TEXTURE_2D, textures[texture.second->id()].handle());
			shader.setUniform1i(texture.first, textureUnit);

			if (textureUnit++ >= textureUnits.size()) {
				break;
			}
		}
	}

	void reloadTexture(std::shared_ptr<TextureBuffer> texture) {
		textures[texture->id()] = OpenGLTextureBuffer(texture);
	}

	void createTextureIfRequired(std::shared_ptr<TextureBuffer> texture)
	{
		if (textures.count(texture->id()) == 0) {
			textures.emplace(texture->id(), texture);

			if (texture->isFileTexture()) {
				Util::File::WatchForChanges(texture->filename(), [=]() { reloadTexture(texture); });
			}
		}
	}

	void createFrameBufferIfRequired(std::shared_ptr<const FrameBuffer> frameBuffer)
	{
		for (auto target : frameBuffer->targets()) {
			createTextureIfRequired(target.second);
		}
		if (frameBuffers.count(frameBuffer->id()) == 0) {
			frameBuffers.emplace(frameBuffer->id(), OpenGLFrameBuffer(frameBuffer, textures));
		}
	}

	void createMeshIfRequired(std::shared_ptr<const Mesh> mesh)
	{
		if (meshes.count(mesh->id()) == 0) {
			meshes.emplace(mesh->id(), mesh);
		}
	}

	void reloadShader(std::shared_ptr<const Shader> shader) {
		shaders[shader->id()] = OpenGLShader(shader);
	}

	void createShaderIfRequired(std::shared_ptr<const Shader> shader)
	{
		if (shaders.count(shader->id()) == 0) {
			shaders.emplace(shader->id(), shader);

			for (auto& type : shader->filenames()) {
				for (auto& file : type.second) {
					Util::File::WatchForChanges(file, [=]() { reloadShader(shader); });
				}
			}
		}
	}

	void createModelIfRequired(std::shared_ptr<const Model> model)
	{
		createMeshIfRequired(model->mesh());
		createShaderIfRequired(model->shader());
		for (auto texture : model->textures()) {
			createTextureIfRequired(texture.second);
		}

		if (models.count(model->id()) == 0) {
			models.emplace(model->id(), OpenGLRenderModel(meshes[model->mesh()->id()], shaders[model->shader()->id()], model->textures(), model->shaderConstants()));
		}
	}

	void uploadInstanceData(GLuint buffer, const std::vector<std::shared_ptr<const ModelInstance>>& instances) {
		std::vector<uint8_t> instanceData;
		instanceData.reserve(instances.size() * instances[0]->instanceData().format()->size());
		for (auto instance : instances) {
			auto& data = instance->instanceData();
			instanceData.insert(instanceData.end(), data.begin(), data.end());
		}
		glNamedBufferData(buffer, instanceData.size() * sizeof(uint8_t), instanceData.data(), GL_DYNAMIC_DRAW);
	}

	void uploadUniformArrayData(OpenGLShader& shader, std::string name, const DataBufferArrayView& uniforms);

	void uploadUniform(OpenGLShader& shader, std::string uniformName, std::string bufferName, const DataBufferView& buffer) {
		auto& uniform = buffer.format()->at(bufferName);
		switch (uniform.second) {
		case BufferElementType::INT_SCALAR: {
			shader.setUniform1i(uniformName, buffer.getInt(bufferName)); break;
		}
		case BufferElementType::FLOAT_SCALAR: {
			shader.setUniform1f(uniformName, buffer.getFloat(bufferName)); break;
		}
		case BufferElementType::FLOAT_VEC2: {
			shader.setUniform2f(uniformName, buffer.getVec2(bufferName)); break;
		}
		case BufferElementType::FLOAT_VEC3: {
			shader.setUniform3f(uniformName, buffer.getVec3(bufferName)); break;
		}
		case BufferElementType::FLOAT_VEC4: {
			shader.setUniform4f(uniformName, buffer.getVec4(bufferName)); break;
		}
		case BufferElementType::MAT4: {
			shader.setUniformMatrix4f(uniformName, buffer.getMat4(bufferName)); break;
		}
		case BufferElementType::ARRAY: {
			uploadUniformArrayData(shader, uniformName, buffer.getArray(bufferName)); break;
		}
		default: assert(false); //not implemented
		}
	}

	void uploadUniformArrayData(OpenGLShader& shader, std::string name, const DataBufferArrayView& uniforms) {
		auto format = uniforms.elementType();
		if (format == BufferElementType::BUFFER) {
			for (int i = 0; i < uniforms.elementCount(); ++i) {
				const DataBufferView& buffer = uniforms.getBufferAt(i);
				for (auto& uniform : buffer.format()->offsets()) {
					uploadUniform(shader, Util::String::Format("%s[%d].%s", name.data(), i, uniform.first.data()), uniform.first, buffer);
				}
			}
		}
		else {
			for (int i = 0; i < uniforms.elementCount(); ++i) {
				std::string name = Util::String::Format("%s[%d]", name.data(), i);
				switch (format) {
				case BufferElementType::INT_SCALAR: {
					shader.setUniform1i(name, uniforms.getIntAt(i)); break;
				}
				case BufferElementType::FLOAT_SCALAR: {
					shader.setUniform1f(name, uniforms.getFloatAt(i)); break;
				}
				case BufferElementType::FLOAT_VEC2: {
					shader.setUniform2f(name, uniforms.getVec2At(i)); break;
				}
				case BufferElementType::FLOAT_VEC3: {
					shader.setUniform3f(name, uniforms.getVec3At(i)); break;
				}
				case BufferElementType::FLOAT_VEC4: {
					shader.setUniform4f(name, uniforms.getVec4At(i)); break;
				}
				case BufferElementType::MAT4: {
					shader.setUniformMatrix4f(name, uniforms.getMat4At(i)); break;
				}
				case BufferElementType::ARRAY: {
					uploadUniformArrayData(shader, name, uniforms.getArrayAt(i));
				}
				default: assert(false); //not implemented
				}
			}
		}
	}

	void uploadUniformData(OpenGLShader& shader, const DataBufferView& uniforms) {
		for (auto& uniform : uniforms.format()->offsets()) {
			uploadUniform(shader, uniform.first, uniform.first, uniforms);
		}
	}
}

OpenGLRenderer::OpenGLRenderer() 
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
}

OpenGLRenderer::~OpenGLRenderer() {}

void OpenGLRenderer::addPointLight(PointLight light) { lights.push_back(light); }

void OpenGLRenderer::processRenderingOptions(RenderOptions & options)
{
	createFrameBufferIfRequired(options.frameBuffer);

	frameBuffers[options.frameBuffer->id()].bind();

	if (options.clearBuffers) {
		glClearColor(options.clearColour.r, options.clearColour.g, options.clearColour.b, options.clearColour.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	if (options.depthTest) {
		glEnable(GL_DEPTH_TEST);
	} 
	else {
		glDisable(GL_DEPTH_TEST);
	}

	glViewport(options.viewportOrigin.x, options.viewportOrigin.y, options.viewportDimensions.x, options.viewportDimensions.y);

	GLenum polygonMode = options.wireframe ? GL_LINE : GL_FILL;
	glPolygonMode(GL_FRONT, polygonMode);
	glPolygonMode(GL_BACK, polygonMode);
}

void OpenGLRenderer::draw(const std::vector<std::shared_ptr<const ModelInstance>>& instances, const std::shared_ptr<Camera> camera)
{
	std::shared_ptr<const ModelInstance> firstInstance = instances[0];
	createModelIfRequired(firstInstance->model());

    OpenGLRenderModel& model = models[instances[0]->model()->id()];
	uploadInstanceData(model.transformVBO(), instances);

	OpenGLShader& shader = shaders[model.shaderID()];
	shader.bind();
	bindTextures(shader, model.textures());
	uploadUniformData(shader, model.uniformBuffer());

    shader.setUniformMatrix4f("view", camera->transform());
    shader.setUniformMatrix4f("projection", camera->projection());
    shader.setUniform3f("cameraPosition", camera->position());

	
    addLightUniforms(shader, lights);
	
    model.draw(static_cast<int>(instances.size()));
}
