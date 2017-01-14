#include "Culling/Frustum.h"
#include <iostream>

#include <glm/gtc/matrix_access.hpp>

namespace Culling {
	Frustum::Frustum(glm::mat4 transform): planes({
		Plane(glm::row(transform, 3) - glm::row(transform, 0)), //left
		Plane(glm::row(transform, 3) + glm::row(transform, 0)), //right
		Plane(glm::row(transform, 3) - glm::row(transform, 1)), //bottom
		Plane(glm::row(transform, 3) + glm::row(transform, 1)), //top
		Plane(glm::row(transform, 3) - glm::row(transform, 2)), //near
		Plane(glm::row(transform, 3) + glm::row(transform, 2))  //far
	})
	{
		/*
		planes[0].print();
		planes[1].print();
		planes[2].print();
		planes[3].print();
		planes[4].print();
		planes[5].print();
		std::cout << std::endl;		
		//*/
	}

	Frustum::~Frustum() {	}

	std::vector<bool> Frustum::intersects(std::vector<glm::vec3> centers, std::vector<float> radii)
	{
		std::vector<bool> intersections(centers.size(), true);

		for (int i = 0; i < 6; i++)
		{
			std::vector<float> distances = planes[i].signedDistances(centers);
			for (size_t j = 0; j < distances.size(); ++j)
			{
				bool a = intersections[j];
				intersections[j] = a & (distances[j] > -radii[j]);
			}
		}

		return intersections;
	}
}