#include "Renderer/OpenGL/OpenGLRenderer.h"

#include <utility>

#include "Models/ModelInstance.h";
#include "Models/Model.h";
#include "Models/Mesh.h";
#include "Renderer/Camera.h";

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
		models.emplace(modelInstance->model()->id(), OpenGLRenderModel(modelInstance->model()->mesh()->id(), modelInstance->model()->shader()->id()));
	}

	modelInstances[modelInstance->model()->id()].emplace_back(modelInstance);
	buffersNeedingUpdates.insert(modelInstance->model()->id());
}

void OpenGLRenderer::draw(const Camera & c)
{
	//remake any buffers that need to be resized
	for (auto s : buffersNeedingUpdates)
	{
		GLuint buffer;
		if (transformsBuffers.count(s) != 0)
		{
			buffer = transformsBuffers[s];
		}

		glGenBuffers(1, &buffer);
		transformsBuffers[s] = buffer;

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, modelInstances[s].size() * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);

		meshes[s].setupTransformAttributes();
	}
	buffersNeedingUpdates.clear();

	//update transform data and draw models
	for (auto instanceList : modelInstances)
	{
		std::vector<glm::mat4> transforms;
		for (auto instance : instanceList.second)
		{
			transforms.push_back(instance->transformMatrix());
		}
		glBindBuffer(GL_ARRAY_BUFFER, transformsBuffers[instanceList.first]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, transforms.size() * sizeof(glm::mat4), transforms.data());

		OpenGLRenderModel& model = models[instanceList.first];
		OpenGLShader& shader = shaders[model.shaderID];
		shader.bind();
		shader.setUniformMatrix4f("view", c.transform());
		shader.setUniformMatrix4f("projection", c.projection());
		meshes[model.meshID].draw(instanceList.second.size());
	}
}
