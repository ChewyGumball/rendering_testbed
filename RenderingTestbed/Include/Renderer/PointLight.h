#pragma once
#include <glm\glm.hpp>
class PointLight
{
private:
	glm::vec3 m_position;
	glm::vec3 m_intensities;
	float m_power;
public:
	PointLight();
	PointLight(glm::vec3 position, glm::vec3 intensity, float power = 1);
	~PointLight();

	glm::vec3 position() const;
	glm::vec3 intensity() const;
	float power() const;
};

