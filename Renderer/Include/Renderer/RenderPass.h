#pragma once
#include <vector>

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
	std::shared_ptr<Camera> m_camera;
	RenderOptions options;
	bool cullingEnabled;
	glm::vec4 m_clearColour;
	bool m_clearBuffers;
	std::vector<std::shared_ptr<const ModelInstance>> modelInstances;

public:
	RenderPass();
	~RenderPass();

	//Returns how many triangles were drawn (after culling if culling is enabled)
	uint64_t draw();

	void clearBuffers(bool enabled);
	void clearColour(glm::vec4 colour);
	void cull(bool enabled);
	void wireframe(bool enabled);
	void camera(std::shared_ptr<Camera> c);
	void frameBuffer(std::shared_ptr<FrameBuffer> buffer);

	void addModelInstance(std::shared_ptr<const ModelInstance> modelInstance);
	void addPointLight(PointLight light);

	void removeModelInstance(std::shared_ptr<const ModelInstance> modelInstance);
	
	void viewport(glm::vec2 origin, glm::vec2 size);
};

