#include <algorithm>
#include <Renderer/ModelInstance.h>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Renderer/BufferFormat.h>
#include <glm/gtc/type_ptr.hpp>

ModelInstance::ModelInstance(std::shared_ptr<Model> model) : baseModel(model), m_instanceData(model->shader()->expectedInstanceStateFormat().initialData())
{
}

ModelInstance::~ModelInstance()
{
}

void ModelInstance::setState(std::string name, glm::vec2 value)
{
	baseModel->shader()->expectedInstanceStateFormat().setState(m_instanceData, name, BufferElementType::FLOAT_VEC2, reinterpret_cast<uint8_t*>(glm::value_ptr(value)));
}

void ModelInstance::setState(std::string name, glm::vec3 value)
{
	baseModel->shader()->expectedInstanceStateFormat().setState(m_instanceData, name, BufferElementType::FLOAT_VEC3, reinterpret_cast<uint8_t*>(glm::value_ptr(value)));
}

void ModelInstance::setState(std::string name, glm::vec4 value)
{
	baseModel->shader()->expectedInstanceStateFormat().setState(m_instanceData, name, BufferElementType::FLOAT_VEC4, reinterpret_cast<uint8_t*>(glm::value_ptr(value)));
}

void ModelInstance::setState(std::string name, uint32_t value)
{
	baseModel->shader()->expectedInstanceStateFormat().setState(m_instanceData, name, BufferElementType::UINT_SCALAR, reinterpret_cast<uint8_t*>(&value));
}

void ModelInstance::setState(std::string name, int32_t value)
{
	baseModel->shader()->expectedInstanceStateFormat().setState(m_instanceData, name, BufferElementType::INT_SCALAR, reinterpret_cast<uint8_t*>(&value));
}

void ModelInstance::setState(std::string name, bool value)
{
	baseModel->shader()->expectedInstanceStateFormat().setState(m_instanceData, name, BufferElementType::BOOL_SCALAR, reinterpret_cast<uint8_t*>(&value));
}

void ModelInstance::setState(std::string name, float value)
{
	baseModel->shader()->expectedInstanceStateFormat().setState(m_instanceData, name, BufferElementType::FLOAT_SCALAR, reinterpret_cast<uint8_t*>(&value));
}

void ModelInstance::setState(std::string name, glm::mat3 value)
{
	baseModel->shader()->expectedInstanceStateFormat().setState(m_instanceData, name, BufferElementType::MAT3, reinterpret_cast<uint8_t*>(glm::value_ptr(value)));
}

void ModelInstance::setState(std::string name, glm::mat4 value)
{
	baseModel->shader()->expectedInstanceStateFormat().setState(m_instanceData, name, BufferElementType::MAT4, reinterpret_cast<uint8_t*>(glm::value_ptr(value)));
}

glm::vec2 ModelInstance::getVec2State(std::string name) const
{
	auto& details = baseModel->shader()->expectedInstanceStateFormat()[name];
	assert(details.second == BufferElementType::FLOAT_VEC2);

	return glm::make_vec2(reinterpret_cast<const float*>(m_instanceData.data() + details.first));
}

glm::vec3 ModelInstance::getVec3State(std::string name) const
{
	auto& details = baseModel->shader()->expectedInstanceStateFormat()[name];
	assert(details.second == BufferElementType::FLOAT_VEC3);

	return glm::make_vec3(reinterpret_cast<const float*>(m_instanceData.data() + details.first));
}

glm::vec4 ModelInstance::getVec4State(std::string name) const
{
	auto& details = baseModel->shader()->expectedInstanceStateFormat()[name];
	assert(details.second == BufferElementType::FLOAT_VEC4);

	return glm::make_vec4(reinterpret_cast<const float*>(m_instanceData.data() + details.first));
}

uint32_t ModelInstance::getUIntState(std::string name) const
{
	auto& details = baseModel->shader()->expectedInstanceStateFormat()[name];
	assert(details.second == BufferElementType::UINT_SCALAR);

	return *reinterpret_cast<const uint32_t*>(m_instanceData.data() + details.first);
}

int32_t ModelInstance::getIntState(std::string name) const
{
	auto& details = baseModel->shader()->expectedInstanceStateFormat()[name];
	assert(details.second == BufferElementType::INT_SCALAR);

	return *reinterpret_cast<const int32_t*>(m_instanceData.data() + details.first);
}

bool ModelInstance::getBoolState(std::string name) const
{
	auto& details = baseModel->shader()->expectedInstanceStateFormat()[name];
	assert(details.second == BufferElementType::BOOL_SCALAR);

	return *reinterpret_cast<const bool*>(m_instanceData.data() + details.first);
}

float ModelInstance::getFloatState(std::string name) const
{
	auto& details = baseModel->shader()->expectedInstanceStateFormat()[name];
	assert(details.second == BufferElementType::FLOAT_SCALAR);

	return *reinterpret_cast<const float*>(m_instanceData.data() + details.first);
}

glm::mat3 ModelInstance::getMat3State(std::string name) const
{
	auto& details = baseModel->shader()->expectedInstanceStateFormat()[name];
	assert(details.second == BufferElementType::MAT3);

	return glm::make_mat3(reinterpret_cast<const float*>(m_instanceData.data() + details.first));
}

glm::mat4 ModelInstance::getMat4State(std::string name) const
{
	auto& details = baseModel->shader()->expectedInstanceStateFormat()[name];
	assert(details.second == BufferElementType::MAT4);

	return glm::make_mat4(reinterpret_cast<const float*>(m_instanceData.data() + details.first));
}

void ModelInstance::rotate(glm::vec3 axis, float degrees)
{
	setState("transform", glm::rotate(getMat4State("transform"), degrees, axis));
}

void ModelInstance::translate(glm::vec3 offset)
{
	setState("transform", glm::translate(getMat4State("transform"), offset));
}

void ModelInstance::scale(glm::vec3 scales)
{
	setState("transform", glm::scale(getMat4State("transform"), scales));
}

glm::vec3 ModelInstance::position() const
{
	return glm::vec3(getMat4State("transform") * glm::vec4(0,0,0,1));
}
void ModelInstance::model(std::shared_ptr<Model> newModel)
{
	baseModel = newModel;
}
std::shared_ptr<const Model> ModelInstance::model() const { return baseModel; }

const std::vector<uint8_t>& ModelInstance::instanceData() const
{
	return m_instanceData;
}

Culling::BoundingSphere ModelInstance::bounds() const
{
	Culling::BoundingSphere meshBounds = baseModel->mesh()->bounds();
	return Culling::BoundingSphere(glm::vec3(getMat4State("transform") * glm::vec4(meshBounds.center(),1)), meshBounds.radius());
}