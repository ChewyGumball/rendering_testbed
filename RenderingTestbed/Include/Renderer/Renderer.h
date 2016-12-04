#pragma once
#include <memory>
#include <vector>

#include <Renderer\Camera.h>
#include <Renderer\PointLight.h>
#include <Models\ModelInstance.h>

class Renderer
{
public:
	Renderer() {};
	virtual ~Renderer() {};
	virtual void draw(const Camera& c) = 0;

	virtual void addModelInstance(std::shared_ptr<const ModelInstance> modelInstance) = 0;
	virtual void addPointLight(PointLight light) = 0;

	virtual void removeModelInstance(std::shared_ptr<const ModelInstance> modelInstance) = 0;

	std::vector<std::shared_ptr<ModelInstance>> cull(const Camera& camera, std::vector<std::shared_ptr<ModelInstance>> instances);
};

