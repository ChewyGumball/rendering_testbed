#pragma once
#include <string>
#include <unordered_set>

#include <GL\glew.h>
#include <glm\glm.hpp>

class Shader
{

protected:
	mutable std::unordered_set<std::string> uniformErrors;
	std::size_t m_id;
	mutable GLuint programHandle;
	std::string vertexFilename;
	std::string fragmentFilename;

	mutable bool needsToReload;

	void reloadFromFiles();

public:
	Shader(std::string& vertexSource, std::string& fragmentSource);
	Shader(std::string vertexFilename, std::string fragmentFilename, bool monitorFiles);
	~Shader();

	void setUniform1i(const std::string & uniformName, int data) const;

	void setUniform2f(const std::string & uniformName, glm::vec2 data) const;
	void setUniform3f(const std::string & uniformName, glm::vec3 data) const;
	void setUniform4f(const std::string & uniformName, glm::vec4 data) const;
	void setUniformMatrix4f(const std::string& uniformName, glm::mat4 data) const;

	void bind() const;

	std::size_t id() const;

};

