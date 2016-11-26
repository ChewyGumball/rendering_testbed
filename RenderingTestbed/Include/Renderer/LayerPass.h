#pragma once
#include <Renderer\TextureBuffer.h>
#include <Renderer\Renderer.h>
#include <Renderer\Camera.h>
#include <Renderer\PointLight.h>
#include <Models\ModelInstance.h>

class LayerPass
{
private:
	GLuint framebuffer;
	Renderer *renderer;
public:
	LayerPass();
	~LayerPass();

	void draw(const Camera & c);

	void addModelInstance(std::shared_ptr<const ModelInstance> modelInstance);
	void addPointLight(PointLight light);

	void removeModelInstance(std::shared_ptr<const ModelInstance> modelInstance);

	void clearBuffers(GLuint buffers);

	void setColourBuffer(std::shared_ptr<TextureBuffer> buffer);
	void setDepthBuffer(std::shared_ptr<TextureBuffer> buffer);
};

