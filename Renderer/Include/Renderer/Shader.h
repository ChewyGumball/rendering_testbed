#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <Renderer/RenderResource.h>
#include <Buffer/BufferFormat.h>
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
	std::shared_ptr<BufferFormat> m_expectedInstanceStateFormat;
	std::shared_ptr<BufferFormat> m_expectedUniformStateFormat;
	bool m_isfileShader;

public:
	Shader(std::string vertexFilename, std::string fragmentFilename, std::shared_ptr<BufferFormat> expectedInstanceStateFormat, std::shared_ptr<BufferFormat> expectedUniformStateFormat = std::make_shared<BufferFormat>(), VertexFormat expectedVertexFormat = VertexFormats::Unknown);
	Shader(std::vector<std::string> vertexFilenames, std::vector<std::string> fragmentFilenames, std::shared_ptr<BufferFormat> expectedInstanceStateFormat, std::shared_ptr<BufferFormat> expectedUniformStateFormat = std::make_shared<BufferFormat>(), VertexFormat expectedVertexFormat = VertexFormats::Unknown);

	const std::unordered_map<ShaderSourceType, std::vector<std::string>>& filenames() const;
	const VertexFormat& expectedVertexFormat() const;
	const std::shared_ptr<BufferFormat> expectedInstanceStateFormat() const;
	const std::shared_ptr<BufferFormat> expectedConstantStateFormat() const;

	~Shader();
};

