#pragma once
#include <string>
#include <unordered_map>
#include <Renderer\RenderResource.h>
#include <Models/ModelInstanceStateFormat.h>
#include <Models/Vertex.h>

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
	VertexFormat m_expectedVertexFormat;
	ModelInstanceStateFormat m_expectedInstanceStateFormat;

public:
	Shader(std::string vertexFilename, std::string fragmentFilename, ModelInstanceStateFormat expectedInstanceStateFormat, VertexFormat expectedVertexFormat = VertexFormats::Unknown);
	Shader(std::vector<std::string> vertexFilenames, std::vector<std::string> fragmentFilenames, ModelInstanceStateFormat expectedInstanceStateFormat, VertexFormat expectedVertexFormat = VertexFormats::Unknown);

	const std::unordered_map<ShaderSourceType, std::vector<std::string>>& filenames() const;
	const VertexFormat& expectedVertexFormat() const;
	const ModelInstanceStateFormat& expectedInstanceStateFormat() const;

	~Shader();
};

