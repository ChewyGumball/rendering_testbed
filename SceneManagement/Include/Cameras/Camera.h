#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
namespace Scene::Cameras {
	class Camera
	{
	protected:

		float m_fov;
		float m_aspectRatio;

		glm::vec3 m_position;
		glm::vec3 m_forward;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::mat4 m_projection;

	public:
		Camera();
		Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, glm::vec4 clippingPlanes);
		Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float aspectRatio);
		virtual ~Camera();

		float fov() const;
		float aspectRatio() const;

		virtual void move(glm::vec3 displacement);
		virtual void pitch(float offset);
		virtual void yaw(float offset);
		virtual void roll(float offset);
		void rotateLocal(glm::vec3 axis, float degrees);

		glm::vec3 position() const;
		const glm::mat4& projection() const;

		glm::vec3 pickViewRay(glm::vec2 normalizedScreenCoordinates);
		glm::vec3 pickWorldRay(glm::vec2 normalizedScreenCoordinates);

		virtual glm::mat4 transform() const;
		glm::mat4 inverseTransform() const;

		virtual const glm::vec3 right() const;
		virtual const glm::vec3 up() const;
		virtual const glm::vec3 forward() const;
	};
}
