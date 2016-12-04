#pragma once
#include <vector>
#include <glm\glm.hpp>

namespace Culling {
	class Plane
	{
		glm::vec4 coefficients;
	public:
		Plane(glm::vec4 coefficients);
		~Plane();

		std::vector<float> signedDistances(std::vector<glm::vec3> points);
	};
}

