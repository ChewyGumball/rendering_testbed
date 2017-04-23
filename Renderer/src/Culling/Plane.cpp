#include "Culling/Plane.h"
#include <iostream>

namespace Renderer::Culling {

	Plane::Plane(glm::vec4 equation) : coefficients(equation / glm::length(glm::vec3(equation)))
	{ }


	Plane::~Plane()
	{
	}

	std::vector<float> Plane::signedDistances(std::vector<glm::vec3> points)
	{
		std::vector<float> distances(points.size());
		for (size_t i = 0; i < points.size(); ++i)
		{
			distances[i] = glm::dot(coefficients, glm::vec4(points[i], 1));
		}

		return distances;
	}

	void Plane::print()
	{
		std::cout << "[" << coefficients.x << "," << coefficients.y << "," << coefficients.z << "," << coefficients.w << "]" << std::endl;
	}
}