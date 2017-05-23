#include <Resources/RenderResourceManagement.h>

#include <Resources/Vertex.h>

namespace {
	std::vector<Renderer::RenderResourceManagement::MeshData> pendingMeshes;
	std::vector<Renderer::RenderResourceManagement::TextureData> pendingTextures;
	std::vector<Renderer::RenderResourceManagement::FrameBufferData> pendingFrameBuffers;
	std::vector<Renderer::RenderResourceManagement::UncompiledShaderData> pendingUncompiledShaders;
	std::vector<Renderer::RenderResourceManagement::CompiledShaderData> pendingCompiledShaders;
	std::vector<Renderer::RenderResourceManagement::ShaderConstantBufferData> pendingShaderConstantBuffers;
	std::vector<Renderer::RenderResourceManagement::ModelData> pendingModels;

	template <typename T>
	std::vector<T> drain(std::vector<T>& source) {
		std::vector<T> d;
		std::swap(source, d);
		return d;
	}
}

namespace Renderer {

	void RenderResourceManagement::createMesh(RenderResourceID id, VertexFormat format, const std::vector<float>& vertexData, const std::vector<uint32_t>& indices)
	{
		pendingMeshes.push_back({ id, format, vertexData, indices });
	}

	void RenderResourceManagement::createMesh(RenderResourceID id, VertexFormat format, std::vector<float>&& vertexData, std::vector<uint32_t>&& indices)
	{
		pendingMeshes.push_back({ id, format, std::move(vertexData), std::move(indices) });
	}

	void RenderResourceManagement::createTexture(RenderResourceID id, const glm::ivec2& dimensions, TextureFormat format, const std::vector<uint8_t>& data)
	{
		pendingTextures.emplace_back(id, dimensions, format, data);
	}

	void RenderResourceManagement::createTexture(RenderResourceID id, const glm::ivec2& dimensions, TextureFormat format, std::vector<uint8_t>&& data)
	{
		pendingTextures.emplace_back(id, dimensions, format, std::move(data));
	}

	void RenderResourceManagement::createFrameBuffer(RenderResourceID id, const std::unordered_map<FrameBufferTarget, std::shared_ptr<TextureBuffer>>& targets)
	{
		pendingFrameBuffers.push_back({ id, targets });
	}

	void RenderResourceManagement::createFrameBuffer(RenderResourceID id, std::unordered_map<FrameBufferTarget, std::shared_ptr<TextureBuffer>>&& targets)
	{
		pendingFrameBuffers.push_back({ id, std::move(targets) });
	}

	void RenderResourceManagement::createShader(RenderResourceID id, const std::unordered_map<ShaderSourceType, std::vector<std::string>>& sources)
	{
		pendingUncompiledShaders.push_back({ id, sources });
	}

	void RenderResourceManagement::createShader(RenderResourceID id, std::unordered_map<ShaderSourceType, std::vector<std::string>>&& sources)
	{
		pendingUncompiledShaders.push_back({ id, std::move(sources) });
	}

	void RenderResourceManagement::createShader(RenderResourceID id, const std::unordered_map<ShaderSourceType, std::vector<uint8_t>>& byteCode)
	{
		pendingCompiledShaders.push_back({ id, byteCode });
	}

	void RenderResourceManagement::createShader(RenderResourceID id, std::unordered_map<ShaderSourceType, std::vector<uint8_t>>&& byteCode)
	{
		pendingCompiledShaders.push_back({ id, std::move(byteCode) });
	}

	void RenderResourceManagement::createShaderConstantBuffer(RenderResourceID id, const DataBufferView& buffer)
	{
		pendingShaderConstantBuffers.push_back({ id, buffer });
	}

	void RenderResourceManagement::createModel(RenderResourceID id, const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>>& textures)
	{
		pendingModels.push_back({ id, mesh, material, textures });
	}

	void RenderResourceManagement::createModel(RenderResourceID id, const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, std::unordered_map<std::string, std::shared_ptr<TextureBuffer>>&& textures)
	{
		pendingModels.push_back({ id, mesh, material, std::move(textures) });
	}

	std::vector<RenderResourceManagement::MeshData> RenderResourceManagement::drainPendingMeshes()
	{
		return drain(pendingMeshes);
	}

	std::vector<RenderResourceManagement::TextureData> RenderResourceManagement::drainPendingTextures()
	{
		return drain(pendingTextures);
	}

	std::vector<RenderResourceManagement::FrameBufferData> RenderResourceManagement::drainPendinFrameBuffers()
	{
		return drain(pendingFrameBuffers);
	}

	std::vector<RenderResourceManagement::UncompiledShaderData> RenderResourceManagement::drainPendingUncompiledShaders()
	{
		return drain(pendingUncompiledShaders);
	}

	std::vector<RenderResourceManagement::CompiledShaderData> RenderResourceManagement::drainPendingCompiledShaders()
	{
		return drain(pendingCompiledShaders);
	}

	std::vector<RenderResourceManagement::ShaderConstantBufferData> RenderResourceManagement::drainPendingShaderConstantBuffers()
	{
		return drain(pendingShaderConstantBuffers);
	}

	std::vector<RenderResourceManagement::ModelData> RenderResourceManagement::drainPendingModels()
	{
		return drain(pendingModels);
	}


	void RenderResourceManagement::destroyMesh()
	{
	}

	RenderResourceManagement::TextureData::TextureData(RenderResourceID id, glm::ivec2 dimensions, TextureFormat format, const std::vector<uint8_t>& data)
		:id(id), dimensions(dimensions), format(format), data(data)
	{ }
	RenderResourceManagement::TextureData::TextureData(RenderResourceID id, glm::ivec2 dimensions, TextureFormat format, std::vector<uint8_t>&& data)
		: id(id), dimensions(dimensions), format(format), data(std::move(data))
	{ }

	RenderResourceManagement::MeshData::MeshData(RenderResourceID id, VertexFormat format, const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData)
		: id(id), format(format), vertexData(vertexData), indexData(indexData)
	{}
	RenderResourceManagement::MeshData::MeshData(RenderResourceID id, VertexFormat format, std::vector<float>&& vertexData, std::vector<uint32_t>&& indexData)
		: id(id), format(format), vertexData(std::move(vertexData)), indexData(std::move(indexData))
	{}

	RenderResourceManagement::FrameBufferData::FrameBufferData(RenderResourceID id, const std::unordered_map<FrameBufferTarget, std::shared_ptr<TextureBuffer>>& targets)
		:id(id), targets(targets)
	{}
	RenderResourceManagement::FrameBufferData::FrameBufferData(RenderResourceID id, std::unordered_map<FrameBufferTarget, std::shared_ptr<TextureBuffer>>&& targets)
		: id(id), targets(std::move(targets))
	{}

	RenderResourceManagement::UncompiledShaderData::UncompiledShaderData(RenderResourceID id, const  std::unordered_map<ShaderSourceType, std::vector<std::string>>& sources)
		:id(id), sources(sources)
	{}
	RenderResourceManagement::UncompiledShaderData::UncompiledShaderData(RenderResourceID id, std::unordered_map<ShaderSourceType, std::vector<std::string>>&& sources)
		: id(id), sources(std::move(sources))
	{}

	RenderResourceManagement::CompiledShaderData::CompiledShaderData(RenderResourceID id, const std::unordered_map<ShaderSourceType, std::vector<uint8_t>>& byteCode)
		:id(id), byteCode(byteCode)
	{}
	RenderResourceManagement::CompiledShaderData::CompiledShaderData(RenderResourceID id, std::unordered_map<ShaderSourceType, std::vector<uint8_t>>&& byteCode)
		: id(id), byteCode(std::move(byteCode))
	{}

	RenderResourceManagement::ModelData::ModelData(RenderResourceID id, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>>& textures)
		:id(id), mesh(mesh), material(material), textures(textures)
	{}
	RenderResourceManagement::ModelData::ModelData(RenderResourceID id, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, std::unordered_map<std::string, std::shared_ptr<TextureBuffer>>&& textures)
		: id(id), mesh(mesh), material(material), textures(std::move(textures))
	{}
}
