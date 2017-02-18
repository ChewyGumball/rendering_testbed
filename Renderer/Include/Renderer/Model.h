#pragma once
#include <memory>
#include <unordered_map>

#include <Renderer\RenderResource.h>

#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/TextureBuffer.h"

class Model : public RenderResource
{
protected:
	std::shared_ptr<const Mesh> m_mesh;
	std::shared_ptr<const Shader> m_shader;
	std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> m_textures;
	std::vector<uint8_t> m_uniformData;

public:
	Model(std::shared_ptr<const Mesh> mesh, std::shared_ptr<const Shader> shader, std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures);
	~Model();
	
	std::shared_ptr<const Mesh> mesh() const;
	std::shared_ptr<const Shader> shader() const;
	const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures() const;

	uint64_t triangleCount() const;
};

