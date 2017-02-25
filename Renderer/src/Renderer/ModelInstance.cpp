#include <algorithm>
#include <Renderer/ModelInstance.h>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Buffer/BufferFormat.h>
#include <glm/gtc/type_ptr.hpp>

ModelInstance::ModelInstance(std::shared_ptr<Model> model) : baseModel(model), m_instanceData(model->shader()->expectedInstanceStateFormat())
{
}

ModelInstance::~ModelInstance()
{
}

void ModelInstance::rotate(glm::vec3 axis, float degrees)
{
	m_instanceData.set("transform", glm::rotate(m_instanceData.getMat4("transform"), degrees, axis));
}

void ModelInstance::translate(glm::vec3 offset)
{
	m_instanceData.set("transform", glm::translate(m_instanceData.getMat4("transform"), offset));
}

void ModelInstance::scale(glm::vec3 scales)
{
	m_instanceData.set("transform", glm::scale(m_instanceData.getMat4("transform"), scales));
}

glm::vec3 ModelInstance::position() const
{
	return glm::vec3(m_instanceData.getMat4("transform") * glm::vec4(0,0,0,1));
}
void ModelInstance::model(std::shared_ptr<Model> newModel)
{
	baseModel = newModel;
}
std::shared_ptr<const Model> ModelInstance::model() const { return baseModel; }

DataBufferView& ModelInstance::instanceData()
{
	return m_instanceData;
}

const DataBufferView& ModelInstance::instanceData() const
{
	return m_instanceData;
}

Culling::BoundingSphere ModelInstance::bounds() const
{
	Culling::BoundingSphere meshBounds = baseModel->mesh()->bounds();
	return Culling::BoundingSphere(glm::vec3(m_instanceData.getMat4("transform") * glm::vec4(meshBounds.center(),1)), meshBounds.radius());
}
