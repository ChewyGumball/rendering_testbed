#include "Renderer/OpenGL/OpenGLShaderConstantBuffer.h"

#include <Renderer/OpenGL/OpenGLShader.h>
#include <Resources/ShaderConstantBuffer.h>

#include <iostream>

using namespace Renderer;
using namespace Renderer::OpenGL;

OpenGLShaderConstantBuffer::OpenGLShaderConstantBuffer(): m_handle(0), m_deviceBufferSize(0), constantBuffer(nullptr)
{
}

OpenGLShaderConstantBuffer::OpenGLShaderConstantBuffer(std::shared_ptr<const ShaderConstantBuffer> shaderConstantBuffer) : m_deviceBufferSize(0), constantBuffer(shaderConstantBuffer)
{
	glCreateBuffers(1, &m_handle);
	glNamedBufferData(m_handle, constantBuffer->buffer().format()->size(), nullptr, GL_DYNAMIC_DRAW);
}


OpenGLShaderConstantBuffer::~OpenGLShaderConstantBuffer()
{
	glDeleteBuffers(1, &m_handle);
}

void OpenGLShaderConstantBuffer::bindTo(GLuint bindPoint)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, m_handle);
}

void OpenGLShaderConstantBuffer::uploadIfDirty() const
{
	if (constantBuffer->isDirty()) {
		auto& buffer = constantBuffer->buffer();
		glNamedBufferSubData(m_handle, 0, buffer.format()->size(), buffer.begin());
	}
}
