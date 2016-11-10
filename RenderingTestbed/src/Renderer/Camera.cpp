#include "Renderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>



Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float aspectRatio) 
	: m_transform(glm::lookAt(position, target, up)), m_projection(glm::perspective(fov, aspectRatio, 0.1f, 10000.0f))
{
}


Camera::~Camera()
{
}

glm::mat4 Camera::transform() const
{
	return m_transform;
}

glm::mat4 Camera::inverseTransform() const
{
	return glm::transpose(m_transform);
}

glm::mat4 Camera::projection() const
{
	return m_projection;
}
