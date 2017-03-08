#include "Renderer/Model.h"

Model::Model(std::shared_ptr<const Mesh> mesh, std::shared_ptr<const Shader> shader, std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures, std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>> shaderConstants)
	: m_mesh(mesh), m_shader(shader), m_textures(textures), m_shaderConstants(shaderConstants)
{
}

Model::~Model()
{
}

std::shared_ptr<const Mesh> Model::mesh() const
{
	return m_mesh;
}

std::shared_ptr<const Shader> Model::shader() const
{
	return m_shader;
}

const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>>& Model::shaderConstants() const
{
	return m_shaderConstants;
}

const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>>& Model::textures() const
{
	return m_textures;
}

uint64_t Model::triangleCount() const
{
	return m_mesh->triangleCount();
}
