#include "Resources/Shader.h"

namespace Renderer {
	Shader::Shader(std::vector<std::string> vertexSources, std::vector<std::string> fragmentSources, std::shared_ptr<const BufferFormat> instanceStateFormat)
		: m_instanceStateFormat(instanceStateFormat)
	{
		m_sources[ShaderSourceType::VERTEX] = vertexSources;
		m_sources[ShaderSourceType::FRAGMENT] = fragmentSources;
	}

	Shader::Shader(std::vector<std::string> vertexSources, std::vector<std::string> fragmentSources, std::shared_ptr<const BufferFormat> instanceStateFormat, std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> materialConstantBufferFormats, std::vector<std::string> systemConstantBufferNames)
		: m_instanceStateFormat(instanceStateFormat), m_materialConstantBufferFormats(materialConstantBufferFormats), m_systemConstantBufferNames(systemConstantBufferNames)
	{
		m_sources[ShaderSourceType::VERTEX] = vertexSources;
		m_sources[ShaderSourceType::FRAGMENT] = fragmentSources;
	}


	const std::unordered_map<ShaderSourceType, std::vector<std::string>>& Shader::sources() const
	{
		return m_sources;
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
