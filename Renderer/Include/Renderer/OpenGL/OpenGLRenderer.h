#pragma once
#include <memory>
#include <unordered_set>
#include <vector>

#include <Renderer/IRenderer.h>

class Camera;
class ModelInstance;
class FrameBuffer;
struct RenderOptions;

class OpenGLRenderer : public IRenderer
{
public:
	static void checkGLError();

	OpenGLRenderer();
	~OpenGLRenderer();

	void processRenderingOptions(RenderOptions& options); 
	void updateConstantBuffers(std::unordered_set<std::shared_ptr<ShaderConstantBuffer>>& constantBuffers);
	void draw(const std::vector<std::shared_ptr<const ModelInstance>>& modelInstances, const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>>& globalShaderConstantBuffers);
};

