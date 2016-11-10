#pragma once
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "OpenGLRenderMesh.h"
#include "OpenGLRenderModel.h"
#include "OpenGLShader.h"

#include "Models/Mesh.h"
#include "Models/Model.h"

class Camera;
class ModelInstance;

class OpenGLRenderer
{
private:
	std::unordered_map<std::size_t, GLuint> transformsBuffers;
	std::unordered_set<std::size_t> buffersNeedingUpdates;

	std::unordered_map<std::size_t, OpenGLRenderMesh> meshes;
	std::unordered_map<std::size_t, OpenGLShader> shaders;
	std::unordered_map<std::size_t, OpenGLRenderModel> models;
	std::unordered_map<std::size_t, std::vector<std::shared_ptr<const ModelInstance>>> modelInstances;
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	void addModelInstance(std::shared_ptr<const ModelInstance> modelInstance);

	void draw(const Camera& c);
};

