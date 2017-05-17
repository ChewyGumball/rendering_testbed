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
		std::shared_ptr<const BufferFormat> m_instanceStateFormat;
		std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> m_materialConstantBufferFormats;
		std::vector<std::string> m_systemConstantBufferNames;

	public:
		Shader(std::unordered_map<ShaderSourceType, std::vector<std::string>> sources, std::shared_ptr<const BufferFormat> instanceStateFormat, std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> materialConstantBufferFormats, std::vector<std::string> systemConstantBufferNames);
		Shader(std::unordered_map<ShaderSourceType, std::vector<uint8_t>> byteCode, std::shared_ptr<const BufferFormat> instanceStateFormat, std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> materialConstantBufferFormats, std::vector<std::string> systemConstantBufferNames);
		~Shader();

		std::shared_ptr<const BufferFormat> instanceStateFormat() const;
		const std::unordered_map<std::string, std::shared_ptr<const BufferFormat>>& materialConstantBufferFormats() const;
		const std::vector<std::string>& systemConstantBufferNames() const;
	};
}
