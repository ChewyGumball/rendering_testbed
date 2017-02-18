#pragma once
#include <string>
#include <unordered_map>
#include <Renderer/RenderResource.h>
#include <Renderer/BufferFormat.h>
#include <Renderer/Vertex.h>

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
	BufferFormat m_expectedInstanceStateFormat;
	BufferFormat m_expectedUniformStateFormat;

public:
	Shader(std::string vertexFilename, std::string fragmentFilename, BufferFormat expectedInstanceStateFormat, BufferFormat expectedUniformStateFormat = BufferFormat(), VertexFormat expectedVertexFormat = VertexFormats::Unknown);
	Shader(std::vector<std::string> vertexFilenames, std::vector<std::string> fragmentFilenames, BufferFormat expectedInstanceStateFormat, BufferFormat expectedUniformStateFormat = BufferFormat(), VertexFormat expectedVertexFormat = VertexFormats::Unknown);

	const std::unordered_map<ShaderSourceType, std::vector<std::string>>& filenames() const;
	const VertexFormat& expectedVertexFormat() const;
	const BufferFormat& expectedInstanceStateFormat() const;
	const BufferFormat& expectedUniformStateFormat() const;

	~Shader();
};

