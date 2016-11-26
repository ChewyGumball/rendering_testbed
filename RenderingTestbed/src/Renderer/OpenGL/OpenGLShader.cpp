#include "Renderer/OpenGL/OpenGLShader.h"



OpenGLShader::OpenGLShader()
{
}

OpenGLShader::OpenGLShader(std::shared_ptr<const Shader> shader): m_shader(shader)
{
}

OpenGLShader::~OpenGLShader()
{
}

void OpenGLShader::bind()
{
	m_shader->bind();
}

void OpenGLShader::setUniform1i(const std::string uniformName, int data) const
{
	m_shader->setUniform1i(uniformName, data);
}

void OpenGLShader::setUniform2f(const std::string uniformName, glm::vec2 data) const
{
	m_shader->setUniform2f(uniformName, data);
}

void OpenGLShader::setUniform3f(const std::string uniformName, glm::vec3 data) const
{
	m_shader->setUniform3f(uniformName, data);
}

void OpenGLShader::setUniform4f(const std::string uniformName, glm::vec4 data) const
{
	m_shader->setUniform4f(uniformName, data);
}

void OpenGLShader::setUniformMatrix4f(const std::string  uniformName, glm::mat4 data) const
{
	m_shader->setUniformMatrix4f(uniformName, data);
}
