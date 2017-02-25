#pragma once
#include "Renderer/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

class QuaternionCamera : public Camera
{
private:
	glm::vec3 m_position;
	float m_pitch, m_yaw, m_roll;
	glm::mat4 m_projection;
public:
	QuaternionCamera();
	QuaternionCamera(glm::vec3 position, glm::vec3 target, glm::vec3 up, glm::vec4 clippingPlanes);
	QuaternionCamera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float aspectRatio);
	~QuaternionCamera();

	void move(glm::vec3 displacement);

	void pitch(float offset);
	void yaw(float offset);
	void roll(float offset);

	glm::vec3 position() const;
	glm::quat rotation() const;
	glm::mat4 transform() const;
	glm::mat4 inverseTransform() const;
	const glm::mat4& projection() const;
	const glm::vec3 right() const;
	const glm::vec3 up() const;
	const glm::vec3 forward() const;
};

