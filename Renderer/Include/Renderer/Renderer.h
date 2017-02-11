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
	uint64_t trianglesDrawn;
	Renderer() {};
	virtual ~Renderer() {};
	virtual void draw(std::shared_ptr<FrameBuffer> frameBuffer, const std::shared_ptr<Camera> camera, RenderOptions& options) = 0;

	virtual void addFrameBuffer(std::shared_ptr<const FrameBuffer> frameBuffer) = 0;
	virtual void addModelInstance(std::shared_ptr<const ModelInstance> modelInstance) = 0;
	virtual void removeModelInstance(std::shared_ptr<const ModelInstance> modelInstance) = 0;

	virtual void addPointLight(PointLight light) = 0;

	std::unordered_map<size_t, std::vector<std::shared_ptr<const ModelInstance>>> cull(const std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<const ModelInstance>> instances);
};

