#pragma once
#include "Renderer/Shader.h"

#include <memory>

class OpenGLShader
{
private:
	GLuint programHandle; 
	std::shared_ptr<const Shader> m_shader;

public:
	OpenGLShader();
	OpenGLShader(std::shared_ptr<const Shader> shader);
	~OpenGLShader();

	const std::shared_ptr<const Shader> shader() const;

	void bind();

	void setUniform1i(const std::string uniformName, const GLint& data) const;

	void setUniform1f(const std::string uniformName, const GLfloat& data) const;
	void setUniform2f(const std::string uniformName, const glm::vec2& data) const;
	void setUniform3f(const std::string uniformName, const glm::vec3& data) const;
	void setUniform4f(const std::string uniformName, const glm::vec4& data) const;
	void setUniformMatrix4f(const std::string uniformName, const glm::mat4& data) const;
	
	GLint getAttributeLocation(const std::string name) const;
};

