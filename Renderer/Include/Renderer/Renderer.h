#pragma once
#include <memory>
#include <vector>
#include <unordered_map>

#include <Renderer/Camera.h>
#include <Renderer/PointLight.h>
#include <Renderer/ModelInstance.h>
#include <Renderer/FrameBuffer.h>
#include <Renderer/RenderOptions.h>

class Renderer
{
public:
	Renderer() {};
	virtual ~Renderer() {};

	virtual void processRenderingOptions(RenderOptions& options) = 0;
	virtual void draw(const std::vector<std::shared_ptr<const ModelInstance>>& instances, const std::shared_ptr<Camera> camera) = 0;

	virtual void addPointLight(PointLight light) = 0;
};

