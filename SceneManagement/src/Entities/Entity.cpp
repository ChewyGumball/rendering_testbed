#include "Entities/Entity.h"

#include <glm/glm.hpp>

#include <Resources/ModelPart.h>
#include "Resources/ModelInstance.h"
#include <Resources/Mesh.h>
#include <Culling\BoundingSphere.h>

namespace Scene {
	Entity::Entity(std::shared_ptr<const Renderer::ModelInstance> modelInstance)
		: m_modelInstance(modelInstance)
	{
	}

	Entity::~Entity()
	{
	}
	Transform & Entity::transform()
	{
		return m_transform;
	}
	const Transform & Entity::transform() const
	{
		return m_transform;
	}

	float Entity::distanceSquaredTo(const glm::vec3 & point) const
	{
		glm::vec3 toPoint = point - m_transform.position();
		return glm::dot(toPoint, toPoint);
	}

	std::shared_ptr<const Renderer::ModelInstance> Entity::modelInstance() const
	{
		return m_modelInstance;
	}
}