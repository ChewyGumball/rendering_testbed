#pragma once
#include <memory>
#include <unordered_map>

namespace Renderer {
	class Shader;
	class ShaderConstantBuffer;

	class Material
	{
	private:
		std::shared_ptr<const Shader> m_shader;
		std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>> m_materialConstants;
	public:
		Material(std::shared_ptr<const Shader> shader, std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>> materialConstants = std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>>());
		~Material();

		std::shared_ptr<const Shader> shader() const;
		const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>>& constants() const;
	};
}
