#pragma once
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "OpenGLRenderMesh.h"
#include "OpenGLRenderModel.h"
#include "OpenGLShader.h"

#include "Models/Mesh.h"
#include "Models/Model.h"

#include <Renderer\Renderer.h>
#include <Renderer/PointLight.h>

class Camera;
class ModelInstance;

class OpenGLRenderer : public Renderer
{
private:

	std::unordered_map<std::size_t, GLuint> transformsBuffers;
	std::unordered_set<std::size_t> buffersNeedingUpdates;

	std::vector<std::shared_ptr<const ModelInstance>> modelInstances;

	std::vector<PointLight> lights;

	glm::mat4 previouslyCulledCameraMatrix;

	void addLightUniforms(OpenGLShader& shader);
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	void addModelInstance(std::shared_ptr<const ModelInstance> modelInstance);
	void addPointLight(PointLight light);

	void removeModelInstance(std::shared_ptr<const ModelInstance> modelInstance);
	//void removePointLight(PointLight light);

	void draw(const Camera& c, bool doFrustrumCulling);
};

