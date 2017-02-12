#pragma once
#include <memory>
#include <vector>
#include <unordered_map>

#include <Renderer\Camera.h>
#include <Renderer\PointLight.h>
#include <Models\ModelInstance.h>
#include <Renderer\FrameBuffer.h>
#include <Renderer\RenderOptions.h>

class Renderer
{
public:
	Renderer() {};
	virtual ~Renderer() {};

	virtual void clearFrameBuffer(std::shared_ptr<const FrameBuffer> frameBuffer, glm::vec4 clearColour) = 0;
	virtual void draw(const std::vector<std::shared_ptr<const ModelInstance>>& instances, const std::shared_ptr<Camera> camera, RenderOptions& options) = 0;

	virtual void addFrameBufferResources(std::shared_ptr<const FrameBuffer> frameBuffer) = 0;
	virtual void addModelInstanceResources(std::shared_ptr<const ModelInstance> modelInstance) = 0;

	virtual void addPointLight(PointLight light) = 0;
};

