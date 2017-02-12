#pragma once
#include <string>
#include <unordered_map>
#include <Renderer\RenderResource.h>

#include <GL\glew.h>
#include <glm\glm.hpp>

enum struct ShaderSourceType {
	VERTEX,
	FRAGMENT
};


class Shader : public RenderResource
{
private:
	std::unordered_map<ShaderSourceType, std::vector<std::string>> m_filenames;

	mutable bool needsToReload;

	void reloadFromFiles();

public:
	Shader(std::string vertexFilename, std::string fragmentFilename);
	Shader(std::vector<std::string> vertexFilenames, std::vector<std::string> fragmentFilenames);

	const std::unordered_map<ShaderSourceType, std::vector<std::string>>& filenames() const;

	~Shader();
};

