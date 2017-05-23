#include "Resources/Model.h"

#include <Resources/RenderResourceManagement.h>

#include <Resources/Mesh.h>

namespace Renderer {
	Model::Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures, bool isOpaque)
		: m_opaque(isOpaque), m_mesh(mesh), m_material(material)
	{
		RenderResourceManagement::createModel(m_id, mesh, material, textures);
	}

	Model::~Model()
	{
	}

	std::shared_ptr<Mesh> Model::mesh() const
	{
		return m_mesh;
	}

	std::shared_ptr<Material> Model::material() const
	{
		return m_material;
	}

	bool Model::isOpaque() const
	{
		return m_opaque;
	}

	uint64_t Model::triangleCount() const
	{
		return m_mesh->triangleCount();
	}
}
