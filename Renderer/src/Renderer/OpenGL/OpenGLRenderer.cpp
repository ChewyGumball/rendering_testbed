#include "Renderer/OpenGL/OpenGLRenderer.h"

#include <iostream>

#include <random>

#include "Util/FileUtils.h"
#include "Util/StringUtils.h"

#include <Buffer/DataBufferView.h>
#include <Buffer/DataBufferArrayView.h>

#include "Resources/Mesh.h"
#include "Resources/Model.h"
#include "Resources/ModelInstance.h"
#include "Resources/FrameBuffer.h"
#include "Renderer/RenderOptions.h"
#include "Resources/Material.h"
#include "Resources/ShaderConstantBuffer.h"

#include "Renderer/OpenGL/OpenGLRenderMesh.h"
#include "Renderer/OpenGL/OpenGLRenderModel.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Renderer/OpenGL/OpenGLFrameBuffer.h"
#include "Renderer/OpenGL/OpenGLTextureBuffer.h"
#include "Renderer/OpenGL/OpenGLShaderConstantBuffer.h"

using namespace Renderer;
using namespace Renderer::OpenGL;

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

	void bindShaderConstants(std::shared_ptr<OpenGLShader> shader, std::shared_ptr<OpenGLRenderModel> model, const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>>& renderPassConstantBuffers)
	{
		std::vector<std::pair<std::string, RenderResourceID>> buffersToBind;
		for (auto& buffer : model->material()->constants()) {
			buffersToBind.push_back(std::make_pair(buffer.first, buffer.second->id()));
		}
		for (auto& buffer : model->material()->shader()->systemConstantBufferNames()) {
			buffersToBind.push_back(std::make_pair(buffer, renderPassConstantBuffers.at(buffer)->id()));
		}

		std::unordered_set<GLint> requiredSlots;
		std::vector<std::pair<std::string, RenderResourceID>> unboundBuffers;
		for(auto& buffer : buffersToBind) {
			int bindPoint = boundShaderConstantBuffers[buffer.second];
			if (bindPoint == -1) {
				unboundBuffers.push_back(buffer);
			}
			else {
				requiredSlots.insert(bindPoint);
				//Make sure the shader has the correct bind point just in case the buffer is still bound but has moved
				shader->bindUniformBufferToBindPoint(buffer.first, bindPoint);
			}
		}

		//If the buffer is not currently bound, pick a random bind point to bind it to (but don't unbind a buffer that is also required by this shader)
		//TODO: Look into better bind piont picking strategy
		for(auto& buffer : unboundBuffers) {
			int bindPoint = boundShaderConstantBuffers[buffer.second];
			do {
				bindPoint = distribution(mt);
			} while (requiredSlots.count(bindPoint) != 0);
			requiredSlots.insert(bindPoint);

			boundShaderConstantBuffers[shaderConstantBufferBindings[bindPoint]] = -1;
			boundShaderConstantBuffers[buffer.second] = bindPoint;
			shaderConstantBufferBindings[bindPoint] = buffer.second;
			shaderConstantBuffers[buffer.second]->bindTo(bindPoint);

			shader->bindUniformBufferToBindPoint(buffer.first, bindPoint);
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
		}
	}

	void createMaterialIfRequired(std::shared_ptr<const Material> material) {
		createShaderIfRequired(material->shader());
		for (auto& constant : material->constants()) {
			createConstantBufferIfRequired(constant.second);
		}
	}

	void createModelIfRequired(std::shared_ptr<const Model> model)
	{
		if (models.count(model->id()) == 0) {
			createMeshIfRequired(model->mesh());
			createMaterialIfRequired(model->material());

			OpenGLRenderer::checkGLError();
			for (auto texture : model->textures()) {
				createTextureIfRequired(texture.second);
			}

			std::unordered_map<std::string, RenderResourceID> modelTextures;
			for (auto texture : model->textures()) {
				modelTextures[texture.first] = texture.second->id();
			}

			models.emplace(model->id(), std::make_shared<OpenGLRenderModel>(meshes[model->mesh()->id()], shaders[model->material()->shader()->id()], modelTextures, model->material()));
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

		distribution = std::uniform_int_distribution<GLint>(0, maxShaderConstantBufferBindingPoints - 1);
		shaderConstantBufferBindings.resize(maxShaderConstantBufferBindingPoints);
	}
}

void OpenGLRenderer::checkGLError()
{
	GLenum err;
	while (err = glGetError() != GL_NO_ERROR) {
		std::string error;
		switch (err) {
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		default: error = "unknown";
		}
		std::cout << error << std::endl;
	}
}

OpenGLRenderer::OpenGLRenderer()
{
	initialize();
}

OpenGLRenderer::~OpenGLRenderer() {}

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

void OpenGLRenderer::updateConstantBuffers(std::unordered_set<std::shared_ptr<ShaderConstantBuffer>>& constantBuffers)
{
	for (std::shared_ptr<ShaderConstantBuffer> constantBuffer : constantBuffers)
	{
		createConstantBufferIfRequired(constantBuffer);
		shaderConstantBuffers[constantBuffer->id()]->uploadIfDirty();
	}
}

void OpenGLRenderer::draw(const std::vector<std::shared_ptr<const ModelInstance>>& modelInstances, const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>>& globalShaderConstantBuffers)
{
	//checkGLError();
	std::shared_ptr<const ModelInstance> firstInstance = modelInstances[0];
	createModelIfRequired(firstInstance->model());

	std::shared_ptr<OpenGLRenderModel> model = models[modelInstances[0]->model()->id()];
	uploadInstanceData(model->transformVBO(), modelInstances);

	std::shared_ptr<OpenGLShader> shader = model->shader();
	shader->bind();
	bindTextures(shader, model->textures());
	bindShaderConstants(shader, model, globalShaderConstantBuffers);

    model->draw(static_cast<int>(modelInstances.size()));
	//checkGLError();
}
