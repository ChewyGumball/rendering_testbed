#pragma once
#include <memory>
#include <unordered_map>

#include <Resources\RenderResource.h>

class Mesh;
class Material;
class TextureBuffer;

class Model : public RenderResource
{
protected:
	std::shared_ptr<const Mesh> m_mesh;
	std::shared_ptr<const Material> m_material;
	std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> m_textures;

public:
	Model(std::shared_ptr<const Mesh> mesh, std::shared_ptr<const Material> material, std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures);
	~Model();
	
	std::shared_ptr<const Mesh> mesh() const;
	std::shared_ptr<const Material> material() const;
	const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>>& textures() const;

	uint64_t triangleCount() const;
};

