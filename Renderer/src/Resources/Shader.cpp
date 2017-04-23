#include "Resources/Shader.h"

namespace Renderer {
	Shader::Shader(std::vector<std::string> vertexFilenames, std::vector<std::string> fragmentFilenames, std::shared_ptr<const BufferFormat> instanceStateFormat, VertexFormat expectedVertexFormat)
		: m_expectedVertexFormat(expectedVertexFormat), m_instanceStateFormat(instanceStateFormat)
	{
		m_filenames[ShaderSourceType::VERTEX] = vertexFilenames;
		m_filenames[ShaderSourceType::FRAGMENT] = fragmentFilenames;
	}

	Shader::Shader(std::vector<std::string> vertexFilenames, std::vector<std::string> fragmentFilenames, std::shared_ptr<const BufferFormat> instanceStateFormat, std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> materialConstantBufferFormats, std::vector<std::string> systemConstantBufferNames, VertexFormat expectedVertexFormat)
		: m_expectedVertexFormat(expectedVertexFormat), m_instanceStateFormat(instanceStateFormat), m_materialConstantBufferFormats(materialConstantBufferFormats), m_systemConstantBufferNames(systemConstantBufferNames)
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

	std::shared_ptr<const BufferFormat> Shader::instanceStateFormat() const
	{
		return m_instanceStateFormat;
	}

	const std::unordered_map<std::string, std::shared_ptr<const BufferFormat>>& Shader::materialConstantBufferFormats() const
	{
		return m_materialConstantBufferFormats;
	}

	const std::vector<std::string>& Shader::systemConstantBufferNames() const
	{
		return m_systemConstantBufferNames;
	}

	Shader::~Shader() {}
}
