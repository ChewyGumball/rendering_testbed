#pragma once
#include "Shaders/Shader.h"

#include <memory>

class OpenGLShader
{
protected:
	std::shared_ptr<const Shader> m_shader;
public:
	OpenGLShader();
	OpenGLShader(std::shared_ptr<const Shader> shader);
	~OpenGLShader();

	void bind();

	void setUniform1i(const std::string uniformName, int data);

	void setUniform2f(const std::string uniformName, glm::vec2 data);
	void setUniform3f(const std::string uniformName, glm::vec3 data);
	void setUniform4f(const std::string uniformName, glm::vec4 data);
	void setUniformMatrix4f(const std::string uniformName, glm::mat4 data);
};

