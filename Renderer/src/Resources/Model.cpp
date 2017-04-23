#include "Resources/Model.h"

#include <Resources/Mesh.h>

namespace Renderer {
	Model::Model(std::shared_ptr<const Mesh> mesh, std::shared_ptr<const Material> material, std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures)
		: m_mesh(mesh), m_material(material), m_textures(textures)
	{
	}

	Model::~Model()
	{
	}

	std::shared_ptr<const Mesh> Model::mesh() const
	{
		return m_mesh;
	}

	std::shared_ptr<const Material> Model::material() const
	{
		return m_material;
	}

	const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>>& Model::textures() const
	{
		return m_textures;
	}

	uint64_t Model::triangleCount() const
	{
		return m_mesh->triangleCount();
	}
}
