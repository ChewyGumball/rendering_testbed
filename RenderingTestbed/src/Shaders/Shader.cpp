#include "Shaders/Shader.h"

#include <iostream>
#include <unordered_map>

#include <glm/gtc/type_ptr.hpp>

#include "Util/FileUtils.h"

namespace {

	std::unordered_map<GLenum, std::string> enumNames = {
		{GL_VERTEX_SHADER, "VERTEX"},
		{GL_FRAGMENT_SHADER, "FRAGMENT"}
	};

	GLuint compileShader(std::string& source, GLenum type)
	{
		GLuint shader = glCreateShader(type);
		const GLchar* code = source.c_str();
		glShaderSource(shader, 1, &code, NULL);
		glCompileShader(shader);

		GLint successful;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &successful);
		if (!successful)
		{
			GLchar log[512];
			glGetShaderInfoLog(shader, 512, NULL, log);
			std::cout << "Shader Compilation Error [" << enumNames[type] << "]: " << log << std::endl;

			return 0;
		}

		return shader;
	}

	GLuint createProgram(std::string& vertexSource, std::string& fragmentSource)
	{
		GLuint vertexProgram = compileShader(vertexSource, GL_VERTEX_SHADER);
		GLuint fragmentProgram = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

		if (vertexProgram != 0 && fragmentProgram != 0)
		{
			GLuint program = glCreateProgram();
			glAttachShader(program, vertexProgram);
			glAttachShader(program, fragmentProgram);
			glLinkProgram(program);

			GLint successful;
			glGetProgramiv(program, GL_LINK_STATUS, &successful);
			if (successful)
			{
				return program;
			}

			GLchar log[512];
			glGetProgramInfoLog(program, 512, NULL, log);
			std::cout << "Program Compilation Error: " << log << std::endl;
		}

		if (vertexProgram != 0)
		{
			glDeleteShader(vertexProgram);
		}

		if (fragmentProgram != 0)
		{
			glDeleteShader(fragmentProgram);
		}

		return 0;
	}
}

Shader::Shader(std::string& vertexSource, std::string& fragmentSource) : programHandle(createProgram(vertexSource, fragmentSource))
{
}

Shader::Shader(std::string& vertexFilename, std::string& fragmentFilename, bool monitorFiles) : Shader(Util::File::ReadWholeFile(vertexFilename), Util::File::ReadWholeFile(fragmentFilename))
{
}


Shader::~Shader()
{
}


void Shader::setUniform2f(const std::string &uniformName, glm::vec2 data) const
{
	GLint location = glGetUniformLocation(programHandle, uniformName.c_str());
	if (location == -1)
	{
		std::cout << "Could not find uniform '" << uniformName << "' in shader!" << std::endl;
		return;
	}
	glUniform2fv(location, 1, glm::value_ptr(data));
}

void Shader::setUniform3f(const std::string &uniformName, glm::vec3 data) const
{
	GLint location = glGetUniformLocation(programHandle, uniformName.c_str());
	if (location == -1)
	{
		std::cout << "Could not find uniform '" << uniformName << "' in shader!" << std::endl;
		return;
	}
	glUniform3fv(location, 1, glm::value_ptr(data));
}

void Shader::setUniform4f(const std::string &uniformName, glm::vec4 data) const
{
	GLint location = glGetUniformLocation(programHandle, uniformName.c_str());
	if (location == -1)
	{
		std::cout << "Could not find uniform '" << uniformName << "' in shader!" << std::endl;
		return;
	}
	glUniform4fv(location, 1, glm::value_ptr(data));
}

void Shader::setUniformMatrix4f(const std::string & uniformName, glm::mat4 data) const
{
	GLint location = glGetUniformLocation(programHandle, uniformName.c_str());
	if (location == -1)
	{
		std::cout << "Could not find uniform '" << uniformName << "' in shader!" << std::endl;
		return;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
}

void Shader::bind() const
{
	glUseProgram(programHandle);
}

std::size_t Shader::id() const
{
	return m_id;
}
