#pragma once
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <memory>
namespace Renderer {

	enum struct BufferPackingType {
		PACKED,
		OPENGL_STD140
	};

	enum struct BufferElementType {
		UNKNOWN,
		FLOAT_SCALAR,
		FLOAT_VEC2,
		FLOAT_VEC3,
		FLOAT_VEC4,
		DOUBLE_SCALAR,
		DOUBLE_VEC2,
		DOUBLE_VEC3,
		DOUBLE_VEC4,
		BOOL_SCALAR,
		BOOL_VEC2,
		BOOL_VEC3,
		BOOL_VEC4,
		INT_SCALAR,
		INT_VEC2,
		INT_VEC3,
		INT_VEC4,
		UINT_SCALAR,
		UINT_VEC2,
		UINT_VEC3,
		UINT_VEC4,
		MAT2,
		MAT3,
		MAT4,
		BUFFER,
		ARRAY
	};
	
	class BufferFormat
	{
	private:

		static const std::unordered_map<BufferPackingType, std::unordered_map<BufferElementType, uint64_t>> typeSizes;
		static const std::unordered_map<BufferPackingType, std::unordered_map<BufferElementType, uint64_t>> typeAlignments;

		uint64_t m_size;
		BufferPackingType m_packingType;

		std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> m_nestedBufferFormats;
		std::unordered_map<std::string, std::pair<uint64_t, BufferElementType>> m_offsets;

		BufferElementType m_arrayType;
		std::shared_ptr<const BufferFormat> m_arrayElementFormat;

	public:
		BufferFormat();
		BufferFormat(uint64_t arraySize, BufferElementType arrayElementType, BufferPackingType packingType);
		BufferFormat(uint64_t arraySize, std::shared_ptr<const BufferFormat> arrayElementFormat);
		BufferFormat(std::vector<std::pair<std::string, BufferElementType>>& format, std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> nestedBufferFormats = std::unordered_map<std::string, std::shared_ptr<const BufferFormat>>(), BufferPackingType packingType = BufferPackingType::PACKED);

		const std::pair<uint64_t, BufferElementType>& at(std::string name) const;

		std::vector<std::pair<std::string, std::pair<uint64_t, BufferElementType>>> orderedOffsets() const;

		const std::unordered_map<std::string, std::pair<uint64_t, BufferElementType>>& offsets() const;

		const std::unordered_map<std::string, std::shared_ptr<const BufferFormat>>& nestedFormats() const;
		std::shared_ptr<const BufferFormat> nestedFormat(std::string name) const;

		BufferPackingType packingType() const;
		uint64_t size() const;
		static uint64_t sizeOfType(BufferElementType type, BufferPackingType packingType = BufferPackingType::PACKED);

		BufferElementType arrayType() const;
		uint64_t arraySize() const;
		std::shared_ptr<const BufferFormat> arrayElementFormat() const;
	};
}
