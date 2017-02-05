#pragma once
#include <array>
#include <vector>
#include <glm/glm.hpp>

#include "BoundingSphere.h"
#include "Plane.h"

namespace Culling {
	class Frustum
	{
		std::array<Plane, 6> planes;
	public:
		Frustum(glm::mat4 transform);
		~Frustum();

		std::vector<bool> intersects(std::vector<glm::vec3> centers, std::vector<float> radii);
	};
}

