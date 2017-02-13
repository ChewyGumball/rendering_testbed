#include "Models/ModelInstanceStateFormat.h"
#include <algorithm>
#include <assert.h>

#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace {
	std::unordered_map<ModelInstanceStateType, uint8_t> typeSizes {
		{ ModelInstanceStateType::FLOAT_SCALAR, static_cast<uint8_t>(sizeof(float) * 1) },
		{ ModelInstanceStateType::FLOAT_VEC2, static_cast<uint8_t>(sizeof(float) * 2) },
		{ ModelInstanceStateType::FLOAT_VEC3, static_cast<uint8_t>(sizeof(float) * 3) },
		{ ModelInstanceStateType::FLOAT_VEC4, static_cast<uint8_t>(sizeof(float) * 4) },
		{ ModelInstanceStateType::DOUBLE_SCALAR, static_cast<uint8_t>(sizeof(double) * 1) },
		{ ModelInstanceStateType::DOUBLE_VEC2, static_cast<uint8_t>(sizeof(double) * 2) },
		{ ModelInstanceStateType::DOUBLE_VEC3, static_cast<uint8_t>(sizeof(double) * 3) },
		{ ModelInstanceStateType::DOUBLE_VEC4, static_cast<uint8_t>(sizeof(double) * 4) },
		{ ModelInstanceStateType::BOOL_SCALAR, static_cast<uint8_t>(sizeof(bool) * 1) },
		{ ModelInstanceStateType::BOOL_VEC2, static_cast<uint8_t>(sizeof(bool) * 2) },
		{ ModelInstanceStateType::BOOL_VEC3, static_cast<uint8_t>(sizeof(bool) * 3) },
		{ ModelInstanceStateType::BOOL_VEC4, static_cast<uint8_t>(sizeof(bool) * 4) },
		{ ModelInstanceStateType::INT_SCALAR, static_cast<uint8_t>(sizeof(int32_t) * 1) },
		{ ModelInstanceStateType::INT_VEC2, static_cast<uint8_t>(sizeof(int32_t) * 2) },
		{ ModelInstanceStateType::INT_VEC3, static_cast<uint8_t>(sizeof(int32_t) * 3) },
		{ ModelInstanceStateType::INT_VEC4, static_cast<uint8_t>(sizeof(int32_t) * 4) },
		{ ModelInstanceStateType::UINT_SCALAR, static_cast<uint8_t>(sizeof(uint32_t) * 1) },
		{ ModelInstanceStateType::UINT_VEC2, static_cast<uint8_t>(sizeof(uint32_t) * 2) },
		{ ModelInstanceStateType::UINT_VEC3, static_cast<uint8_t>(sizeof(uint32_t) * 3) },
		{ ModelInstanceStateType::UINT_VEC4, static_cast<uint8_t>(sizeof(uint32_t) * 4) },
		{ ModelInstanceStateType::MAT2, static_cast<uint8_t>(sizeof(float) * 4) },
		{ ModelInstanceStateType::MAT3, static_cast<uint8_t>(sizeof(float) * 9) },
		{ ModelInstanceStateType::MAT4, static_cast<uint8_t>(sizeof(float) * 16) }
	};
}

ModelInstanceStateFormat::ModelInstanceStateFormat() : m_size(0)
{
}

ModelInstanceStateFormat::ModelInstanceStateFormat(std::vector<std::pair<std::string, ModelInstanceStateType>>& format)
{
	uint8_t offset = 0;
	for (auto& value : format) {
		offsets[value.first] = std::make_pair(offset, value.second);
		offset += typeSizes[value.second];
	}

	m_size = offset;
}

void ModelInstanceStateFormat::setState(std::vector<uint8_t>& instanceData, std::string name, ModelInstanceStateType type, uint8_t * data) const
{
	assert(offsets[name].second == type);
	std::copy(data, data + typeSizes[type], instanceData.begin() + offsets[name].first);
}

const std::pair<uint8_t, ModelInstanceStateType>& ModelInstanceStateFormat::operator[](std::string name) const
{
	return offsets[name];
}

std::vector<uint8_t> ModelInstanceStateFormat::initialData() const
{
	std::vector<uint8_t> data(m_size);

	for (auto& field : offsets) {
		switch (field.second.second) {
			case ModelInstanceStateType::MAT4: {
				const uint8_t* matrix = reinterpret_cast<const uint8_t*>(glm::value_ptr(glm::mat4()));
				std::copy(matrix, matrix + typeSizes[ModelInstanceStateType::MAT4], data.begin() + field.second.first);
				break;
			}
			case ModelInstanceStateType::MAT3: {
				const uint8_t* matrix = reinterpret_cast<const uint8_t*>(glm::value_ptr(glm::mat3()));
				std::copy(matrix, matrix + typeSizes[ModelInstanceStateType::MAT3], data.begin() + field.second.first);
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
