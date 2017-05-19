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

void Renderer::RenderResourceManagement::createMesh(RenderResourceID id, VertexFormat format, std::vector<float> vertexData, std::vector<uint32_t> indices)
{
	pendingMeshes.push_back({ id, format, vertexData, indices });
}

void Renderer::RenderResourceManagement::createTexture(RenderResourceID id, glm::ivec2 dimensions, TextureFormat format, std::vector<uint8_t>& data)
{
	pendingTextures.emplace_back(id, dimensions, format, data);
}

void Renderer::RenderResourceManagement::createTexture(RenderResourceID id, glm::ivec2 dimensions, TextureFormat format, std::vector<uint8_t>&& data)
{
	pendingTextures.emplace_back(id, dimensions, format, std::move(data));
}

void Renderer::RenderResourceManagement::createFrameBuffer(RenderResourceID id, std::unordered_map<FrameBufferTarget, std::shared_ptr<TextureBuffer>> targets)
{
	pendingFrameBuffers.push_back({ id, targets });
}

void Renderer::RenderResourceManagement::createShader(RenderResourceID id, std::unordered_map<ShaderSourceType, std::vector<std::string>> sources)
{
	pendingUncompiledShaders.push_back({ id, sources });
}
void Renderer::RenderResourceManagement::createShader(RenderResourceID id, std::unordered_map<ShaderSourceType, std::vector<uint8_t>> byteCode)
{
	pendingCompiledShaders.push_back({ id, byteCode });
}

void Renderer::RenderResourceManagement::createShaderConstantBuffer(RenderResourceID id, DataBufferView buffer)
{
	pendingShaderConstantBuffers.push_back({ id, buffer });
}

void Renderer::RenderResourceManagement::createModel(RenderResourceID id, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures)
{
	pendingModels.push_back({ id, mesh, material, textures });
}

std::vector<Renderer::RenderResourceManagement::MeshData> Renderer::RenderResourceManagement::drainPendingMeshes()
{
	return drain(pendingMeshes);
}

std::vector<Renderer::RenderResourceManagement::TextureData> Renderer::RenderResourceManagement::drainPendingTextures()
{
	return drain(pendingTextures);
}

std::vector<Renderer::RenderResourceManagement::FrameBufferData> Renderer::RenderResourceManagement::drainPendinFrameBuffers()
{
	return drain(pendingFrameBuffers);
}

std::vector<Renderer::RenderResourceManagement::UncompiledShaderData> Renderer::RenderResourceManagement::drainPendingUncompiledShaders()
{
	return drain(pendingUncompiledShaders);
}

std::vector<Renderer::RenderResourceManagement::CompiledShaderData> Renderer::RenderResourceManagement::drainPendingCompiledShaders()
{
	return drain(pendingCompiledShaders);
}

std::vector<Renderer::RenderResourceManagement::ShaderConstantBufferData> Renderer::RenderResourceManagement::drainPendingShaderConstantBuffers()
{
	return drain(pendingShaderConstantBuffers);
}

std::vector<Renderer::RenderResourceManagement::ModelData> Renderer::RenderResourceManagement::drainPendingModels()
{
	return drain(pendingModels);
}


void Renderer::RenderResourceManagement::destroyMesh()
{
}

Renderer::RenderResourceManagement::TextureData::TextureData(RenderResourceID id, glm::ivec2 dimensions, TextureFormat format, std::vector<uint8_t>& data)
	:id(id), dimensions(dimensions), format(format), data(data)
{
}

Renderer::RenderResourceManagement::TextureData::TextureData(RenderResourceID id, glm::ivec2 dimensions, TextureFormat format, std::vector<uint8_t>&& data)
	: id(id), dimensions(dimensions), format(format), data(std::move(data))
{
}
