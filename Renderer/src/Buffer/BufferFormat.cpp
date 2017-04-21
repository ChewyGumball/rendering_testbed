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

	const std::unordered_map<BufferElementType, BufferElementTypeSize> typeAlignments{
		{ BufferElementType::FLOAT_SCALAR, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::FLOAT_VEC2, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::FLOAT_VEC3, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::FLOAT_VEC4, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::DOUBLE_SCALAR, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::DOUBLE_VEC2, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::DOUBLE_VEC3, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::DOUBLE_VEC4, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::BOOL_SCALAR, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::BOOL_VEC2, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::BOOL_VEC3, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::BOOL_VEC4, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::INT_SCALAR, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::INT_VEC2, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::INT_VEC3, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::INT_VEC4, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::UINT_SCALAR, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::UINT_VEC2, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::UINT_VEC3, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::UINT_VEC4, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::MAT2, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::MAT3, static_cast<BufferElementTypeSize>(1) },
		{ BufferElementType::MAT4, static_cast<BufferElementTypeSize>(1) }
	};

	const std::unordered_map<BufferElementType, BufferElementTypeSize> openglSTD140TypeSizes{
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
		{ BufferElementType::MAT2, static_cast<BufferElementTypeSize>(2 * sizeof(glm::vec4)) },
		{ BufferElementType::MAT3, static_cast<BufferElementTypeSize>(3 * sizeof(glm::vec4)) },
		{ BufferElementType::MAT4, static_cast<BufferElementTypeSize>(4 * sizeof(glm::vec4)) }
	};

	const std::unordered_map<BufferElementType, BufferElementTypeSize> openglSTD140TypeAlignments {
		{ BufferElementType::FLOAT_SCALAR, static_cast<BufferElementTypeSize>(sizeof(float)) },
		{ BufferElementType::FLOAT_VEC2, static_cast<BufferElementTypeSize>(sizeof(glm::vec2)) },
		{ BufferElementType::FLOAT_VEC3, static_cast<BufferElementTypeSize>(sizeof(glm::vec4)) },
		{ BufferElementType::FLOAT_VEC4, static_cast<BufferElementTypeSize>(sizeof(glm::vec4)) },
		{ BufferElementType::DOUBLE_SCALAR, static_cast<BufferElementTypeSize>(sizeof(double)) },
		{ BufferElementType::DOUBLE_VEC2, static_cast<BufferElementTypeSize>(sizeof(glm::dvec2)) },
		{ BufferElementType::DOUBLE_VEC3, static_cast<BufferElementTypeSize>(sizeof(glm::dvec4)) },
		{ BufferElementType::DOUBLE_VEC4, static_cast<BufferElementTypeSize>(sizeof(glm::dvec4)) },
		{ BufferElementType::BOOL_SCALAR, static_cast<BufferElementTypeSize>(sizeof(bool)) },
		{ BufferElementType::BOOL_VEC2, static_cast<BufferElementTypeSize>(sizeof(glm::bvec2)) },
		{ BufferElementType::BOOL_VEC3, static_cast<BufferElementTypeSize>(sizeof(glm::bvec4)) },
		{ BufferElementType::BOOL_VEC4, static_cast<BufferElementTypeSize>(sizeof(glm::bvec4)) },
		{ BufferElementType::INT_SCALAR, static_cast<BufferElementTypeSize>(sizeof(int32_t)) },
		{ BufferElementType::INT_VEC2, static_cast<BufferElementTypeSize>(sizeof(glm::ivec2)) },
		{ BufferElementType::INT_VEC3, static_cast<BufferElementTypeSize>(sizeof(glm::ivec4)) },
		{ BufferElementType::INT_VEC4, static_cast<BufferElementTypeSize>(sizeof(glm::ivec4)) },
		{ BufferElementType::UINT_SCALAR, static_cast<BufferElementTypeSize>(sizeof(uint32_t)) },
		{ BufferElementType::UINT_VEC2, static_cast<BufferElementTypeSize>(sizeof(glm::uvec2)) },
		{ BufferElementType::UINT_VEC3, static_cast<BufferElementTypeSize>(sizeof(glm::uvec4)) },
		{ BufferElementType::UINT_VEC4, static_cast<BufferElementTypeSize>(sizeof(glm::uvec4)) },
		{ BufferElementType::MAT2, static_cast<BufferElementTypeSize>(sizeof(glm::vec4)) },
		{ BufferElementType::MAT3, static_cast<BufferElementTypeSize>(sizeof(glm::vec4)) },
		{ BufferElementType::MAT4, static_cast<BufferElementTypeSize>(sizeof(glm::vec4)) }
	};

	const std::unordered_map<BufferElementType, BufferElementTypeSize>& selectSizesFromPackingType(BufferPackingType packingType) {
		switch (packingType) {
		case BufferPackingType::PACKED : return typeSizes;
		case BufferPackingType::OPENGL_STD140 : return openglSTD140TypeSizes;
		default: {
			assert(false); //Unknown packing type
			return typeSizes;
		}
		}
	}

	const std::unordered_map<BufferElementType, BufferElementTypeSize>& selectAlignmentsFromPackingType(BufferPackingType packingType) {
		switch (packingType) {
		case BufferPackingType::PACKED: return typeAlignments;
		case BufferPackingType::OPENGL_STD140: return openglSTD140TypeAlignments;
		default: {
			assert(false); //Unknown packing type
			return typeSizes;
		}
		}
	}

	uint64_t align(uint64_t offset, uint64_t alignment) {
		uint64_t spill = offset % alignment;
		if (spill > 0) {
			return offset + alignment - spill;
		}
		return offset;
	}
}

BufferFormat::BufferFormat() : m_size(0), m_arrayType(BufferElementType::UNKNOWN), m_arrayElementFormat(nullptr)
{
}

BufferFormat::BufferFormat(uint64_t arraySize, BufferElementType arrayType, std::shared_ptr<const BufferFormat> arrayElementFormat)
	: m_size(arraySize * (arrayElementFormat == nullptr ? typeSizes.at(arrayType) : arrayElementFormat->size())), m_arrayType(arrayType), m_arrayElementFormat(arrayElementFormat)
{
}

BufferFormat::BufferFormat(std::vector<std::pair<std::string, BufferElementType>>& format, std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> nestedBufferFormats, BufferPackingType packingType)
	: m_nestedBufferFormats(nestedBufferFormats), m_arrayType(BufferElementType::UNKNOWN), m_arrayElementFormat(nullptr), m_packingType(packingType)
{
	uint64_t offset = 0;
	const std::unordered_map<BufferElementType, BufferElementTypeSize>& sizes = selectSizesFromPackingType(packingType);
	const std::unordered_map<BufferElementType, BufferElementTypeSize>& alignments = selectAlignmentsFromPackingType(packingType);

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


const std::pair<BufferOffset, BufferElementType>& BufferFormat::at(std::string name) const
{
	return m_offsets.at(name);
}

std::vector<std::pair<std::string, std::pair<BufferOffset, BufferElementType>>> BufferFormat::orderedOffsets() const
{
	std::vector<std::pair<std::string, std::pair<BufferOffset, BufferElementType>>> orderedOffsets(m_offsets.begin(), m_offsets.end());
	std::sort(orderedOffsets.begin(), orderedOffsets.end(), [](auto& a, auto&b) { return a.second.first < b.second.first; });
	return orderedOffsets;
}

const std::unordered_map<std::string, std::pair<BufferOffset, BufferElementType>>& BufferFormat::offsets() const
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

BufferElementTypeSize BufferFormat::sizeOfType(BufferElementType type, BufferPackingType packingType)
{
	return selectSizesFromPackingType(packingType).at(type);
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
		const std::unordered_map<BufferElementType, BufferElementTypeSize>& sizes = selectSizesFromPackingType(m_packingType);
		return m_size / sizes.at(m_arrayType);
	}
}

std::shared_ptr<const BufferFormat> BufferFormat::arrayElementFormat() const
{
	return m_arrayElementFormat;
}
