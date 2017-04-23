#pragma warning(disable:4996) //MSVC does not like std::copy with bare pointers
#include "Buffer/DataBufferArrayView.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Renderer;

namespace {
	template<typename T>
	void setState(uint8_t* instanceData, T* data, BufferElementType requestedType, BufferElementType arrayType, uint64_t index)	{
		assert(requestedType == arrayType);

		uint64_t dataSize = BufferFormat::sizeOfType(arrayType);
		const uint8_t* actualData = reinterpret_cast<const uint8_t*>(data);

		std::copy(actualData, actualData + dataSize, instanceData + index * dataSize);
	}

	template<typename T>
	T* getPointer(uint8_t* arrayData, BufferElementType requestedType, BufferElementType arrayType, uint64_t index) {
		assert(arrayType == requestedType);

		return reinterpret_cast<T*>(arrayData + index * BufferFormat::sizeOfType(arrayType));
	}
}

DataBufferArrayView::DataBufferArrayView(uint8_t* data, uint64_t elementCount, BufferElementType elementType, std::shared_ptr<const BufferFormat> elementFormat)
	:data(data), m_type(elementType), m_elementFormat(elementFormat), m_count(elementCount)
{
}


DataBufferArrayView::~DataBufferArrayView()
{
}

BufferElementType DataBufferArrayView::elementType() const
{
	return m_type;
}

uint64_t DataBufferArrayView::elementCount() const
{
	return m_count;
}


void DataBufferArrayView::setAt(uint64_t index, const glm::vec2 & value)
{
	setState(data, glm::value_ptr(value), BufferElementType::FLOAT_VEC2, m_type, index);
}

void DataBufferArrayView::setAt(uint64_t index, const glm::vec3 & value)
{
	setState(data, glm::value_ptr(value), BufferElementType::FLOAT_VEC3, m_type, index);
}

void DataBufferArrayView::setAt(uint64_t index, const glm::vec4 & value)
{
	setState(data, glm::value_ptr(value), BufferElementType::FLOAT_VEC4, m_type, index);
}

void DataBufferArrayView::setAt(uint64_t index, const uint32_t & value)
{
	setState(data, &value, BufferElementType::UINT_SCALAR, m_type, index);
}

void DataBufferArrayView::setAt(uint64_t index, const int32_t & value)
{
	setState(data, &value, BufferElementType::INT_SCALAR, m_type, index);
}

void DataBufferArrayView::setAt(uint64_t index, const bool & value)
{
	setState(data, &value, BufferElementType::BOOL_SCALAR, m_type, index);
}

void DataBufferArrayView::setAt(uint64_t index, const float & value)
{
	setState(data, &value, BufferElementType::FLOAT_SCALAR, m_type, index);
}

void DataBufferArrayView::setAt(uint64_t index, const glm::mat3 & value)
{
	setState(data, glm::value_ptr(value), BufferElementType::MAT3, m_type, index);
}

void DataBufferArrayView::setAt(uint64_t index, const glm::mat4 & value)
{
	setState(data, glm::value_ptr(value), BufferElementType::MAT4, m_type, index);
}

void DataBufferArrayView::setAt(uint64_t index, const DataBufferView & value)
{
	assert(m_type == BufferElementType::BUFFER);
	std::copy(value.begin(), value.end(), data + index * m_elementFormat->size());
}

void DataBufferArrayView::setAt(uint64_t index, const DataBufferArrayView & value)
{
}

const glm::vec2 & DataBufferArrayView::getVec2At(uint64_t index) const
{
	return *getPointer<glm::vec2>(data, BufferElementType::FLOAT_VEC2, m_type, index);
}

const glm::vec3 & DataBufferArrayView::getVec3At(uint64_t index) const
{
	return *getPointer<glm::vec3>(data, BufferElementType::FLOAT_VEC3, m_type, index);
}

const glm::vec4 & DataBufferArrayView::getVec4At(uint64_t index) const
{
	return *getPointer<glm::vec4>(data, BufferElementType::FLOAT_VEC4, m_type, index);
}

const uint32_t & DataBufferArrayView::getUIntAt(uint64_t index) const
{
	return *getPointer<uint32_t>(data, BufferElementType::UINT_SCALAR, m_type, index);
}

const int32_t & DataBufferArrayView::getIntAt(uint64_t index) const
{
	return *getPointer<int32_t>(data, BufferElementType::INT_SCALAR, m_type, index);
}

const bool & DataBufferArrayView::getBoolAt(uint64_t index) const
{
	return *getPointer<bool>(data, BufferElementType::BOOL_SCALAR, m_type, index);
}

const float & DataBufferArrayView::getFloatAt(uint64_t index) const
{
	return *getPointer<float>(data, BufferElementType::FLOAT_SCALAR, m_type, index);
}

const glm::mat3 & DataBufferArrayView::getMat3At(uint64_t index) const
{
	return *getPointer<glm::mat3>(data, BufferElementType::MAT3, m_type, index);
}

const glm::mat4 & DataBufferArrayView::getMat4At(uint64_t index) const
{
	return *getPointer<glm::mat4>(data, BufferElementType::MAT4, m_type, index);
}


const DataBufferView DataBufferArrayView::getBufferAt(uint64_t index) const
{
	assert(m_type == BufferElementType::BUFFER);
	return DataBufferView(data + index * m_elementFormat->size(), m_elementFormat);
}

DataBufferView DataBufferArrayView::getBufferAt(uint64_t index)
{
	assert(m_type == BufferElementType::BUFFER);
	return DataBufferView(data + index * m_elementFormat->size(), m_elementFormat);
}

const DataBufferArrayView DataBufferArrayView::getArrayAt(uint64_t index) const
{
	assert(m_type == BufferElementType::ARRAY);
	return DataBufferArrayView(data + index * m_elementFormat->size(), m_elementFormat->arraySize(), m_elementFormat->arrayType(), m_elementFormat->arrayElementFormat());
}

DataBufferArrayView DataBufferArrayView::getArrayAt(uint64_t index)
{
	assert(m_type == BufferElementType::ARRAY);
	return DataBufferArrayView(data + index * m_elementFormat->size(), m_elementFormat->arraySize(), m_elementFormat->arrayType(), m_elementFormat->arrayElementFormat());
}

uint8_t * DataBufferArrayView::begin()
{
	return data;
}

uint8_t * DataBufferArrayView::end()
{
	return data + m_count * m_elementFormat->size();
}

const uint8_t * DataBufferArrayView::begin() const
{
	return data;
}

const uint8_t * DataBufferArrayView::end() const
{
	return data + m_count * m_elementFormat->size();
}
