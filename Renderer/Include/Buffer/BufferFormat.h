#pragma once
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <memory>

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

//TODO: I changed my mind, I don't like these typedefs
typedef uint64_t BufferElementTypeSize;
typedef uint64_t BufferOffet;

class BufferFormat
{
private:
	uint64_t m_size;
	std::unordered_map<std::string, const std::shared_ptr<BufferFormat>> m_nestedBufferFormats;
	std::unordered_map<std::string, std::pair<BufferOffet, BufferElementType>> m_offsets;
	BufferElementType m_arrayType;
	const std::shared_ptr<BufferFormat> m_arrayElementFormat;

public:
	BufferFormat();
	BufferFormat(uint64_t arraySize, BufferElementType arrayType, const std::shared_ptr<BufferFormat> arrayElementFormat = nullptr);
	BufferFormat(std::vector<std::pair<std::string, BufferElementType>>& format, std::unordered_map<std::string, const std::shared_ptr<BufferFormat>> nestedBufferFormats = std::unordered_map<std::string, const std::shared_ptr<BufferFormat>>());

	const std::pair<BufferOffet, BufferElementType>& at(std::string name) const;

	const std::unordered_map<std::string, std::pair<BufferOffet, BufferElementType>>& offsets() const;

	const std::shared_ptr<BufferFormat> nestedFormat(std::string name) const;
	
	uint64_t size() const;
	static BufferElementTypeSize sizeOfType(BufferElementType type);

	BufferElementType arrayType() const;
	uint64_t arraySize() const;
	const std::shared_ptr<BufferFormat> arrayElementFormat() const;
};

