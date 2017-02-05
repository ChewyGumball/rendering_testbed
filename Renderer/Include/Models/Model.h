#pragma once
#include <memory>
#include <unordered_map>

#include "Models/Material.h"
#include "Models/Mesh.h"
#include "Shaders/Shader.h"
#include "Renderer/TextureBuffer.h"

class Model
{
protected:
	std::size_t m_id;
	std::shared_ptr<const Mesh> m_mesh;
	std::shared_ptr<const Shader> m_shader;
	std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> m_textures;
public:
	Model(std::shared_ptr<const Mesh> mesh, std::shared_ptr<const Shader> shader);
	~Model();

	void setTexture(std::string name, std::shared_ptr<TextureBuffer> texture);

	std::size_t id() const;

	std::shared_ptr<const Mesh> mesh() const;
	std::shared_ptr<const Shader> shader() const;
	const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures() const;

	uint64_t triangleCount() const;
};
