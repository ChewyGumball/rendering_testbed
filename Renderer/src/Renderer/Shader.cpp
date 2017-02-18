#include "Renderer/Shader.h"

#include <algorithm>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <unordered_set>

#include <glm/gtc/type_ptr.hpp>

#include <lib/SimpleFileWatcher/FileWatcher/FileWatcher.h>

#include "Util/FileUtils.h"

Shader::Shader(std::string vertexFilename, std::string fragmentFilename, BufferFormat expectedInstanceStateFormat, BufferFormat expectedUniformStateFormat, VertexFormat expectedVertexFormat)
	: m_expectedVertexFormat(expectedVertexFormat), m_expectedInstanceStateFormat(expectedInstanceStateFormat), m_expectedUniformStateFormat(expectedUniformStateFormat)
{
	m_filenames[ShaderSourceType::VERTEX].push_back(vertexFilename);
	m_filenames[ShaderSourceType::FRAGMENT].push_back(fragmentFilename);
}

Shader::Shader(std::vector<std::string> vertexFilenames, std::vector<std::string> fragmentFilenames, BufferFormat expectedInstanceStateFormat, BufferFormat expectedUniformStateFormat, VertexFormat expectedVertexFormat)
	: m_expectedVertexFormat(expectedVertexFormat), m_expectedInstanceStateFormat(expectedInstanceStateFormat), m_expectedUniformStateFormat(expectedUniformStateFormat)
{
	m_filenames[ShaderSourceType::VERTEX] = vertexFilenames;
	m_filenames[ShaderSourceType::FRAGMENT] = fragmentFilenames;
}

const std::unordered_map<ShaderSourceType, std::vector<std::string>>& Shader::filenames() const
{
	return m_filenames;
}

const VertexFormat& Shader::expectedVertexFormat() const
{
	return m_expectedVertexFormat;
}

const BufferFormat& Shader::expectedInstanceStateFormat() const
{
	return m_expectedInstanceStateFormat;
}

const BufferFormat & Shader::expectedUniformStateFormat() const
{
	return m_expectedUniformStateFormat;
}

Shader::~Shader() {}
