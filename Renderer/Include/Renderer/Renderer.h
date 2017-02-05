#pragma once
#include <memory>
#include <vector>
#include <unordered_map>

#include <Renderer\Camera.h>
#include <Renderer\PointLight.h>
#include <Models\ModelInstance.h>

class Renderer
{
public:
	uint64_t trianglesDrawn;
	Renderer() {};
	virtual ~Renderer() {};
	virtual void draw(const std::shared_ptr<Camera> c, bool doFrustrumCulling) = 0;

	virtual void addModelInstance(std::shared_ptr<const ModelInstance> modelInstance) = 0;
	virtual void removeModelInstance(std::shared_ptr<const ModelInstance> modelInstance) = 0;

	virtual void addPointLight(PointLight light) = 0;

	std::unordered_map<size_t, std::vector<std::shared_ptr<const ModelInstance>>> cull(const std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<const ModelInstance>> instances);
};

