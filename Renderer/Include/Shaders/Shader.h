#pragma once
#include <string>
#include <unordered_set>
#include <Renderer\RenderResource.h>

#include <GL\glew.h>
#include <glm\glm.hpp>

class Shader : public RenderResource
{
protected:
	mutable GLuint programHandle;
	std::vector<std::string> vertexFilenames;
	std::vector<std::string> fragmentFilenames;

	mutable bool needsToReload;

	void reloadFromFiles();

public:
	Shader(std::string& vertexSource, std::string& fragmentSource);
	Shader(std::string vertexFilename, std::string fragmentFilename, bool monitorFiles);
	Shader(std::vector<std::string> vertexFilenames, std::vector<std::string> fragmentFilenames, bool monitorFiles);
	~Shader();

	void setUniform1i(const std::string & uniformName, int data) const;

	void setUniform1f(const std::string & uniformName, float data) const;
	void setUniform2f(const std::string & uniformName, glm::vec2 data) const;
	void setUniform3f(const std::string & uniformName, glm::vec3 data) const;
	void setUniform4f(const std::string & uniformName, glm::vec4 data) const;
	void setUniformMatrix4f(const std::string& uniformName, glm::mat4 data) const;

	void bind() const;
};

