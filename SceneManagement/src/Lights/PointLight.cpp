#include "Lights/PointLight.h"



PointLight::PointLight()
{
}

PointLight::PointLight(glm::vec3 position, glm::vec3 intensity, float power):m_position(position), m_intensities(intensity), m_power(power)
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

float PointLight::power() const
{
	return m_power;
}
