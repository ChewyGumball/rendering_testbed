#include <Models/ModelInstance.h>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

ModelInstance::ModelInstance(std::shared_ptr<Model> model) : baseModel(model), transform(glm::mat4())
{
}


ModelInstance::~ModelInstance()
{
}

void ModelInstance::rotate(glm::vec3 axis, float degrees)
{
	transform = glm::rotate(transform, degrees, axis);
}

void ModelInstance::translate(glm::vec3 offset)
{
	transform = glm::translate(transform, offset);
}

void ModelInstance::scale(glm::vec3 scales)
{
	transform = glm::scale(transform, scales);
}

std::shared_ptr<const Model> ModelInstance::model() const { return baseModel; }

glm::mat4 ModelInstance::transformMatrix() const { return transform; }

Culling::BoundingSphere ModelInstance::bounds() const
{
	Culling::BoundingSphere meshBounds = baseModel->mesh()->bounds();
	return Culling::BoundingSphere(glm::vec3(transform * glm::vec4(meshBounds.center(),1)), meshBounds.radius());
}

uint64_t ModelInstance::triangleCount() const
{
	return baseModel->triangleCount();
}
