#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
class Camera
{
private:
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::mat4 m_projection;
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float aspectRatio);
	~Camera();

	void move(glm::vec3 displacement);
	void rotateLocal(glm::vec3 axis, float degrees);

	glm::vec3 position() const;
	const glm::mat4 transform() const;
	glm::mat4 inverseTransform() const;
	const glm::mat4& projection() const;
	const glm::vec3& right() const;
	const glm::vec3& up() const;
	const glm::vec3& forward() const;
};

