#include "Entities/Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>



namespace Scene {
	Transform::Transform() : m_position(glm::vec3()), m_rotationAngles(glm::vec3()), m_scales(glm::vec3(1,1,1))
	{
	}


	Transform::~Transform()
	{
	}

	const glm::vec3 & Transform::position() const
	{
		return m_position;
	}

	const glm::vec3 & Transform::rotationAngles() const
	{
		return m_rotationAngles;
	}

	const glm::vec3 & Transform::scales() const
	{
		return m_scales;
	}

	void Transform::translate(glm::vec3 offset)
	{
		m_position += offset;
	}

	void Transform::rotate(glm::vec3 angles)
	{
		m_rotationAngles += angles;
	}

	void Transform::scale(glm::vec3 factors)
	{
		m_scales *= factors;
	}

	glm::mat4 Transform::matrix() const
	{
		glm::mat4 rotated = glm::eulerAngleXYZ(m_rotationAngles.x, m_rotationAngles.y, m_rotationAngles.z) * glm::scale(glm::mat4(), m_scales);
		return glm::translate(rotated, m_position);
	}
}
