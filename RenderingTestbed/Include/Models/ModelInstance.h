#pragma once
#include <memory>
#include <Models/Model.h>
#include <glm/glm.hpp>

class ModelInstance
{
protected:
	Model baseModel;
	glm::mat4 transform;
public:
	ModelInstance(Model model);
	~ModelInstance();

	void rotate(glm::vec3 axis, float degrees);
	void translate(glm::vec3 offset);
	void scale(glm::vec3 scales);

	std::shared_ptr<const Model> model() const;
	glm::mat4 transformMatrix() const;
};

