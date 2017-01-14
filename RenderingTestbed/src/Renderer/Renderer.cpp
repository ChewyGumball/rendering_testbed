#include "Renderer/Renderer.h"

#include "Culling/Frustum.h"

std::unordered_map<size_t, std::vector<std::shared_ptr<const ModelInstance>>> Renderer::cull(const Camera & camera, std::vector<std::shared_ptr<const ModelInstance>> instances)
{
	glm::mat4 t = camera.transform();
	glm::mat4 p = camera.projection();
	glm::mat4 c = p * t;
	Culling::Frustum frustum(c);
	
	//I don't know why, but in debug mode, this inline multiplication doesn't compute properly
	//Culling::Frustum frustum(camera.projection() * camera.transform());
	std::vector<glm::vec3> centers(instances.size());
	std::vector<float> radii(instances.size());

	for (size_t i = 0; i < instances.size(); ++i)
	{
		Culling::BoundingSphere s = instances[i]->bounds();
		centers[i] = s.center();
		radii[i] = s.radius();
	}

	std::vector<bool> intersections = frustum.intersects(centers, radii);

	std::unordered_map<size_t, std::vector<std::shared_ptr<const ModelInstance>>> visibleModels;

	for (size_t i = 0; i < instances.size(); ++i)
	{
		if (intersections[i])
		{
			visibleModels[instances[i]->model()->id()].push_back(instances[i]);
		}
	}

	return visibleModels;
}
