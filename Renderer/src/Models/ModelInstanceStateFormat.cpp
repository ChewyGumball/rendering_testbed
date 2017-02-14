#include "Models/ModelInstanceStateFormat.h"
#include <algorithm>
#include <assert.h>

#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace {
	const std::unordered_map<ModelInstanceStateType, uint8_t> typeSizes {
		{ ModelInstanceStateType::FLOAT_SCALAR, static_cast<uint8_t>(sizeof(float)) },
		{ ModelInstanceStateType::FLOAT_VEC2, static_cast<uint8_t>(sizeof(glm::vec2)) },
		{ ModelInstanceStateType::FLOAT_VEC3, static_cast<uint8_t>(sizeof(glm::vec3)) },
		{ ModelInstanceStateType::FLOAT_VEC4, static_cast<uint8_t>(sizeof(glm::vec4)) },
		{ ModelInstanceStateType::DOUBLE_SCALAR, static_cast<uint8_t>(sizeof(double)) },
		{ ModelInstanceStateType::DOUBLE_VEC2, static_cast<uint8_t>(sizeof(glm::dvec2)) },
		{ ModelInstanceStateType::DOUBLE_VEC3, static_cast<uint8_t>(sizeof(glm::dvec3)) },
		{ ModelInstanceStateType::DOUBLE_VEC4, static_cast<uint8_t>(sizeof(glm::dvec4)) },
		{ ModelInstanceStateType::BOOL_SCALAR, static_cast<uint8_t>(sizeof(bool)) },
		{ ModelInstanceStateType::BOOL_VEC2, static_cast<uint8_t>(sizeof(glm::bvec2)) },
		{ ModelInstanceStateType::BOOL_VEC3, static_cast<uint8_t>(sizeof(glm::bvec3)) },
		{ ModelInstanceStateType::BOOL_VEC4, static_cast<uint8_t>(sizeof(glm::bvec4)) },
		{ ModelInstanceStateType::INT_SCALAR, static_cast<uint8_t>(sizeof(int32_t)) },
		{ ModelInstanceStateType::INT_VEC2, static_cast<uint8_t>(sizeof(glm::ivec2)) },
		{ ModelInstanceStateType::INT_VEC3, static_cast<uint8_t>(sizeof(glm::ivec3)) },
		{ ModelInstanceStateType::INT_VEC4, static_cast<uint8_t>(sizeof(glm::ivec4)) },
		{ ModelInstanceStateType::UINT_SCALAR, static_cast<uint8_t>(sizeof(uint32_t)) },
		{ ModelInstanceStateType::UINT_VEC2, static_cast<uint8_t>(sizeof(glm::uvec2)) },
		{ ModelInstanceStateType::UINT_VEC3, static_cast<uint8_t>(sizeof(glm::uvec3)) },
		{ ModelInstanceStateType::UINT_VEC4, static_cast<uint8_t>(sizeof(glm::uvec4)) },
		{ ModelInstanceStateType::MAT2, static_cast<uint8_t>(sizeof(glm::mat2)) },
		{ ModelInstanceStateType::MAT3, static_cast<uint8_t>(sizeof(glm::mat3)) },
		{ ModelInstanceStateType::MAT4, static_cast<uint8_t>(sizeof(glm::mat4)) }
	};
}

ModelInstanceStateFormat::ModelInstanceStateFormat() : m_size(0)
{
}

ModelInstanceStateFormat::ModelInstanceStateFormat(std::vector<std::pair<std::string, ModelInstanceStateType>>& format)
{
	uint8_t offset = 0;
	for (auto& value : format) {
		m_offsets[value.first] = std::make_pair(offset, value.second);
		offset += typeSizes.at(value.second);
	}

	m_size = offset;
}

void ModelInstanceStateFormat::setState(std::vector<uint8_t>& instanceData, std::string name, ModelInstanceStateType type, uint8_t * data) const
{
	assert(m_offsets.at(name).second == type);
	std::copy(data, data + typeSizes.at(type), instanceData.begin() + m_offsets.at(name).first);
}

const std::pair<uint8_t, ModelInstanceStateType>& ModelInstanceStateFormat::operator[](std::string name) const
{
	return m_offsets.at(name);
}

const std::unordered_map<std::string, std::pair<uint8_t, ModelInstanceStateType>>& ModelInstanceStateFormat::offsets() const
{
	return m_offsets;
}

std::vector<uint8_t> ModelInstanceStateFormat::initialData() const
{
	std::vector<uint8_t> data(m_size);

	for (auto& field : m_offsets) {
		switch (field.second.second) {
			case ModelInstanceStateType::MAT4: {
				const uint8_t* matrix = reinterpret_cast<const uint8_t*>(glm::value_ptr(glm::mat4()));
				std::copy(matrix, matrix + typeSizes.at(ModelInstanceStateType::MAT4), data.begin() + field.second.first);
				break;
			}
			case ModelInstanceStateType::MAT3: {
				const uint8_t* matrix = reinterpret_cast<const uint8_t*>(glm::value_ptr(glm::mat3()));
				std::copy(matrix, matrix + typeSizes.at(ModelInstanceStateType::MAT3), data.begin() + field.second.first);
				break;
			}
		}
	}

	return data;
}

uint8_t ModelInstanceStateFormat::size() const
{
	return m_size;
}

uint8_t ModelInstanceStateFormat::sizeOfType(ModelInstanceStateType type)
{
	return typeSizes.at(type);
}
