#pragma once
#include <memory>
#include <Models/Model.h>
#include <glm/glm.hpp>

class ModelInstance
{
protected:
	std::shared_ptr<Model> baseModel;
	glm::mat4 transform;
public:
	ModelInstance(std::shared_ptr<Model> model);
	~ModelInstance();

	void rotate(glm::vec3 axis, float degrees);
	void translate(glm::vec3 offset);
	void scale(glm::vec3 scales);

	glm::vec3 position() const;

	std::shared_ptr<const Model> model() const;
	glm::mat4 transformMatrix() const;
	Culling::BoundingSphere bounds() const;

	uint64_t triangleCount() const;
};

