#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
class Camera
{
private:
	glm::vec3 m_position;
	glm::mat4 m_transform, m_projection;
public:
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float aspectRatio);
	~Camera();

	void move(glm::vec3 displacement);

	glm::vec3 position() const;
	const glm::mat4& transform() const;
	glm::mat4 inverseTransform() const;
	const glm::mat4& projection() const;
};

