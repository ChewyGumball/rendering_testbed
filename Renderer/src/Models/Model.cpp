#include "Models/Model.h"

Model::Model(std::shared_ptr<const Mesh> mesh, std::shared_ptr<const Shader> shader) : m_mesh(mesh), m_shader(shader)
{
}

Model::~Model()
{
}

void Model::setTexture(std::string name, std::shared_ptr<TextureBuffer> texture)
{
	m_textures[name] = texture;
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
