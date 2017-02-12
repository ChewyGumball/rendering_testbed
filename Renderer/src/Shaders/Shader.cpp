#include "Shaders/Shader.h"

#include <algorithm>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <unordered_set>

#include <glm/gtc/type_ptr.hpp>

#include <lib/SimpleFileWatcher/FileWatcher/FileWatcher.h>

#include "Util/FileUtils.h"

Shader::Shader(std::string vertexFilename, std::string fragmentFilename)
{
	m_filenames[ShaderSourceType::VERTEX].push_back(vertexFilename);
	m_filenames[ShaderSourceType::FRAGMENT].push_back(fragmentFilename);
}

Shader::Shader(std::vector<std::string> vertexFilenames, std::vector<std::string> fragmentFilenames)
{
	m_filenames[ShaderSourceType::VERTEX] = vertexFilenames;
	m_filenames[ShaderSourceType::FRAGMENT] = fragmentFilenames;
}

const std::unordered_map<ShaderSourceType, std::vector<std::string>>& Shader::filenames() const
{
	return m_filenames;
}

Shader::~Shader() {}

void Shader::reloadFromFiles() { needsToReload = true; }
