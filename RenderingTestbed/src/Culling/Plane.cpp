#include "Culling/Plane.h"

namespace Culling {

	Plane::Plane(glm::vec4 equation)
	{
		float length = glm::length(glm::vec3(equation));
		coefficients = glm::vec4(-glm::vec3(equation) / length, equation.w / length);
	}


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
}