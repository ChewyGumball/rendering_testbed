#include "Cameras/QuaternionCamera.h"

#include <glm/ext.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

namespace Scene::Cameras {
	QuaternionCamera::QuaternionCamera() :m_position(glm::vec3(0, 0, 0)), m_pitch(0), m_yaw(0), m_roll(0), m_projection(glm::perspective(45.0f, 1.0f, 0.1f, 10000.0f))
	{
	}

	QuaternionCamera::QuaternionCamera(glm::vec3 position, glm::vec3 target, glm::vec3 up, glm::vec4 clippingPlanes)
		: m_position(position), m_projection(glm::ortho(clippingPlanes.x, clippingPlanes.y, clippingPlanes.z, clippingPlanes.w, 0.1f, 10000.0f))
	{
		glm::vec3 view = glm::normalize(target - position);

		m_yaw = glm::atan(view.x, view.z);
		m_pitch = glm::asin(view.y);
		m_roll = up.y == 0 ? 0 : glm::acos(glm::dot(glm::vec3(0, 1, 0), glm::normalize(glm::vec3(up.x, up.y, 0))));
	}

	QuaternionCamera::QuaternionCamera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float aspectRatio)
		: m_position(position), m_projection(glm::perspective(fov, aspectRatio, 0.1f, 10000.0f))
	{
		glm::vec3 view = glm::normalize(target - position);

		m_yaw = glm::atan(view.x, view.z);
		m_pitch = glm::asin(view.y);
		m_roll = up.y == 0 ? 0 : glm::acos(glm::dot(glm::vec3(0, 1, 0), glm::normalize(glm::vec3(up.x, up.y, 0))));
	}


	QuaternionCamera::~QuaternionCamera()
	{
	}

	void QuaternionCamera::move(glm::vec3 displacement)
	{
		m_position += displacement;
	}

	void QuaternionCamera::pitch(float offset)
	{
		m_pitch += offset;
	}

	void QuaternionCamera::yaw(float offset)
	{
		m_yaw += offset;
	}

	void QuaternionCamera::roll(float offset)
	{
		m_roll += offset;
	}

	glm::vec3 QuaternionCamera::position() const
	{
		return m_position;
	}

	glm::quat QuaternionCamera::rotation() const
	{
		return glm::angleAxis(m_yaw, glm::vec3(0, 1, 0)) * glm::angleAxis(m_pitch, glm::vec3(1, 0, 0));
	}

	glm::mat4 QuaternionCamera::transform() const
	{
		return glm::lookAt(m_position, m_position + (rotation() * glm::vec3(0, 0, 1)), glm::vec3(0, 1, 0));
	}

	glm::mat4 QuaternionCamera::inverseTransform() const
	{
		return glm::transpose(transform());
	}

	const glm::mat4& QuaternionCamera::projection() const
	{
		return m_projection;
	}

	const glm::vec3 QuaternionCamera::right() const
	{
		return rotation() * glm::vec3(1, 0, 0);
	}

	const glm::vec3 QuaternionCamera::up() const
	{
		return rotation() * glm::vec3(0, 1, 0);
	}

	const glm::vec3 QuaternionCamera::forward() const
	{
		return rotation() * glm::vec3(0, 0, 1);
	}
}
