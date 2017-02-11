#pragma once
#include <Renderer\TextureBuffer.h>
#include <Renderer\Renderer.h>
#include <Renderer\Camera.h>
#include <Renderer\PointLight.h>
#include <Models\ModelInstance.h>
#include <Renderer\FrameBuffer.h>
#include <Renderer\RenderOptions.h>

class RenderPass
{
private:
	Renderer *renderer;
	std::shared_ptr<FrameBuffer> m_frameBuffer;
	std::shared_ptr<Camera> m_camera;
	RenderOptions options;
public:
	RenderPass();
	~RenderPass();

	void draw();

	void clearColour(glm::vec4 colour);
	void cull(bool enabled);
	void wireframe(bool enabled);
	void camera(std::shared_ptr<Camera> c);
	void frameBuffer(std::shared_ptr<FrameBuffer> buffer);

	void addModelInstance(std::shared_ptr<const ModelInstance> modelInstance);
	void addPointLight(PointLight light);

	void removeModelInstance(std::shared_ptr<const ModelInstance> modelInstance);
	
	void viewport(glm::vec2 origin, glm::vec2 size);

	uint64_t trianglesDrawn() const;
};

