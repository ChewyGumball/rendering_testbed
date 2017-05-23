#include "Renderer/OpenGL/OpenGLRenderer.h"

#include <iostream>

#include <random>

#include "Util/FileUtils.h"
#include "Util/StringUtils.h"

#include <Buffer/DataBufferView.h>
#include <Buffer/DataBufferArrayView.h>
#include <Buffer/BufferFormat.h>

#include "Resources/Mesh.h"
#include "Resources/Model.h"
#include "Resources/Shader.h"
#include "Resources/ModelInstance.h"
#include "Resources/FrameBuffer.h"
#include "Renderer/RenderOptions.h"
#include "Resources/Material.h"
#include "Resources/ShaderConstantBuffer.h"

#include <Resources/RenderResourceManagement.h>

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

	std::unordered_map<RenderResourceID, OpenGLRenderMesh>    meshes;
	std::unordered_map<RenderResourceID, OpenGLShader>        shaders;
	std::unordered_map<RenderResourceID, OpenGLRenderModel>   models;
	std::unordered_map<RenderResourceID, OpenGLTextureBuffer> textures;
	std::unordered_map<RenderResourceID, OpenGLFrameBuffer>   frameBuffers;
	std::unordered_map<RenderResourceID, OpenGLShaderConstantBuffer>   shaderConstantBuffers;

	std::unordered_map<RenderResourceID, int> boundShaderConstantBuffers;
	std::vector<RenderResourceID> shaderConstantBufferBindings;
	
	void bindTextures(OpenGLShader& shader, const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>>& modelTextures)
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

	void bindShaderConstants(OpenGLShader& shader, const std::shared_ptr<const Material> material, const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>>& renderPassConstantBuffers)
	{
		std::vector<std::pair<std::string, RenderResourceID>> buffersToBind;
		for (auto& buffer : material->constants()) {
			buffersToBind.push_back(std::make_pair(buffer.first, buffer.second->id()));
		}
		for (auto& buffer : material->shader()->systemConstantBufferNames()) {
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
				shader.bindUniformBufferToBindPoint(buffer.first, bindPoint);
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
			shaderConstantBuffers.at(buffer.second).bindTo(bindPoint);

			shader.bindUniformBufferToBindPoint(buffer.first, bindPoint);
		}
	}
	
	std::vector<uint64_t> sortInstances(const std::vector<std::shared_ptr<const ModelInstance>>& instances) {
		std::vector<uint64_t> indices;
		return indices;
	}

	void uploadInstanceData(GLuint buffer, const std::vector<std::shared_ptr<const ModelInstance>>& instances, SortType sortType) {
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
	if (options.frameBuffer) {
		frameBuffers.at(options.frameBuffer->id()).bind();
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

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
		shaderConstantBuffers.at(constantBuffer->id()).uploadIfDirty();
	}
}

void OpenGLRenderer::draw(const std::vector<std::shared_ptr<const ModelInstance>>& modelInstances, SortType sortType, const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>>& globalShaderConstantBuffers)
{
	//checkGLError();
	std::shared_ptr<const ModelInstance> firstInstance = modelInstances[0];

	OpenGLRenderModel& model = models.at(firstInstance->model()->id());
	uploadInstanceData(model.transformVBO(), modelInstances, sortType);

	OpenGLShader& shader = shaders.at(model.material()->shader()->id());
	shader.bind();
	bindTextures(shader, model.textures());
	bindShaderConstants(shader, model.material(), globalShaderConstantBuffers);

    model.draw(static_cast<int>(modelInstances.size()));
	//checkGLError();
}

void Renderer::OpenGL::OpenGLRenderer::createPendingResources()
{
	for (RenderResourceManagement::MeshData& m : RenderResourceManagement::drainPendingMeshes()) {
		meshes.emplace(std::piecewise_construct, std::forward_as_tuple(m.id), std::forward_as_tuple(m.format, m.vertexData, m.indexData));
	}

	for (RenderResourceManagement::TextureData& t : RenderResourceManagement::drainPendingTextures()) {
		textures.emplace(std::piecewise_construct, std::forward_as_tuple(t.id), std::forward_as_tuple(t.dimensions, t.format, t.data));
	}

	for (RenderResourceManagement::ShaderConstantBufferData& s : RenderResourceManagement::drainPendingShaderConstantBuffers()) {
		shaderConstantBuffers.emplace(std::piecewise_construct, std::forward_as_tuple(s.id), std::forward_as_tuple(s.buffer));
		boundShaderConstantBuffers[s.id] = -1;
	}
	
	for (RenderResourceManagement::FrameBufferData& f : RenderResourceManagement::drainPendinFrameBuffers()) {
		frameBuffers.emplace(std::piecewise_construct, std::forward_as_tuple(f.id), std::forward_as_tuple(f.targets, textures));
	}

	for (RenderResourceManagement::UncompiledShaderData& s : RenderResourceManagement::drainPendingUncompiledShaders()) {
		shaders.emplace(std::piecewise_construct, std::forward_as_tuple(s.id), std::forward_as_tuple(s.id, s.sources));
	}

	for (RenderResourceManagement::ModelData& m : RenderResourceManagement::drainPendingModels()) {
		models.emplace(std::piecewise_construct, std::forward_as_tuple(m.id), std::forward_as_tuple(meshes.at(m.mesh->id()), shaders.at(m.material->shader()->id()), std::move(m.textures), m.material));
	}
}

void Renderer::OpenGL::OpenGLRenderer::destroyPendingResources()
{
}

