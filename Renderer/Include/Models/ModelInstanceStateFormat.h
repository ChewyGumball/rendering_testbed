#pragma once
#include <stdint.h>
#include <string>
#include <unordered_map>

enum struct ModelInstanceStateType {
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

class ModelInstanceStateFormat
{
private:
	uint8_t m_size;
	std::unordered_map<std::string, std::pair<uint8_t, ModelInstanceStateType>> m_offsets;

public:
	ModelInstanceStateFormat();
	ModelInstanceStateFormat(std::vector<std::pair<std::string, ModelInstanceStateType>>& format);

	void setState(std::vector<uint8_t>& instanceData, std::string name, ModelInstanceStateType type, uint8_t* data) const;
	const std::pair<uint8_t, ModelInstanceStateType>& operator[](std::string name) const;

	const std::unordered_map<std::string, std::pair<uint8_t, ModelInstanceStateType>>& offsets() const;

	std::vector<uint8_t> initialData() const;

	uint8_t size() const;
	static uint8_t sizeOfType(ModelInstanceStateType type);
};

