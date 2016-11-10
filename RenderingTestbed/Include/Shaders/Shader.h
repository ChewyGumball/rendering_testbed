#pragma once
#include <string>

#include <GL\glew.h>
#include <glm\glm.hpp>

class Shader
{

protected:
	std::size_t m_id;
	GLuint programHandle;
public:
	Shader(std::string& vertexSource, std::string& fragmentSource);
	Shader(std::string& vertexFilename, std::string& fragmentFilename, bool monitorFiles);
	~Shader();
	void setUniform2f(const std::string & uniformName, glm::vec2 data) const;
	void setUniform3f(const std::string & uniformName, glm::vec3 data) const;
	void setUniform4f(const std::string & uniformName, glm::vec4 data) const;
	void setUniformMatrix4f(const std::string& uniformName, glm::mat4 data) const;

	void bind() const;

	std::size_t id() const;
};

