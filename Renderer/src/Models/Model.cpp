#include "Models/Model.h"

Model::Model(std::shared_ptr<const Mesh> mesh, std::shared_ptr<const Shader> shader, std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures) : m_mesh(mesh), m_shader(shader), m_textures(textures)
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

const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> Model::textures() const
{
	return m_textures;
}

uint64_t Model::triangleCount() const
{
	return m_mesh->triangleCount();
}
