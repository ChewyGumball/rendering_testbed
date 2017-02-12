#pragma once
#include "Shaders/Shader.h"

#include <memory>

class OpenGLShader
{
private:
	GLuint programHandle;
public:
	OpenGLShader();
	OpenGLShader(std::shared_ptr<const Shader> shader);
	~OpenGLShader();

	void bind();

	void setUniform1i(const std::string uniformName, int data) const;

	void setUniform1f(const std::string uniformName, float data) const;
	void setUniform2f(const std::string uniformName, glm::vec2 data) const;
	void setUniform3f(const std::string uniformName, glm::vec3 data) const;
	void setUniform4f(const std::string uniformName, glm::vec4 data) const;
	void setUniformMatrix4f(const std::string uniformName, glm::mat4 data) const;
};

