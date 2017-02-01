#include "Renderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>



Camera::Camera() :m_position(glm::vec3(0,0,0)), m_forward(glm::vec3(1,0,0)), m_up(glm::vec3(0,0,1)), m_right(glm::vec3(0,1,0)), m_projection(glm::perspective(45.0f, 1.0f, 0.1f, 10000.0f))
{
}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float aspectRatio)
	: m_position(position), 
	m_forward(glm::normalize(target - position)), 
	m_up(up), 
	m_right(glm::normalize(glm::cross(m_forward, m_up))), 
	m_projection(glm::perspective(fov, aspectRatio, 0.1f, 10000.0f))
{
}


Camera::~Camera()
{
}

void Camera::move(glm::vec3 displacement)
{
	m_position += displacement;
}

void Camera::rotateLocal(glm::vec3 axis, float degrees)
{	
	m_forward = glm::normalize(glm::vec3(glm::vec4(m_forward, 1) * glm::rotate(degrees, axis)));
	m_right = glm::normalize(glm::cross(m_up, m_forward));
	m_up = glm::normalize(glm::cross(m_forward, m_right));
}

glm::vec3 Camera::position() const
{
	return m_position;
}

const glm::mat4 Camera::transform() const
{
	return glm::lookAt(m_position, m_position + m_forward, m_up);
}

glm::mat4 Camera::inverseTransform() const
{
	return glm::transpose(transform());
}

const glm::mat4& Camera::projection() const
{
	return m_projection;
}

const glm::vec3& Camera::right() const
{
	return m_right;
}

const glm::vec3& Camera::up() const
{
	return m_up;
}

const glm::vec3 & Camera::forward() const
{
	return m_forward;
}
