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
	std::unordered_map<GLuint, std::unordered_map<std::string, GLuint>> uniformBufferLocationCache;

	GLuint uniformLocation(GLuint program, const std::string& uniformName)
	{
		if (uniformLocationCache[program].count(uniformName) == 0) {
			const char* s = uniformName.c_str();
			GLint location = glGetUniformLocation(program, s);
			if (location == -1 && uniformErrors[program].count(uniformName) == 0) {
				uniformErrors[program].insert(uniformName);
				std::cout << "Could not find uniform '" << uniformName << "' in shader!" << std::endl;
			}
			uniformLocationCache[program][uniformName] = location;
		}

		return uniformLocationCache[program][uniformName];
	}

	GLuint uniformBufferLocation(GLuint program, const std::string& uniformBufferName)
	{
		if (uniformBufferLocationCache[program].count(uniformBufferName) == 0) {
			const char* s = uniformBufferName.c_str();
			GLuint location = glGetUniformBlockIndex(program, s);
			if (location == GL_INVALID_INDEX && uniformErrors[program].count(uniformBufferName) == 0) {
				uniformErrors[program].insert(uniformBufferName);
				std::cout << "Could not find uniform buffer '" << uniformBufferName << "' in shader!" << std::endl;
			}
			uniformBufferLocationCache[program][uniformBufferName] = location;
		}

		return uniformBufferLocationCache[program][uniformBufferName];
	}

	std::vector<GLuint> compileShaders(std::vector<std::string>& sources, GLenum type, bool& successful)
	{
		std::vector<GLuint> shaders;
		for(std::string source : sources) {
			GLuint        shader = glCreateShader(type);
			shaders.push_back(shader);
			const GLchar* code = source.c_str();
			glShaderSource(shader, 1, &code, NULL);
			glCompileShader(shader);

			GLint good;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &good);
			if (!good) {
				GLchar log[512];
				glGetShaderInfoLog(shader, 512, NULL, log);
				std::cout << "Shader Compilation Error [" << enumNames[type] << "]: " << log << std::endl;
				successful = false;
			}
		}
		return shaders;
	}

	GLuint createProgram(std::vector<std::string> vertexSources, std::vector<std::string> fragmentSources)
	{
		GLuint program = glCreateProgram();
		bool successful = true;
		std::vector<GLuint> allShaders;

		std::vector<GLuint> vertexShaders = compileShaders(vertexSources, GL_VERTEX_SHADER, successful);
		allShaders.insert(allShaders.end(), vertexShaders.begin(), vertexShaders.end());

		if (successful) {
			std::vector<GLuint> fragmentShaders = compileShaders(fragmentSources, GL_FRAGMENT_SHADER, successful);
			allShaders.insert(allShaders.end(), fragmentShaders.begin(), fragmentShaders.end());
		}

		if (successful) {
			for (GLuint shader : allShaders) {
				glAttachShader(program, shader);
			}
			glLinkProgram(program);
			for (GLuint shader : allShaders) {
				glDetachShader(program, shader);
			}

			GLint good;
			glGetProgramiv(program, GL_LINK_STATUS, &good);
			if (!good) {
				GLchar log[512];
				glGetProgramInfoLog(program, 512, NULL, log);
				std::cout << "Program Link Error: " << log << std::endl;

				glDeleteProgram(program);
				program = 0;
			}
		}
		for (GLuint shader : allShaders) {
			glDeleteShader(shader);
		}

		return program;
	}

	GLuint createProgram(std::string& vertexSource, std::string& fragmentSource) { 
		return createProgram(std::vector<std::string>{ vertexSource }, std::vector<std::string>{ fragmentSource }); 
	}

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

OpenGLShader::OpenGLShader() : programHandle(0), m_shader(nullptr)
{
}

OpenGLShader::OpenGLShader(std::shared_ptr<const Shader> shader) : programHandle(createProgramFromFiles(shader->filenames())), m_shader(shader)
{ }

OpenGLShader::~OpenGLShader()
{
	glDeleteProgram(programHandle);
}

const std::shared_ptr<const Shader> OpenGLShader::shader() const
{
	return m_shader;
}

void OpenGLShader::bind()
{
	glUseProgram(programHandle);
}

void OpenGLShader::setUniform1i(const std::string uniformName, const GLint& data) const
{
	GLint location = uniformLocation(programHandle, uniformName);
	if (location != -1) {
		glUniform1iv(location, 1, &data);
	}
}

void OpenGLShader::setUniform1f(const std::string uniformName, const GLfloat& data) const
{
	GLint location = uniformLocation(programHandle, uniformName);
	if (location != -1) {
		glUniform1fv(location, 1, &data);
	}
}

void OpenGLShader::setUniform2f(const std::string uniformName, const glm::vec2& data) const
{
	GLint location = uniformLocation(programHandle, uniformName);
	if (location != -1) {
		glUniform2fv(location, 1, glm::value_ptr(data));
	}
}

void OpenGLShader::setUniform3f(const std::string uniformName, const glm::vec3& data) const
{
	GLint location = uniformLocation(programHandle, uniformName);
	if (location != -1) {
		glUniform3fv(location, 1, glm::value_ptr(data));
	}
}

void OpenGLShader::setUniform4f(const std::string uniformName, const glm::vec4& data) const
{
	GLint location = uniformLocation(programHandle, uniformName);
	if (location != -1) {
		glUniform4fv(location, 1, glm::value_ptr(data));
	}
}

void OpenGLShader::setUniformMatrix4f(const std::string  uniformName, const glm::mat4& data) const
{
	GLint location = uniformLocation(programHandle, uniformName);
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
	}
}

GLuint OpenGLShader::program() const
{
	return programHandle;
}

GLint OpenGLShader::getAttributeLocation(const std::string name) const
{
	return glGetAttribLocation(programHandle, name.data());
}

void OpenGLShader::bindUniformBufferToBindPoint(const std::string name, GLuint bindPoint)
{
	GLuint location = uniformBufferLocation(programHandle, name);
	if (location != GL_INVALID_INDEX) {
		auto currentBindPoint = boundUniformBufferBindPoints.find(name);
		if (currentBindPoint == boundUniformBufferBindPoints.end() || currentBindPoint->second != bindPoint) {
			glUniformBlockBinding(programHandle, location, bindPoint);
			GLint minSize;
			glGetActiveUniformBlockiv(programHandle, location, GL_UNIFORM_BLOCK_DATA_SIZE, &minSize);
			boundUniformBufferBindPoints[name] = bindPoint;
		}
	}
}
