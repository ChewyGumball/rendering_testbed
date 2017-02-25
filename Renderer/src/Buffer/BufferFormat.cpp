#include "Buffer/BufferFormat.h"
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

BufferFormat::BufferFormat() : m_size(0), m_arrayType(BufferElementType::UNKNOWN), m_arrayElementFormat(nullptr)
{
}

BufferFormat::BufferFormat(uint64_t arraySize, BufferElementType arrayType, const std::shared_ptr<BufferFormat> arrayElementFormat)
	: m_size(arraySize * (arrayElementFormat == nullptr ? typeSizes.at(arrayType) : arrayElementFormat->size())), m_arrayType(arrayType), m_arrayElementFormat(arrayElementFormat)
{
}

BufferFormat::BufferFormat(std::vector<std::pair<std::string, BufferElementType>>& format, std::unordered_map<std::string, const std::shared_ptr<BufferFormat>> nestedBufferFormats)
	: m_nestedBufferFormats(nestedBufferFormats), m_arrayType(BufferElementType::UNKNOWN), m_arrayElementFormat(nullptr)
{
	uint64_t offset = 0;
	for (auto& value : format) {
		m_offsets[value.first] = std::make_pair(offset, value.second);
		if (value.second == BufferElementType::BUFFER || value.second == BufferElementType::ARRAY) {
			assert(nestedBufferFormats.count(value.first) != 0);
			offset += nestedBufferFormats[value.first]->size();
		}
		else {
			offset += typeSizes.at(value.second);
		}
	}

	m_size = offset;
}


const std::pair<BufferOffet, BufferElementType>& BufferFormat::at(std::string name) const
{
	return m_offsets.at(name);
}

const std::unordered_map<std::string, std::pair<BufferOffet, BufferElementType>>& BufferFormat::offsets() const
{
	return m_offsets;
}

uint64_t BufferFormat::size() const
{
	return m_size;
}

BufferElementTypeSize BufferFormat::sizeOfType(BufferElementType type)
{
	assert(typeSizes.count(type) != 0);
	return typeSizes.at(type);
}

const std::shared_ptr<BufferFormat> BufferFormat::nestedFormat(std::string name) const
{
	return m_nestedBufferFormats.at(name);
}

BufferElementType BufferFormat::arrayType() const
{
	assert(m_arrayType != BufferElementType::UNKNOWN);
	return m_arrayType;
}

uint64_t BufferFormat::arraySize() const
{
	assert(m_arrayType != BufferElementType::UNKNOWN);

	if (m_arrayType == BufferElementType::BUFFER) {
		return m_size / m_arrayElementFormat->size();
	}
	else {
		return m_size / typeSizes.at(m_arrayType);
	}
}

const std::shared_ptr<BufferFormat> BufferFormat::arrayElementFormat() const
{
	return m_arrayElementFormat;
}
