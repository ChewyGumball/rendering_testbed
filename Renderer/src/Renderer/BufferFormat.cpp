#include "Renderer/BufferFormat.h"
#include <algorithm>
//#include <assert.h>

#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace {
	const std::unordered_map<BufferElementType, BufferElementTypeSize> typeSizes {
		{ BufferElementType::FLOAT_SCALAR, static_cast<BufferElementTypeSize>(sizeof(float)) },
		{ BufferElementType::FLOAT_VEC2, static_cast<BufferElementTypeSize>(sizeof(glm::vec2)) },
		{ BufferElementType::FLOAT_VEC3, static_cast<BufferElementTypeSize>(sizeof(glm::vec3)) },
		{ BufferElementType::FLOAT_VEC4, static_cast<BufferElementTypeSize>(sizeof(glm::vec4)) },
		{ BufferElementType::DOUBLE_SCALAR, static_cast<BufferElementTypeSize>(sizeof(double)) },
		{ BufferElementType::DOUBLE_VEC2, static_cast<BufferElementTypeSize>(sizeof(glm::dvec2)) },
		{ BufferElementType::DOUBLE_VEC3, static_cast<BufferElementTypeSize>(sizeof(glm::dvec3)) },
		{ BufferElementType::DOUBLE_VEC4, static_cast<BufferElementTypeSize>(sizeof(glm::dvec4)) },
		{ BufferElementType::BOOL_SCALAR, static_cast<BufferElementTypeSize>(sizeof(bool)) },
		{ BufferElementType::BOOL_VEC2, static_cast<BufferElementTypeSize>(sizeof(glm::bvec2)) },
		{ BufferElementType::BOOL_VEC3, static_cast<BufferElementTypeSize>(sizeof(glm::bvec3)) },
		{ BufferElementType::BOOL_VEC4, static_cast<BufferElementTypeSize>(sizeof(glm::bvec4)) },
		{ BufferElementType::INT_SCALAR, static_cast<BufferElementTypeSize>(sizeof(int32_t)) },
		{ BufferElementType::INT_VEC2, static_cast<BufferElementTypeSize>(sizeof(glm::ivec2)) },
		{ BufferElementType::INT_VEC3, static_cast<BufferElementTypeSize>(sizeof(glm::ivec3)) },
		{ BufferElementType::INT_VEC4, static_cast<BufferElementTypeSize>(sizeof(glm::ivec4)) },
		{ BufferElementType::UINT_SCALAR, static_cast<BufferElementTypeSize>(sizeof(uint32_t)) },
		{ BufferElementType::UINT_VEC2, static_cast<BufferElementTypeSize>(sizeof(glm::uvec2)) },
		{ BufferElementType::UINT_VEC3, static_cast<BufferElementTypeSize>(sizeof(glm::uvec3)) },
		{ BufferElementType::UINT_VEC4, static_cast<BufferElementTypeSize>(sizeof(glm::uvec4)) },
		{ BufferElementType::MAT2, static_cast<BufferElementTypeSize>(sizeof(glm::mat2)) },
		{ BufferElementType::MAT3, static_cast<BufferElementTypeSize>(sizeof(glm::mat3)) },
		{ BufferElementType::MAT4, static_cast<BufferElementTypeSize>(sizeof(glm::mat4)) }
	};
}

BufferFormat::BufferFormat() : m_size(0)
{
}

BufferFormat::BufferFormat(std::vector<std::pair<std::string, BufferElementType>>& format)
{
	uint16_t offset = 0;
	for (auto& value : format) {
		m_offsets[value.first] = std::make_pair(offset, value.second);
		offset += typeSizes.at(value.second);
	}

	m_size = offset;
}

void BufferFormat::setState(DataBuffer& instanceData, std::string name, BufferElementType type, uint8_t * data) const
{
	//assert(m_offsets.at(name).second == type);
	std::copy(data, data + typeSizes.at(type), instanceData.begin() + m_offsets.at(name).first);
}

const std::pair<uint16_t, BufferElementType>& BufferFormat::operator[](std::string name) const
{
	return m_offsets.at(name);
}

const std::unordered_map<std::string, std::pair<uint16_t, BufferElementType>>& BufferFormat::offsets() const
{
	return m_offsets;
}

DataBuffer BufferFormat::initialData() const
{
	DataBuffer data(m_size);

	for (auto& field : m_offsets) {
		switch (field.second.second) {
			case BufferElementType::MAT4: {
				const uint8_t* matrix = reinterpret_cast<const uint8_t*>(glm::value_ptr(glm::mat4()));
				std::copy(matrix, matrix + typeSizes.at(BufferElementType::MAT4), data.begin() + field.second.first);
				break;
			}
			case BufferElementType::MAT3: {
				const uint8_t* matrix = reinterpret_cast<const uint8_t*>(glm::value_ptr(glm::mat3()));
				std::copy(matrix, matrix + typeSizes.at(BufferElementType::MAT3), data.begin() + field.second.first);
				break;
			}
		}
	}

	return data;
}

uint16_t BufferFormat::size() const
{
	return m_size;
}

BufferElementTypeSize BufferFormat::sizeOfType(BufferElementType type)
{
	return typeSizes.at(type);
}
