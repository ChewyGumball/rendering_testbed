#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Scene {
	class Transform
	{
	private:
		glm::vec3 m_position;
		glm::vec3 m_rotationAngles;
		glm::vec3 m_scales;

	public:
		Transform();
		~Transform();

		const glm::vec3& position() const;
		const glm::vec3& rotationAngles() const;
		const glm::vec3& scales() const;

		void translate(glm::vec3 offset);
		void rotate(glm::vec3 angles);
		void scale(glm::vec3 factors);

		glm::mat4 matrix() const;

	};
}
