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

	void addPointLight(PointLight light);

	void processRenderingOptions(RenderOptions& options);
	void draw(const std::vector<std::shared_ptr<const ModelInstance>>& instances, const std::shared_ptr<Camera> camera);
};

