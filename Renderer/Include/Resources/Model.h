#pragma once
#include <memory>
#include <unordered_map>

#include <Resources/RenderResource.h>

namespace Renderer {

	class Mesh;
	class Material;
	class TextureBuffer;

	class Model : public RenderResource
	{
	protected:
		std::shared_ptr<Mesh> m_mesh;
		std::shared_ptr<Material> m_material;

	public:
		Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures);
		~Model();

		std::shared_ptr<Mesh> mesh() const;
		std::shared_ptr<Material> material() const;

		uint64_t triangleCount() const;
	};
}
