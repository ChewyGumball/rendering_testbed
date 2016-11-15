#include "Renderer/PointLight.h"



PointLight::PointLight()
{
}

PointLight::PointLight(glm::vec3 position, glm::vec3 intensity):m_position(position), m_intensities(intensity)
{
}


PointLight::~PointLight()
{
}

glm::vec3 PointLight::position() const
{
	return m_position;
}

glm::vec3 PointLight::intensity() const
{
	return m_intensities;
}
