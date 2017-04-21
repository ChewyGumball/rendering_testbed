#include "Resources/ShaderConstantBuffer.h"



ShaderConstantBuffer::ShaderConstantBuffer(std::string name, std::shared_ptr<BufferFormat> format) : m_name(name), m_buffer(format)
{
	assert(format->packingType() == BufferPackingType::OPENGL_STD140);
}


ShaderConstantBuffer::~ShaderConstantBuffer()
{
}

void ShaderConstantBuffer::set(std::string name, const glm::vec2 & value)
{
	m_dirty = true;
	m_buffer.set(name, value);
}

void ShaderConstantBuffer::set(std::string name, const glm::vec3 & value)
{
	m_dirty = true;
	m_buffer.set(name, value);
}

void ShaderConstantBuffer::set(std::string name, const glm::vec4 & value)
{
	m_dirty = true;
	m_buffer.set(name, value);
}

void ShaderConstantBuffer::set(std::string name, const uint32_t & value)
{
	m_dirty = true;
	m_buffer.set(name, value);
}

void ShaderConstantBuffer::set(std::string name, const int32_t & value)
{
	m_dirty = true;
	m_buffer.set(name, value);
}

void ShaderConstantBuffer::set(std::string name, const bool & value)
{
	m_dirty = true;
	m_buffer.set(name, value);
}

void ShaderConstantBuffer::set(std::string name, const float & value)
{
	m_dirty = true;
	m_buffer.set(name, value);
}

void ShaderConstantBuffer::set(std::string name, const glm::mat3 & value)
{
	m_dirty = true;
	m_buffer.set(name, value);
}

void ShaderConstantBuffer::set(std::string name, const glm::mat4 & value)
{
	m_dirty = true;
	m_buffer.set(name, value);
}

DataBufferView ShaderConstantBuffer::getBuffer(std::string name)
{
	return m_buffer.getBuffer(name);
}

DataBufferArrayView ShaderConstantBuffer::getArray(std::string name)
{
	return m_buffer.getArray(name);
}

void ShaderConstantBuffer::set(std::string name, const DataBufferView & value)
{
	m_dirty = true;
	m_buffer.set(name, value);
}

void ShaderConstantBuffer::set(std::string name, const DataBufferArrayView & value)
{
	m_dirty = true;
	m_buffer.set(name, value);
}

const std::string & ShaderConstantBuffer::name() const
{
	return m_name;
}

const DataBufferView & ShaderConstantBuffer::buffer() const
{
	return m_buffer;
}

void ShaderConstantBuffer::addUpdater(std::string tag, std::function<void(ShaderConstantBuffer&)> updater)
{
	updateFunctions[tag] = updater;
}

void ShaderConstantBuffer::removeUpdated(std::string tag)
{
	updateFunctions.erase(tag);
}

void ShaderConstantBuffer::makeDirty()
{
	m_dirty = true;
}

bool ShaderConstantBuffer::isDirty() const
{
	return m_dirty;
}

void ShaderConstantBuffer::clean()
{
	m_dirty = false;
}