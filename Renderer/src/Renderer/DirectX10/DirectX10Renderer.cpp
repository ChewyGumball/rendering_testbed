#include "Renderer/DirectX10/DirectX10Renderer.h"



namespace Renderer::DirectX10 {
	DirectX10Renderer::DirectX10Renderer(HWND window)
	{
	}


	DirectX10Renderer::~DirectX10Renderer()
	{
	}
	void DirectX10Renderer::processRenderingOptions(RenderOptions & options)
	{
	}
	void DirectX10Renderer::updateConstantBuffers(std::unordered_set<std::shared_ptr<ShaderConstantBuffer>>& constantBuffers)
	{
	}
	void DirectX10Renderer::draw(const std::vector<std::shared_ptr<const ModelInstance>>& instances, const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>>& renderPassConstants)
	{
	}
}