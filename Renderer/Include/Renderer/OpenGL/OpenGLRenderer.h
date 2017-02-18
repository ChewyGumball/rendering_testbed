#pragma once
#include <memory>
#include <vector>

#include <Renderer/Renderer.h>
#include <Renderer/PointLight.h>

class Camera;
class ModelInstance;
class FrameBuffer;
struct RenderOptions;

class OpenGLRenderer : public Renderer
{
private:
	std::vector<PointLight> lights;

public:
	OpenGLRenderer();
	~OpenGLRenderer();

	void addFrameBufferResources(std::shared_ptr<const FrameBuffer> frameBuffer);
	void addModelInstanceResources(std::shared_ptr<const ModelInstance> modelInstance);
	void addPointLight(PointLight light);

	void clearFrameBuffer(std::shared_ptr<const FrameBuffer> frameBuffer, glm::vec4 clearColour);
	void draw(const std::vector<std::shared_ptr<const ModelInstance>>& instances, const std::shared_ptr<Camera> camera, RenderOptions& options);
};

