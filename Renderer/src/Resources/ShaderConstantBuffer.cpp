#include "Resources/ShaderConstantBuffer.h"

#include <Buffer/DataBufferArrayView.h>

#include <Resources/RenderResourceManagement.h>

namespace Renderer {

	ShaderConstantBuffer::ShaderConstantBuffer(std::shared_ptr<BufferFormat> format) : m_buffer(DataBuffer(format))
	{
		RenderResourceManagement::createShaderConstantBuffer(m_id, m_buffer);
	}


	ShaderConstantBuffer::~ShaderConstantBuffer()
	{
	}

	void ShaderConstantBuffer::set(const std::string& name, const glm::vec2 & value)
	{
		makeDirty();
		m_buffer.set(name, value);
	}

	void ShaderConstantBuffer::set(const std::string& name, const glm::vec3 & value)
	{
		makeDirty();
		m_buffer.set(name, value);
	}

	void ShaderConstantBuffer::set(const std::string& name, const glm::vec4 & value)
	{
		makeDirty();
		m_buffer.set(name, value);
	}

	void ShaderConstantBuffer::set(const std::string& name, const uint32_t & value)
	{
		makeDirty();
		m_buffer.set(name, value);
	}

	void ShaderConstantBuffer::set(const std::string& name, const int32_t & value)
	{
		makeDirty();
		m_buffer.set(name, value);
	}

	void ShaderConstantBuffer::set(const std::string& name, const bool & value)
	{
		makeDirty();
		m_buffer.set(name, value);
	}

	void ShaderConstantBuffer::set(const std::string& name, const float & value)
	{
		makeDirty();
		m_buffer.set(name, value);
	}

	void ShaderConstantBuffer::set(const std::string& name, const glm::mat3 & value)
	{
		makeDirty();
		m_buffer.set(name, value);
	}

	void ShaderConstantBuffer::set(const std::string& name, const glm::mat4 & value)
	{
		makeDirty();
		m_buffer.set(name, value);
	}

	DataBufferView ShaderConstantBuffer::getBuffer(const std::string& name)
	{
		return m_buffer.getBuffer(name);
	}

	DataBufferArrayView ShaderConstantBuffer::getArray(const std::string& name)
	{
		return m_buffer.getArray(name);
	}

	void ShaderConstantBuffer::set(const std::string& name, const DataBufferView & value)
	{
		makeDirty();
		m_buffer.set(name, value);
	}

	void ShaderConstantBuffer::set(const std::string& name, const DataBufferArrayView & value)
	{
		makeDirty();
		m_buffer.set(name, value);
	}

	const DataBufferView & ShaderConstantBuffer::buffer() const
	{
		return m_buffer;
	}

	void ShaderConstantBuffer::makeDirty()
	{
		m_buffer.markDirty();
	}

	bool ShaderConstantBuffer::isDirty() const
	{
		return m_buffer.isDirty();
	}

	void ShaderConstantBuffer::clean()
	{
		m_buffer.clean();
	}
}
