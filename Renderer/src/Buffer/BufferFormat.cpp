#include "Buffer/BufferFormat.h"
#include <algorithm>
//#include <assert.h>

#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Renderer;

namespace {
	uint64_t align(uint64_t offset, uint64_t alignment) {
		uint64_t spill = offset % alignment;
		if (spill > 0) {
			return offset + alignment - spill;
		}
		return offset;
	}
}

const std::unordered_map<BufferPackingType, std::unordered_map<BufferElementType, uint64_t>> BufferFormat::typeSizes = {
	{
		BufferPackingType::PACKED,{
			{ BufferElementType::FLOAT_SCALAR, sizeof(float) },
			{ BufferElementType::FLOAT_VEC2, sizeof(glm::vec2) },
			{ BufferElementType::FLOAT_VEC3, sizeof(glm::vec3) },
			{ BufferElementType::FLOAT_VEC4, sizeof(glm::vec4) },
			{ BufferElementType::DOUBLE_SCALAR, sizeof(double) },
			{ BufferElementType::DOUBLE_VEC2, sizeof(glm::dvec2) },
			{ BufferElementType::DOUBLE_VEC3, sizeof(glm::dvec3) },
			{ BufferElementType::DOUBLE_VEC4, sizeof(glm::dvec4) },
			{ BufferElementType::BOOL_SCALAR, sizeof(bool) },
			{ BufferElementType::BOOL_VEC2, sizeof(glm::bvec2) },
			{ BufferElementType::BOOL_VEC3, sizeof(glm::bvec3) },
			{ BufferElementType::BOOL_VEC4, sizeof(glm::bvec4) },
			{ BufferElementType::INT_SCALAR, sizeof(int32_t) },
			{ BufferElementType::INT_VEC2, sizeof(glm::ivec2) },
			{ BufferElementType::INT_VEC3, sizeof(glm::ivec3) },
			{ BufferElementType::INT_VEC4, sizeof(glm::ivec4) },
			{ BufferElementType::UINT_SCALAR, sizeof(uint32_t) },
			{ BufferElementType::UINT_VEC2, sizeof(glm::uvec2) },
			{ BufferElementType::UINT_VEC3, sizeof(glm::uvec3) },
			{ BufferElementType::UINT_VEC4, sizeof(glm::uvec4) },
			{ BufferElementType::MAT2, sizeof(glm::mat2) },
			{ BufferElementType::MAT3, sizeof(glm::mat3) },
			{ BufferElementType::MAT4, sizeof(glm::mat4) }
		}
	},
	{
		BufferPackingType::OPENGL_STD140,{
			{ BufferElementType::FLOAT_SCALAR, sizeof(float) },
			{ BufferElementType::FLOAT_VEC2, sizeof(glm::vec2) },
			{ BufferElementType::FLOAT_VEC3, sizeof(glm::vec3) },
			{ BufferElementType::FLOAT_VEC4, sizeof(glm::vec4) },
			{ BufferElementType::DOUBLE_SCALAR, sizeof(double) },
			{ BufferElementType::DOUBLE_VEC2, sizeof(glm::dvec2) },
			{ BufferElementType::DOUBLE_VEC3, sizeof(glm::dvec3) },
			{ BufferElementType::DOUBLE_VEC4, sizeof(glm::dvec4) },
			{ BufferElementType::BOOL_SCALAR, sizeof(bool) },
			{ BufferElementType::BOOL_VEC2, sizeof(glm::bvec2) },
			{ BufferElementType::BOOL_VEC3, sizeof(glm::bvec3) },
			{ BufferElementType::BOOL_VEC4, sizeof(glm::bvec4) },
			{ BufferElementType::INT_SCALAR, sizeof(int32_t) },
			{ BufferElementType::INT_VEC2, sizeof(glm::ivec2) },
			{ BufferElementType::INT_VEC3, sizeof(glm::ivec3) },
			{ BufferElementType::INT_VEC4, sizeof(glm::ivec4) },
			{ BufferElementType::UINT_SCALAR, sizeof(uint32_t) },
			{ BufferElementType::UINT_VEC2, sizeof(glm::uvec2) },
			{ BufferElementType::UINT_VEC3, sizeof(glm::uvec3) },
			{ BufferElementType::UINT_VEC4, sizeof(glm::uvec4) },
			{ BufferElementType::MAT2, 2 * sizeof(glm::vec4) },
			{ BufferElementType::MAT3, 3 * sizeof(glm::vec4) },
			{ BufferElementType::MAT4, 4 * sizeof(glm::vec4) }
		}
	}
};

const std::unordered_map<BufferPackingType, std::unordered_map<BufferElementType, uint64_t>> BufferFormat::typeAlignments = {
	{
		BufferPackingType::PACKED,{
			{ BufferElementType::FLOAT_SCALAR, 1 },
			{ BufferElementType::FLOAT_VEC2, 1 },
			{ BufferElementType::FLOAT_VEC3, 1 },
			{ BufferElementType::FLOAT_VEC4, 1 },
			{ BufferElementType::DOUBLE_SCALAR, 1 },
			{ BufferElementType::DOUBLE_VEC2, 1 },
			{ BufferElementType::DOUBLE_VEC3, 1 },
			{ BufferElementType::DOUBLE_VEC4, 1 },
			{ BufferElementType::BOOL_SCALAR, 1 },
			{ BufferElementType::BOOL_VEC2, 1 },
			{ BufferElementType::BOOL_VEC3, 1 },
			{ BufferElementType::BOOL_VEC4, 1 },
			{ BufferElementType::INT_SCALAR, 1 },
			{ BufferElementType::INT_VEC2, 1 },
			{ BufferElementType::INT_VEC3, 1 },
			{ BufferElementType::INT_VEC4, 1 },
			{ BufferElementType::UINT_SCALAR, 1 },
			{ BufferElementType::UINT_VEC2, 1 },
			{ BufferElementType::UINT_VEC3, 1 },
			{ BufferElementType::UINT_VEC4, 1 },
			{ BufferElementType::MAT2, 1 },
			{ BufferElementType::MAT3, 1 },
			{ BufferElementType::MAT4, 1 }
		}
	},
	{
		BufferPackingType::OPENGL_STD140,{
			{ BufferElementType::FLOAT_SCALAR, sizeof(float) },
			{ BufferElementType::FLOAT_VEC2, sizeof(glm::vec2) },
			{ BufferElementType::FLOAT_VEC3, sizeof(glm::vec4) },
			{ BufferElementType::FLOAT_VEC4, sizeof(glm::vec4) },
			{ BufferElementType::DOUBLE_SCALAR, sizeof(double) },
			{ BufferElementType::DOUBLE_VEC2, sizeof(glm::dvec2) },
			{ BufferElementType::DOUBLE_VEC3, sizeof(glm::dvec4) },
			{ BufferElementType::DOUBLE_VEC4, sizeof(glm::dvec4) },
			{ BufferElementType::BOOL_SCALAR, sizeof(bool) },
			{ BufferElementType::BOOL_VEC2, sizeof(glm::bvec2) },
			{ BufferElementType::BOOL_VEC3, sizeof(glm::bvec4) },
			{ BufferElementType::BOOL_VEC4, sizeof(glm::bvec4) },
			{ BufferElementType::INT_SCALAR, sizeof(int32_t) },
			{ BufferElementType::INT_VEC2, sizeof(glm::ivec2) },
			{ BufferElementType::INT_VEC3, sizeof(glm::ivec4) },
			{ BufferElementType::INT_VEC4, sizeof(glm::ivec4) },
			{ BufferElementType::UINT_SCALAR, sizeof(uint32_t) },
			{ BufferElementType::UINT_VEC2, sizeof(glm::uvec2) },
			{ BufferElementType::UINT_VEC3, sizeof(glm::uvec4) },
			{ BufferElementType::UINT_VEC4, sizeof(glm::uvec4) },
			{ BufferElementType::MAT2, sizeof(glm::vec4) },
			{ BufferElementType::MAT3, sizeof(glm::vec4) },
			{ BufferElementType::MAT4, sizeof(glm::vec4) }
		}
	}
};

BufferFormat::BufferFormat() : m_size(0), m_arrayType(BufferElementType::UNKNOWN), m_arrayElementFormat(nullptr)
{
}

Renderer::BufferFormat::BufferFormat(uint64_t arraySize, BufferElementType arrayElementType, BufferPackingType packingType)
	: m_size(arraySize * typeSizes.at(packingType).at(arrayElementType)), m_arrayType(arrayElementType), m_arrayElementFormat(nullptr), m_packingType(packingType)
{
}

BufferFormat::BufferFormat(uint64_t arraySize, std::shared_ptr<const BufferFormat> arrayElementFormat)
	: m_size(arraySize * arrayElementFormat->size()), 
	  m_arrayType(arrayElementFormat->arrayType() != BufferElementType::UNKNOWN ? BufferElementType::ARRAY : BufferElementType::BUFFER), 
	  m_arrayElementFormat(arrayElementFormat),
	  m_packingType(arrayElementFormat->packingType())
{
}

BufferFormat::BufferFormat(std::vector<std::pair<std::string, BufferElementType>>& format, std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> nestedBufferFormats, BufferPackingType packingType)
	: m_nestedBufferFormats(nestedBufferFormats), m_arrayType(BufferElementType::UNKNOWN), m_arrayElementFormat(nullptr), m_packingType(packingType)
{
	uint64_t offset = 0;
	const std::unordered_map<BufferElementType, uint64_t>& sizes = typeSizes.at(packingType);
	const std::unordered_map<BufferElementType, uint64_t>& alignments = typeAlignments.at(packingType);

	for (auto& value : format) {
		uint64_t size;
		if (value.second == BufferElementType::BUFFER || value.second == BufferElementType::ARRAY) {
			assert(nestedBufferFormats.count(value.first) != 0);

			//align the offset to a vec4:
			//	std140 requires this, packed has the same alignment for all types so it doesn't matter which type's alignment is used
			offset = align(offset, alignments.at(BufferElementType::FLOAT_VEC4));
			size = nestedBufferFormats[value.first]->size();
		}
		else {
			//align the offset according to the alignment of the element type
			offset = align(offset, alignments.at(value.second));
			size = sizes.at(value.second);
		}

		m_offsets[value.first] = std::make_pair(offset, value.second);
		offset += size;
	}

	//the size is a multiple of vec4 alignment so make sure to add extra if required
	if (packingType == BufferPackingType::OPENGL_STD140) {
		offset = align(offset, alignments.at(BufferElementType::FLOAT_VEC4));
	}

	m_size = offset;
}


const std::pair<uint64_t, BufferElementType>& BufferFormat::at(std::string name) const
{
	return m_offsets.at(name);
}

std::vector<std::pair<std::string, std::pair<uint64_t, BufferElementType>>> BufferFormat::orderedOffsets() const
{
	std::vector<std::pair<std::string, std::pair<uint64_t, BufferElementType>>> orderedOffsets(m_offsets.begin(), m_offsets.end());
	std::sort(orderedOffsets.begin(), orderedOffsets.end(), [](auto& a, auto&b) { return a.second.first < b.second.first; });
	return orderedOffsets;
}

const std::unordered_map<std::string, std::pair<uint64_t, BufferElementType>>& BufferFormat::offsets() const
{
	return m_offsets;
}

BufferPackingType BufferFormat::packingType() const
{
	return m_packingType;
}

uint64_t BufferFormat::size() const
{
	return m_size;
}

uint64_t BufferFormat::sizeOfType(BufferElementType type, BufferPackingType packingType)
{
	return typeSizes.at(packingType).at(type);
}

const std::unordered_map<std::string, std::shared_ptr<const BufferFormat>>& BufferFormat::nestedFormats() const
{
	return m_nestedBufferFormats;
}

std::shared_ptr<const BufferFormat> BufferFormat::nestedFormat(std::string name) const
{
	return m_nestedBufferFormats.at(name);
}

BufferElementType BufferFormat::arrayType() const
{
	return m_arrayType;
}

uint64_t BufferFormat::arraySize() const
{
	assert(m_arrayType != BufferElementType::UNKNOWN);

	if (m_arrayType == BufferElementType::BUFFER || m_arrayType == BufferElementType::ARRAY) {
		return m_size / m_arrayElementFormat->size();
	}
	else {
		return m_size / typeSizes.at(m_packingType).at(m_arrayType);
	}
}

std::shared_ptr<const BufferFormat> BufferFormat::arrayElementFormat() const
{
	return m_arrayElementFormat;
}
