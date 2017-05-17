#include "Renderer/OpenGL/OpenGLShaderConstantBuffer.h"

#include <GL/glew.h>

#include <Renderer/OpenGL/OpenGLShader.h>
#include <Resources/ShaderConstantBuffer.h>
#include <Buffer/BufferFormat.h>

#include <iostream>

using namespace Renderer;
using namespace Renderer::OpenGL;

OpenGLShaderConstantBuffer::OpenGLShaderConstantBuffer(DataBufferView buffer) : m_deviceBufferSize(0), constantBuffer(buffer)
{
	glCreateBuffers(1, &m_handle);
	glNamedBufferData(m_handle, constantBuffer.format()->size(), nullptr, GL_DYNAMIC_DRAW);
}


OpenGLShaderConstantBuffer::~OpenGLShaderConstantBuffer()
{
	glDeleteBuffers(1, &m_handle);
}

void OpenGLShaderConstantBuffer::bindTo(uint32_t bindPoint)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, m_handle);
}

void OpenGLShaderConstantBuffer::uploadIfDirty() const
{
	if (constantBuffer.isDirty()) {
		glNamedBufferSubData(m_handle, 0, constantBuffer.format()->size(), constantBuffer.begin());
	}
}
