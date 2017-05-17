#include "Resources/Shader.h"

#include <Resources/RenderResourceManagement.h>

namespace Renderer {
	Shader::Shader(std::unordered_map<ShaderSourceType, std::vector<std::string>> sources, std::shared_ptr<const BufferFormat> instanceStateFormat, std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> materialConstantBufferFormats, std::vector<std::string> systemConstantBufferNames)
		: m_instanceStateFormat(instanceStateFormat), m_materialConstantBufferFormats(materialConstantBufferFormats), m_systemConstantBufferNames(systemConstantBufferNames)
	{
		RenderResourceManagement::createShader(m_id, sources);
	}
	Shader::Shader(std::unordered_map<ShaderSourceType, std::vector<uint8_t>> byteCode, std::shared_ptr<const BufferFormat> instanceStateFormat, std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> materialConstantBufferFormats, std::vector<std::string> systemConstantBufferNames)
		: m_instanceStateFormat(instanceStateFormat), m_materialConstantBufferFormats(materialConstantBufferFormats), m_systemConstantBufferNames(systemConstantBufferNames)
	{
		RenderResourceManagement::createShader(m_id, byteCode);
	}

	Shader::~Shader() {}

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
}
