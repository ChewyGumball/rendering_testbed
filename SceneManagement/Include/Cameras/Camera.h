#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
namespace Scene::Cameras {
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
		Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, glm::vec4 clippingPlanes);
		Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float aspectRatio);
		~Camera();

		virtual void move(glm::vec3 displacement);
		virtual void pitch(float offset);
		virtual void yaw(float offset);
		virtual void roll(float offset);
		void rotateLocal(glm::vec3 axis, float degrees);

		virtual glm::vec3 position() const;
		virtual glm::mat4 transform() const;
		glm::mat4 inverseTransform() const;
		virtual const glm::mat4& projection() const;
		virtual const glm::vec3 right() const;
		virtual const glm::vec3 up() const;
		virtual const glm::vec3 forward() const;
	};
}
