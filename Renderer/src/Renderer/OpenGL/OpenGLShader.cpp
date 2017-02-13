#include "Renderer/OpenGL/OpenGLShader.h"

#include <unordered_set>
#include <iostream>
#include <Util/FileUtils.h>
#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>

namespace {
	std::unordered_map<GLenum, std::string>                     enumNames = { { GL_VERTEX_SHADER, "VERTEX" },{ GL_FRAGMENT_SHADER, "FRAGMENT" } };
	std::unordered_map<GLuint, std::unordered_set<std::string>> uniformErrors;
	std::unordered_map<GLuint, std::unordered_map<std::string, GLuint>> uniformLocationCache;

	GLuint uniformLocation(GLuint program, const std::string& uniformName)
	{
		if (uniformLocationCache[program].count(uniformName) == 0) {
			GLint location = glGetUniformLocation(program, uniformName.c_str());
			if (location == -1 && uniformErrors[program].count(uniformName) == 0) {
				uniformErrors[program].insert(uniformName);
				std::cout << "Could not find uniform '" << uniformName << "' in shader!" << std::endl;
			}
			uniformLocationCache[program][uniformName] = location;
		}

		return uniformLocationCache[program][uniformName];
	}

	GLuint compileShader(std::string& source, GLenum type)
	{
		GLuint        shader = glCreateShader(type);
		const GLchar* code = source.c_str();
		glShaderSource(shader, 1, &code, NULL);
		glCompileShader(shader);

		GLint successful;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &successful);
		if (!successful) {
			GLchar log[512];
			glGetShaderInfoLog(shader, 512, NULL, log);
			std::cout << "Shader Compilation Error [" << enumNames[type] << "]: " << log << std::endl;

			return 0;
		}

		return shader;
	}

	GLuint createProgram(std::vector<std::string> vertexSources, std::vector<std::string> fragmentSources)
	{
		GLuint program = glCreateProgram();
		for (std::string source : vertexSources) {
			GLuint vertexProgram = compileShader(source, GL_VERTEX_SHADER);
			if (vertexProgram != 0) {
				glAttachShader(program, vertexProgram);
			}
			else {
				glDeleteShader(vertexProgram);
				return 0;
			}
		}
		for (std::string source : fragmentSources) {
			GLuint fragmentProgram = compileShader(source, GL_FRAGMENT_SHADER);
			if (fragmentProgram != 0) {
				glAttachShader(program, fragmentProgram);
			}
			else {
				glDeleteShader(fragmentProgram);
				return 0;
			}
		}

		glLinkProgram(program);
		GLint successful;
		glGetProgramiv(program, GL_LINK_STATUS, &successful);
		if (!successful) {
			GLchar log[512];
			glGetProgramInfoLog(program, 512, NULL, log);
			std::cout << "Program Link Error: " << log << std::endl;

			return 0;
		}
		return successful ? program : 0;
	}

	GLuint createProgram(std::string& vertexSource, std::string& fragmentSource) { return createProgram(std::vector<std::string>{ vertexSource }, std::vector<std::string>{ fragmentSource }); }

	GLuint createProgramFromFiles(const std::unordered_map<ShaderSourceType, std::vector<std::string>>& filenames)
	{
		std::vector<std::string> vertexSources;
		std::vector<std::string> fragmentSources;

		for (auto& source : filenames)
		{
			if (source.first == ShaderSourceType::VERTEX) {
				for (const std::string& file : source.second) {
					vertexSources.push_back(Util::File::ReadWholeFile(file));
				}
			}
			if (source.first == ShaderSourceType::FRAGMENT) {
				for (const std::string& file : source.second) {
					fragmentSources.push_back(Util::File::ReadWholeFile(file));
				}
			}
		}

		return createProgram(vertexSources, fragmentSources);
	}
}

OpenGLShader::OpenGLShader()
{
}

OpenGLShader::OpenGLShader(std::shared_ptr<const Shader> shader) : programHandle(createProgramFromFiles(shader->filenames()))
{}

OpenGLShader::~OpenGLShader()
{
}

void OpenGLShader::bind()
{
	glUseProgram(programHandle);
}

void OpenGLShader::setUniform1i(const std::string uniformName, int data) const
{
	GLint location = uniformLocation(programHandle, uniformName);
	if (location != -1) {
		glUniform1i(location, data);
	}
}

void OpenGLShader::setUniform1f(const std::string uniformName, float data) const
{
	GLint location = uniformLocation(programHandle, uniformName);
	if (location != -1) {
		glUniform1f(location, data);
	}
}

void OpenGLShader::setUniform2f(const std::string uniformName, glm::vec2 data) const
{
	GLint location = uniformLocation(programHandle, uniformName);
	if (location != -1) {
		glUniform2fv(location, 1, glm::value_ptr(data));
	}
}

void OpenGLShader::setUniform3f(const std::string uniformName, glm::vec3 data) const
{
	GLint location = uniformLocation(programHandle, uniformName);
	if (location != -1) {
		glUniform3fv(location, 1, glm::value_ptr(data));
	}
}

void OpenGLShader::setUniform4f(const std::string uniformName, glm::vec4 data) const
{
	GLint location = uniformLocation(programHandle, uniformName);
	if (location != -1) {
		glUniform4fv(location, 1, glm::value_ptr(data));
	}
}

void OpenGLShader::setUniformMatrix4f(const std::string  uniformName, glm::mat4 data) const
{
	GLint location = uniformLocation(programHandle, uniformName);
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
	}
}
