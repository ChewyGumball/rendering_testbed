#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <Resources/RenderResource.h>
#include <Resources/Vertex.h>

#include <GL\glew.h>
#include <glm\glm.hpp>
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
		VertexFormat m_expectedVertexFormat;
		std::shared_ptr<const BufferFormat> m_instanceStateFormat;
		std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> m_materialConstantBufferFormats;
		std::vector<std::string> m_systemConstantBufferNames;
		bool m_isfileShader;

	public:
		Shader(std::vector<std::string> vertexSources, std::vector<std::string> fragmentSources, std::shared_ptr<const BufferFormat> instanceStateFormat, VertexFormat expectedVertexFormat = VertexFormats::Unknown);
		Shader(std::vector<std::string> vertexSources, std::vector<std::string> fragmentSources, std::shared_ptr<const BufferFormat> instanceStateFormat, std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> materialConstantBufferFormats, std::vector<std::string> systemConstantBufferNames, VertexFormat expectedVertexFormat = VertexFormats::Unknown);

		const std::unordered_map<ShaderSourceType, std::vector<std::string>>& sources() const;
		const VertexFormat& expectedVertexFormat() const;
		std::shared_ptr<const BufferFormat> instanceStateFormat() const;
		const std::unordered_map<std::string, std::shared_ptr<const BufferFormat>>& materialConstantBufferFormats() const;
		const std::vector<std::string>& systemConstantBufferNames() const;

		~Shader();
	};
}
