#include "Renderer/Material.h"



Material::Material(std::shared_ptr<const Shader> shader, std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>> materialConstants)
	:m_shader(shader), m_materialConstants(materialConstants)
{
}


Material::~Material()
{
}

std::shared_ptr<const Shader> Material::shader() const
{
	return m_shader;
}

const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>>& Material::constants() const
{
	return m_materialConstants;
}
