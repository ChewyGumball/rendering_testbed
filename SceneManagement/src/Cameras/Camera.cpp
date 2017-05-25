#include "Cameras/Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>


namespace Scene::Cameras {

	Camera::Camera() 
		: m_fov(45), 
		m_aspectRatio(1), 
		m_position(glm::vec3(0, 0, 0)), 
		m_forward(glm::vec3(1, 0, 0)), 
		m_up(glm::vec3(0, 0, 1)), 
		m_right(glm::vec3(0, 1, 0)), 
		m_projection(glm::perspective(45.0f, 1.0f, 0.1f, 10000.0f))
	{
	}

	Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, glm::vec4 clippingPlanes)
		: m_fov(45),
		m_aspectRatio(1),
		m_position(position),
		m_forward(glm::normalize(target - position)),
		m_up(up),
		m_right(glm::normalize(glm::cross(m_forward, m_up))),
		m_projection(glm::ortho(clippingPlanes.x, clippingPlanes.y, clippingPlanes.z, clippingPlanes.w, 0.1f, 10000.0f))
	{
	}

	Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float aspectRatio)
		: m_fov(fov),
		m_aspectRatio(aspectRatio),
		m_position(position),
		m_forward(glm::normalize(target - position)),
		m_up(up),
		m_right(glm::normalize(glm::cross(m_forward, m_up))),
		m_projection(glm::perspective(fov, aspectRatio, 0.1f, 10000.0f))
	{
	}


	Camera::~Camera()
	{
	}

	float Camera::fov() const
	{
		return m_fov;
	}

	float Camera::aspectRatio() const
	{
		return m_aspectRatio;
	}

	void Camera::move(glm::vec3 displacement)
	{
		m_position += displacement;
	}

	void Camera::pitch(float offset)
	{
		rotateLocal(m_right, offset);
	}

	void Camera::yaw(float offset)
	{
		rotateLocal(m_up, offset);
	}

	void Camera::roll(float offset)
	{
		rotateLocal(m_forward, offset);
	}

	void Camera::rotateLocal(glm::vec3 axis, float degrees)
	{
		if (axis == m_forward) {
			m_right = glm::normalize(glm::vec3(glm::rotate(degrees, axis) * glm::vec4(m_right, 1)));
			m_forward = glm::normalize(glm::cross(m_up, m_right));
		}
		else {
			m_forward = glm::normalize(glm::vec3(glm::rotate(degrees, axis) * glm::vec4(m_forward, 1)));
			m_right = glm::normalize(glm::cross(m_up, m_forward));
		}
	}

	glm::vec3 Camera::position() const
	{
		return m_position;
	}

	glm::vec3 Camera::pickWorldRay(glm::vec2 normalizedScreenCoordinates)
	{
		return glm::normalize(glm::inverse(m_projection * this->transform()) * glm::vec4(normalizedScreenCoordinates, 1, 1));
	}

	glm::mat4 Camera::transform() const
	{
		return glm::lookAt(m_position, m_position + m_forward, m_up);
	}

	glm::mat4 Camera::inverseTransform() const
	{
		return glm::transpose(this->transform());
	}

	const glm::mat4& Camera::projection() const
	{
		return m_projection;
	}

	glm::vec3 Camera::pickViewRay(glm::vec2 normalizedScreenCoordinates)
	{
		return glm::normalize(glm::inverse(m_projection) * glm::vec4(normalizedScreenCoordinates, 1, 1));
	}

	const glm::vec3 Camera::right() const
	{
		return m_right;
	}

	const glm::vec3 Camera::up() const
	{
		return glm::normalize(glm::cross(m_forward, m_right));
	}

	const glm::vec3 Camera::forward() const
	{
		return m_forward;
	}
}
