#include "Renderer/OpenGL/OpenGLRenderer.h"

#include <random>

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
#include "Renderer/OpenGL/OpenGLShaderConstantBuffer.h"

namespace {
	std::random_device r;
	std::mt19937 mt(r());
	std::uniform_int_distribution<GLint> distribution;

	std::vector<GLenum> textureUnits{ GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5, GL_TEXTURE6, GL_TEXTURE7, GL_TEXTURE8 };

	std::unordered_map<RenderResourceID, std::shared_ptr<OpenGLRenderMesh>>    meshes;
	std::unordered_map<RenderResourceID, std::shared_ptr<OpenGLShader>>        shaders;
	std::unordered_map<RenderResourceID, std::shared_ptr<OpenGLRenderModel>>   models;
	std::unordered_map<RenderResourceID, std::shared_ptr<OpenGLTextureBuffer>> textures;
	std::unordered_map<RenderResourceID, std::shared_ptr<OpenGLFrameBuffer>>   frameBuffers;
	std::unordered_map<RenderResourceID, std::shared_ptr<OpenGLShaderConstantBuffer>>   shaderConstantBuffers;

	std::unordered_map<RenderResourceID, int> boundShaderConstantBuffers;
	std::vector<RenderResourceID> shaderConstantBufferBindings;


	void addLightUniforms(std::shared_ptr<OpenGLShader> shader, std::vector<PointLight>& lights)
	{
		for (int i = 0; i < lights.size(); i++) {
			PointLight& light = lights[i];
			shader->setUniform3f(Util::String::Format("pointLights[%d].position", i), light.position());
			shader->setUniform3f(Util::String::Format("pointLights[%d].intensity", i), light.intensity());
			shader->setUniform1f(Util::String::Format("pointLights[%d].power", i), light.power());
		}
	}

	void bindTextures(std::shared_ptr<OpenGLShader> shader, std::unordered_map<std::string, RenderResourceID> modelTextures)
	{
		int textureUnit = 0;
		for (auto& texture : modelTextures) {
			glActiveTexture(textureUnits[textureUnit]);
			glBindTexture(GL_TEXTURE_2D, textures[texture.second]->handle());
			shader->setUniform1i(texture.first, textureUnit);

			if (textureUnit++ >= textureUnits.size()) {
				break;
			}
		}
	}

	void bindShaderConstants(std::shared_ptr<OpenGLShader> shader, std::unordered_map<std::string, RenderResourceID> shaderConstantBuffersToBind)
	{
		for (auto buffer : shaderConstantBuffersToBind) {
			int bindPoint = boundShaderConstantBuffers[buffer.second];

			//If the buffer is not currently bound, pick a random bind point to bind it to
			//TODO: Look into better bind piont picking strategy
			if (bindPoint == -1) {
				bindPoint = distribution(mt);
				boundShaderConstantBuffers[shaderConstantBufferBindings[bindPoint]] = -1;
				boundShaderConstantBuffers[buffer.second] = bindPoint;
				shaderConstantBufferBindings[bindPoint] = buffer.second;
			}

			//Make sure the shader has the correct bind point just in case the buffer is still bound but has moved
			shader->bindUniformBufferToBindPoint(buffer.first, bindPoint);

			//Upload data if it is dirty
			shaderConstantBuffers[buffer.second]->uploadIfDirty();
		}
	}

	void reloadTexture(std::shared_ptr<TextureBuffer> texture) {
		textures[texture->id()] = std::make_shared<OpenGLTextureBuffer>(texture);
	}

	void createTextureIfRequired(std::shared_ptr<TextureBuffer> texture)
	{
		if (textures.count(texture->id()) == 0) {
			textures.emplace(texture->id(), std::make_shared<OpenGLTextureBuffer>(texture));

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
			frameBuffers.emplace(frameBuffer->id(), std::make_shared<OpenGLFrameBuffer>(frameBuffer, textures));
		}
	}

	void createMeshIfRequired(std::shared_ptr<const Mesh> mesh)
	{
		if (meshes.count(mesh->id()) == 0) {
			meshes.emplace(mesh->id(), std::make_shared<OpenGLRenderMesh>(mesh));
		}
	}

	void createConstantBufferIfRequired(std::shared_ptr<const ShaderConstantBuffer> shaderConstantBuffer) 
	{
		if (shaderConstantBuffers.count(shaderConstantBuffer->id()) == 0) {
			shaderConstantBuffers.emplace(shaderConstantBuffer->id(), std::make_shared<OpenGLShaderConstantBuffer>(shaderConstantBuffer));
			boundShaderConstantBuffers[shaderConstantBuffer->id()] = -1;
		}
	}

	void reloadShader(std::shared_ptr<const Shader> shader) {
		shaders[shader->id()] = std::make_shared<OpenGLShader>(shader);
	}

	void createShaderIfRequired(std::shared_ptr<const Shader> shader)
	{
		if (shaders.count(shader->id()) == 0) {
			shaders.emplace(shader->id(), std::make_shared<OpenGLShader>(shader));

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

		for (auto constant : model->shaderConstants()) {
			createConstantBufferIfRequired(constant.second);
		}

		if (models.count(model->id()) == 0) {
			std::unordered_map<std::string, RenderResourceID> modelTextures;
			for (auto texture : model->textures()) {
				modelTextures[texture.first] = texture.second->id();
			}

			std::unordered_map<std::string, RenderResourceID> shaderConstants;
			for (auto constants : model->shaderConstants()) {
				shaderConstants[constants.first] = constants.second->id();
			}

			models.emplace(model->id(), std::make_shared<OpenGLRenderModel>(meshes[model->mesh()->id()], shaders[model->shader()->id()], modelTextures, shaderConstants));
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


	bool initialized = false;
	void initialize() {
		if (initialized) return;

		initialized = true;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);

		GLint maxShaderConstantBufferBindingPoints;
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxShaderConstantBufferBindingPoints);	

		distribution = std::uniform_int_distribution<GLint>(0, maxShaderConstantBufferBindingPoints);
		shaderConstantBufferBindings.resize(maxShaderConstantBufferBindingPoints);
	}
}

OpenGLRenderer::OpenGLRenderer() 
{
	initialize();
}

OpenGLRenderer::~OpenGLRenderer() {}

void OpenGLRenderer::addPointLight(PointLight light) { lights.push_back(light); }

void OpenGLRenderer::processRenderingOptions(RenderOptions & options)
{
	createFrameBufferIfRequired(options.frameBuffer);

	frameBuffers[options.frameBuffer->id()]->bind();

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

	std::shared_ptr<OpenGLRenderModel> model = models[instances[0]->model()->id()];
	uploadInstanceData(model->transformVBO(), instances);

	std::shared_ptr<OpenGLShader> shader = shaders[model->shaderID()];
	shader->bind();

	bindTextures(shader, model->textures());
	bindShaderConstants(shader, model->shaderConstants());

    shader->setUniformMatrix4f("view", camera->transform());
    shader->setUniformMatrix4f("projection", camera->projection());
    shader->setUniform3f("cameraPosition", camera->position());

	
    addLightUniforms(shader, lights);
	
    model->draw(static_cast<int>(instances.size()));
}
