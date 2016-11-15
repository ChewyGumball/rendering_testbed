#pragma once
#include <glm\glm.hpp>
class PointLight
{
private:
	glm::vec3 m_position;
	glm::vec3 m_intensities;
public:
	PointLight();
	PointLight(glm::vec3 position, glm::vec3 intensity);
	~PointLight();

	glm::vec3 position() const;
	glm::vec3 intensity() const;
};

