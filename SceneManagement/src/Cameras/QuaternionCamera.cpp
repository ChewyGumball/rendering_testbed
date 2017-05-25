#include "Cameras/QuaternionCamera.h"

#include <glm/ext.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

namespace Scene::Cameras {
	QuaternionCamera::QuaternionCamera() : Camera()
	{
	}

	QuaternionCamera::QuaternionCamera(glm::vec3 position, glm::vec3 target, glm::vec3 up, glm::vec4 clippingPlanes)
		: Camera(position, target, up, clippingPlanes)
	{
		glm::vec3 view = glm::normalize(target - position);

		m_yaw = glm::atan(view.x, view.z);
		m_pitch = glm::asin(view.y);
		m_roll = up.y == 0 ? 0 : glm::acos(glm::dot(glm::vec3(0, 1, 0), glm::normalize(glm::vec3(up.x, up.y, 0))));
	}

	QuaternionCamera::QuaternionCamera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float aspectRatio)
		: Camera(position, target, up, fov, aspectRatio)
	{
		glm::vec3 view = glm::normalize(target - position);

		m_yaw = glm::atan(view.x, view.z);
		m_pitch = glm::asin(view.y);
		m_roll = up.y == 0 ? 0 : glm::acos(glm::dot(glm::vec3(0, 1, 0), glm::normalize(glm::vec3(up.x, up.y, 0))));
	}


	QuaternionCamera::~QuaternionCamera()
	{
	}

	void QuaternionCamera::pitch(float offset)
	{
		float newPitch = m_pitch + offset;
		if (newPitch < glm::half_pi<float>() && newPitch > -glm::half_pi<float>())
		{
			m_pitch = newPitch;
		}
	}

	void QuaternionCamera::yaw(float offset)
	{
		m_yaw += offset;
		m_yaw = fmodf(m_yaw, glm::two_pi<float>());
	}

	void QuaternionCamera::roll(float offset)
	{
		m_roll += offset;
	}

	glm::quat QuaternionCamera::rotation() const
	{
		return glm::angleAxis(m_yaw, glm::vec3(0, 1, 0)) * glm::angleAxis(m_pitch, glm::vec3(1, 0, 0));
	}

	glm::mat4 QuaternionCamera::transform() const
	{
		return glm::lookAt(m_position, m_position + (rotation() * glm::vec3(0, 0, 1)), glm::vec3(0, 1, 0));
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
