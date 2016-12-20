#include "Renderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>



Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float aspectRatio) 
	: m_position(position), m_transform(glm::lookAt(position, target, up)), m_projection(glm::perspective(fov, aspectRatio, 0.1f, 10000.0f))
{
}


Camera::~Camera()
{
}

void Camera::move(glm::vec3 displacement)
{
	m_transform = glm::translate(displacement) * m_transform;
	m_position += displacement;
}



glm::vec3 Camera::position() const
{
	return m_position;
}

const glm::mat4& Camera::transform() const
{
	return m_transform;
}

glm::mat4 Camera::inverseTransform() const
{
	return glm::transpose(m_transform);
}

const glm::mat4& Camera::projection() const
{
	return m_projection;
}
