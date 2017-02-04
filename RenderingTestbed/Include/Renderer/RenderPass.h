#pragma once
#include <Renderer\TextureBuffer.h>
#include <Renderer\Renderer.h>
#include <Renderer\Camera.h>
#include <Renderer\PointLight.h>
#include <Models\ModelInstance.h>

class RenderPass
{
private:
	GLuint framebuffer;
	Renderer *renderer;
	std::shared_ptr<Camera> m_camera;
	glm::vec4 m_clearColour;
	bool culllingEnabled;
	glm::vec2 viewportOrigin, viewportSize;
public:
	RenderPass();
	~RenderPass();

	void draw();

	void clearColour(glm::vec4 colour);
	void cull(bool enabled);
	void camera(std::shared_ptr<Camera> c);

	void addModelInstance(std::shared_ptr<const ModelInstance> modelInstance);
	void addPointLight(PointLight light);

	void removeModelInstance(std::shared_ptr<const ModelInstance> modelInstance);

	void clearBuffers(GLuint buffers);

	void setColourBuffer(std::shared_ptr<TextureBuffer> buffer);
	void setDepthBuffer(std::shared_ptr<TextureBuffer> buffer);

	void viewport(glm::vec2 origin, glm::vec2 size);

	uint64_t trianglesDrawn() const;
};

