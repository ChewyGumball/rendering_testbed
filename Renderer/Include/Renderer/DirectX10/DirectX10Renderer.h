#pragma once
#include <Renderer/IRenderer.h>

#include <d3d10.h>

namespace Renderer::DirectX10
{
	class DirectX10Renderer : public IRenderer
	{
	public:
		DirectX10Renderer(HWND window);
		~DirectX10Renderer();
		void processRenderingOptions(RenderOptions& options);
		void updateConstantBuffers(std::unordered_set<std::shared_ptr<ShaderConstantBuffer>>& constantBuffers);
		void draw(const std::vector<std::shared_ptr<const ModelInstance>>& instances, const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>>& renderPassConstants);

	};
}
