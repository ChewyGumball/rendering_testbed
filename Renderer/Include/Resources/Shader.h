#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include <Resources/RenderResource.h>

namespace Renderer {

	enum struct ShaderSourceType {
		VERTEX,
		FRAGMENT
	};

	class BufferFormat;

	class Shader : public RenderResource
	{
	private:
		std::unordered_map<ShaderSourceType, std::vector<std::string>> m_sources;
		std::shared_ptr<const BufferFormat> m_instanceStateFormat;
		std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> m_materialConstantBufferFormats;
		std::vector<std::string> m_systemConstantBufferNames;
		bool m_isfileShader;

	public:
		Shader(std::vector<std::string> vertexSources, std::vector<std::string> fragmentSources, std::shared_ptr<const BufferFormat> instanceStateFormat);
		Shader(std::vector<std::string> vertexSources, std::vector<std::string> fragmentSources, std::shared_ptr<const BufferFormat> instanceStateFormat, std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> materialConstantBufferFormats, std::vector<std::string> systemConstantBufferNames);

		const std::unordered_map<ShaderSourceType, std::vector<std::string>>& sources() const;
		std::shared_ptr<const BufferFormat> instanceStateFormat() const;
		const std::unordered_map<std::string, std::shared_ptr<const BufferFormat>>& materialConstantBufferFormats() const;
		const std::vector<std::string>& systemConstantBufferNames() const;

		~Shader();
	};
}
