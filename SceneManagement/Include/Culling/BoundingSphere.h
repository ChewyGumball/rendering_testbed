#pragma once
#include <glm\glm.hpp>


namespace Renderer::Culling {
	class Plane;
	class BoundingSphere
	{
		glm::vec3 m_center;
		float m_radius;
	public:
		BoundingSphere(glm::vec3 center, float radius) : m_center(center), m_radius(radius) {}
		~BoundingSphere() {}

		glm::vec3 center() const { return m_center; }
		float radius() const { return m_radius; }

		bool outside(Plane& plane);
	};
}

