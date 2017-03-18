#pragma once
#include <memory>
#include <unordered_set>
#include <vector>

#include <Renderer/Renderer.h>

class Camera;
class ModelInstance;
class FrameBuffer;
struct RenderOptions;

class OpenGLRenderer : public Renderer
{
public:
	static void checkGLError();

	OpenGLRenderer();
	~OpenGLRenderer();

	void processRenderingOptions(RenderOptions& options); 
	void updateConstantBuffers(std::unordered_set<std::shared_ptr<ShaderConstantBuffer>>& constantBuffers);
	void draw(const std::vector<std::shared_ptr<const ModelInstance>>& instances, const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>>& renderPassConstants);
};

