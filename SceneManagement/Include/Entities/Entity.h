#pragma once

#include <memory>
#include <vector>

#include <Entities\Transform.h>

namespace Renderer {
	class ModelInstance;
}

namespace Scene {
	class Entity
	{
	private:
		Transform m_transform;
		std::shared_ptr<const Renderer::ModelInstance> m_modelInstance;

	public:
		Entity(std::shared_ptr<const Renderer::ModelInstance> modelInstance);
		~Entity();

		Transform& transform();
		const Transform& transform() const;

		float distanceSquaredTo(const glm::vec3& point) const;

		std::shared_ptr<const Renderer::ModelInstance> modelInstance() const;
	};
}

