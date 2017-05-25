#pragma once
#include "Cameras/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Scene::Cameras {
	class QuaternionCamera : public Camera
	{
	private:
		float m_pitch, m_yaw, m_roll;

	public:
		QuaternionCamera();
		QuaternionCamera(glm::vec3 position, glm::vec3 target, glm::vec3 up, glm::vec4 clippingPlanes);
		QuaternionCamera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float aspectRatio);
		~QuaternionCamera();
		
		void pitch(float offset);
		void yaw(float offset);
		void roll(float offset);

		glm::quat rotation() const;
		glm::mat4 transform() const;

		const glm::vec3 right() const;
		const glm::vec3 up() const;
		const glm::vec3 forward() const;
	};
}
