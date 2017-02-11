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
#include <Renderer/FrameBuffer.h>
#include <Renderer\RenderOptions.h>

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
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	void addFrameBuffer(std::shared_ptr<const FrameBuffer> frameBuffer);
	void addModelInstance(std::shared_ptr<const ModelInstance> modelInstance);
	void addPointLight(PointLight light);

	void removeModelInstance(std::shared_ptr<const ModelInstance> modelInstance);
	//void removePointLight(PointLight light);

	void draw(std::shared_ptr<FrameBuffer> frameBuffer, const std::shared_ptr<Camera> camera, RenderOptions& options);
};

