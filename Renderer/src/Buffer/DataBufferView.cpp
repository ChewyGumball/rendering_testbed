#pragma warning(disable:4996) //MSVC does not like std::copy with bare pointers
#include "Buffer/DataBufferView.h"

#include <Buffer/DataBufferArrayView.h>

#include <glm/gtc/type_ptr.hpp>

using namespace Renderer;

namespace {
	template<typename T>
	void setState(uint8_t* instanceData, std::string name, T* data, BufferElementType type, std::shared_ptr<const BufferFormat> format)
	{
		auto& details = format->at(name);
		assert(details.second == type);

		uint64_t dataSize;
		if (type == BufferElementType::BUFFER || type == BufferElementType::ARRAY) {
			dataSize = format->nestedFormat(name)->size();
		}
		else {
			dataSize = BufferFormat::sizeOfType(type, format->packingType());
		}
		const uint8_t* actualData = reinterpret_cast<const uint8_t*>(data);
		std::copy(actualData, actualData + dataSize, instanceData + details.first);
	}

	template<typename T>
	T& getReferenceToData(uint8_t* data, BufferElementType expectedType, const std::pair<BufferOffset, BufferElementType>& details) {
		assert(details.second == expectedType);

		//As far as I can tell, this is safe for glm types because glm::value_ptr returns a pointer to the first member which is required to be the
		//	address of the whole class by the standard if it is a standard layout class (which I BELIEVE the glm types are)
		return *reinterpret_cast<T*>(data + details.first);
	}
}

DataBufferView::DataBufferView(uint8_t * data, std::shared_ptr<const BufferFormat> format) : data(data), m_format(format)
{
}

DataBufferView::~DataBufferView()
{
}

std::shared_ptr<const BufferFormat> DataBufferView::format() const
{
	return m_format;
}

void DataBufferView::set(std::string name, const glm::vec2& value)
{
	setState(data, name, glm::value_ptr(value), BufferElementType::FLOAT_VEC2, m_format);
}

void DataBufferView::set(std::string name, const glm::vec3& value)
{
	switch (m_format->packingType()) {
	case BufferPackingType::OPENGL_STD140: {
		glm::vec4 wrapped(value, 0);
		setState(data, name, glm::value_ptr(wrapped), BufferElementType::FLOAT_VEC3, m_format);
	}
	default: {
		setState(data, name, glm::value_ptr(value), BufferElementType::FLOAT_VEC3, m_format);
		break;
	}
	}
}

void DataBufferView::set(std::string name, const glm::vec4& value)
{
	setState(data, name, glm::value_ptr(value), BufferElementType::FLOAT_VEC4, m_format);
}

void DataBufferView::set(std::string name, const uint32_t& value)
{
	setState(data, name, &value, BufferElementType::UINT_SCALAR, m_format);
}

void DataBufferView::set(std::string name, const int32_t& value)
{
	setState(data, name, &value, BufferElementType::INT_SCALAR, m_format);
}

void DataBufferView::set(std::string name, const bool& value)
{
	setState(data, name, &value, BufferElementType::BOOL_SCALAR, m_format);
}

void DataBufferView::set(std::string name, const float& value)
{
	setState(data, name, &value, BufferElementType::FLOAT_SCALAR, m_format);
}

void DataBufferView::set(std::string name, const glm::mat3& value)
{
	switch (m_format->packingType()) {
	case BufferPackingType::OPENGL_STD140: {
		glm::mat3x4 wrapped(value);
		setState(data, name, glm::value_ptr(wrapped), BufferElementType::MAT3, m_format);
	}
	default: {
		setState(data, name, glm::value_ptr(value), BufferElementType::MAT3, m_format);
		break;
	}
	}
}

void DataBufferView::set(std::string name, const glm::mat4& value)
{
	setState(data, name, glm::value_ptr(value), BufferElementType::MAT4, m_format);
}

void DataBufferView::set(std::string name, const DataBufferView& value)
{
	setState(data, name, value.begin(), BufferElementType::BUFFER, m_format);
}

void DataBufferView::set(std::string name, const DataBufferArrayView& value)
{
	setState(data, name, value.begin(), BufferElementType::ARRAY, m_format);
}

const glm::vec2& DataBufferView::getVec2(std::string name) const
{
	return getReferenceToData<const glm::vec2>(data, BufferElementType::FLOAT_VEC2, m_format->at(name));
}

const glm::vec3& DataBufferView::getVec3(std::string name) const
{
	return getReferenceToData<const glm::vec3>(data, BufferElementType::FLOAT_VEC3, m_format->at(name));
}

const glm::vec4& DataBufferView::getVec4(std::string name) const
{
	return getReferenceToData<const glm::vec4>(data, BufferElementType::FLOAT_VEC4, m_format->at(name));
}

const uint32_t& DataBufferView::getUInt(std::string name) const
{
	return getReferenceToData<uint32_t>(data, BufferElementType::UINT_SCALAR, m_format->at(name));
}

const int32_t& DataBufferView::getInt(std::string name) const
{
	return getReferenceToData<int32_t>(data, BufferElementType::INT_SCALAR, m_format->at(name));
}

const bool& DataBufferView::getBool(std::string name) const
{
	return getReferenceToData<bool>(data, BufferElementType::BOOL_SCALAR, m_format->at(name));;
}

const float& DataBufferView::getFloat(std::string name) const
{
	return getReferenceToData<float>(data, BufferElementType::FLOAT_SCALAR, m_format->at(name));
}

const glm::mat3& DataBufferView::getMat3(std::string name) const
{
	assert(m_format->packingType() != BufferPackingType::OPENGL_STD140); //std140 stores mat3s as mat3x4 so this doesnt work
	return getReferenceToData<const glm::mat3>(data, BufferElementType::MAT3, m_format->at(name));
}

const glm::mat4& DataBufferView::getMat4(std::string name) const
{
	return getReferenceToData<const glm::mat4>(data, BufferElementType::MAT4, m_format->at(name));
}

const DataBufferView DataBufferView::getBuffer(std::string name) const
{
	return DataBufferView(&getReferenceToData<uint8_t>(data, BufferElementType::BUFFER, m_format->at(name)), m_format->nestedFormat(name));
}

DataBufferView DataBufferView::getBuffer(std::string name)
{
	return DataBufferView(&getReferenceToData<uint8_t>(data, BufferElementType::BUFFER, m_format->at(name)), m_format->nestedFormat(name));
}

const DataBufferArrayView DataBufferView::getArray(std::string name) const
{
	auto format = m_format->nestedFormat(name);
	return DataBufferArrayView(&getReferenceToData<uint8_t>(data, BufferElementType::ARRAY, m_format->at(name)), format->arraySize(), format->arrayType(), format->arrayElementFormat());
}

DataBufferArrayView DataBufferView::getArray(std::string name)
{
	auto format = m_format->nestedFormat(name);
	return DataBufferArrayView(&getReferenceToData<uint8_t>(data, BufferElementType::ARRAY, m_format->at(name)), format->arraySize(), format->arrayType(), format->arrayElementFormat());
}

uint8_t * DataBufferView::begin()
{
	return data;
}

uint8_t * DataBufferView::end()
{
	return data + m_format->size();
}

const uint8_t * DataBufferView::begin() const
{
	return data;
}

const uint8_t * DataBufferView::end() const
{
	return data + m_format->size();
}
