#pragma once
#include <memory>
#include <vector>
#include <unordered_set>

#include <Renderer/RenderOptions.h>
namespace Renderer {
	class ShaderConstantBuffer;
	class ModelInstance;

	class IRenderer
	{
	public:
		IRenderer() {};
		virtual ~IRenderer() {};

		virtual void processRenderingOptions(RenderOptions& options) = 0;
		virtual void updateConstantBuffers(std::unordered_set<std::shared_ptr<ShaderConstantBuffer>>& constantBuffers) = 0;
		virtual void draw(const std::vector<std::shared_ptr<const ModelInstance>>& instances, const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>>& renderPassConstants) = 0;
	};
}