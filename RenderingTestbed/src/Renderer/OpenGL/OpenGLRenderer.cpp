#include "Renderer/OpenGL/OpenGLRenderer.h"

#include <utility>
#include <algorithm>

#include "Util/StringUtils.h"

#include "Models/ModelInstance.h"
#include "Models/Model.h"
#include "Models/Mesh.h"
#include "Renderer/Camera.h"

namespace {
	std::unordered_map<std::size_t, OpenGLRenderMesh> meshes;
	std::unordered_map<std::size_t, OpenGLShader> shaders;
	std::unordered_map<std::size_t, OpenGLRenderModel> models;
}

void OpenGLRenderer::addLightUniforms(OpenGLShader & shader)
{
	for (int i = 0; i < lights.size(); i++)
	{
		PointLight& light = lights[i];
		shader.setUniform3f(Util::String::Format("pointLights[%d].position", i), light.position());
		shader.setUniform3f(Util::String::Format("pointLights[%d].intensity", i), light.intensity());
		shader.setUniform1f(Util::String::Format("pointLights[%d].power", i), light.power());
	}
}

OpenGLRenderer::OpenGLRenderer()
{
}


OpenGLRenderer::~OpenGLRenderer()
{
}

void OpenGLRenderer::addModelInstance(std::shared_ptr<const ModelInstance> modelInstance)
{
	if (meshes.count(modelInstance->model()->mesh()->id()) == 0)
	{
		meshes.emplace(modelInstance->model()->mesh()->id(), modelInstance->model()->mesh());
	}

	if (shaders.count(modelInstance->model()->shader()->id()) == 0)
	{
		shaders.emplace(modelInstance->model()->shader()->id(), modelInstance->model()->shader());
	}

	if (models.count(modelInstance->model()->id()) == 0)
	{
		models.emplace(modelInstance->model()->id(), OpenGLRenderModel(meshes[modelInstance->model()->mesh()->id()], modelInstance->model()->shader()->id(), modelInstance->model()->textures()));
	}

	modelInstances.emplace_back(modelInstance);
}

void OpenGLRenderer::addPointLight(PointLight light)
{
	lights.push_back(light);
}

void OpenGLRenderer::removeModelInstance(std::shared_ptr<const ModelInstance> modelInstance)
{
	modelInstances.erase(std::remove(modelInstances.begin(), modelInstances.end(), modelInstance), modelInstances.end());
}

void OpenGLRenderer::draw(const Camera & c, bool doFrustrumCulling)
{
	trianglesDrawn = 0;
	std::unordered_map<size_t, std::vector<std::shared_ptr<const ModelInstance>>> culledInstances;
	if (doFrustrumCulling)
	{
		culledInstances = cull(c, modelInstances);
	}
	else
	{
		for (size_t i = 0; i < modelInstances.size(); ++i)
		{
			culledInstances[modelInstances[i]->model()->id()].push_back(modelInstances[i]);
		}
	}

	for (auto instanceList : culledInstances)
	{
		OpenGLRenderModel& model = models[instanceList.first];
		//update transform data and draw models
		std::vector<glm::mat4> transforms;
		for (auto instance : instanceList.second)
		{
			transforms.push_back(instance->transformMatrix());
			//transforms.push_back(glm::mat4(0));
			trianglesDrawn += instance->triangleCount();
		}
		glNamedBufferData(model.transformVBO(), transforms.size() * sizeof(glm::mat4), transforms.data(), GL_STATIC_DRAW);

		OpenGLShader& shader = shaders[model.shaderID];
		shader.bind();
		model.bindTextures(shader);
		shader.setUniformMatrix4f("view", c.transform());
		shader.setUniformMatrix4f("projection", c.projection());
		shader.setUniform3f("cameraPosition", c.position());
		addLightUniforms(shader);
		model.draw(transforms.size());
	}
}
