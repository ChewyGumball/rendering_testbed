#pragma once
#include <memory>

#include "Models/Material.h"
#include "Models/Mesh.h"
#include "Shaders/Shader.h"

class Model
{
protected:
	std::size_t m_id;
	std::shared_ptr<const Mesh> m_mesh;
	std::shared_ptr<const Shader> m_shader;
public:
	Model(std::shared_ptr<const Mesh> mesh, std::shared_ptr<const Shader> shader);
	~Model();

	std::size_t id() const;

	std::shared_ptr<const Mesh> mesh() const;
	std::shared_ptr<const Shader> shader() const;
};

