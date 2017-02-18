#pragma once
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <Renderer/DataBuffer.h>

enum struct BufferElementType {
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
	MAT4
};

typedef uint8_t BufferElementTypeSize;

class BufferFormat
{
private:
	uint16_t m_size;
	std::unordered_map<std::string, std::pair<uint16_t, BufferElementType>> m_offsets;

public:
	BufferFormat();
	BufferFormat(std::vector<std::pair<std::string, BufferElementType>>& format);

	void setState(DataBuffer& instanceData, std::string name, BufferElementType type, uint8_t* data) const;
	const std::pair<uint16_t, BufferElementType>& operator[](std::string name) const;

	const std::unordered_map<std::string, std::pair<uint16_t, BufferElementType>>& offsets() const;

	DataBuffer initialData() const;

	uint16_t size() const;
	static BufferElementTypeSize sizeOfType(BufferElementType type);
};

