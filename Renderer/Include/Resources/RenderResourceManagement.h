#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include <Buffer/DataBufferView.h>
#include <Resources/RenderResource.h>

#include <glm/vec2.hpp>

#include <Resources/Vertex.h>
#include <Resources/TextureBuffer.h>

namespace Renderer {
	class TextureBuffer;
	class ShaderConstantBuffer;
	class Material;
	class Mesh;
	class BufferFormat;

	enum struct FrameBufferTarget;
	enum struct ShaderSourceType;

	namespace RenderResourceManagement
	{
		struct MeshData {
			RenderResourceID id;
			VertexFormat format;
			std::vector<float> vertexData;
			std::vector<uint32_t> indices;
		};
		struct TextureData {
			RenderResourceID id;
			glm::ivec2 dimensions;
			TextureFormat format;
			std::vector<uint8_t> data;

			TextureData(RenderResourceID id, glm::ivec2 dimensions, TextureFormat format, std::vector<uint8_t>& data);
			TextureData(RenderResourceID id, glm::ivec2 dimensions, TextureFormat format, std::vector<uint8_t>&& data);
		};
		struct FrameBufferData {
			RenderResourceID id;
			std::unordered_map<FrameBufferTarget, std::shared_ptr<TextureBuffer>> targets;
		};
		struct UncompiledShaderData {
			RenderResourceID id;
			std::unordered_map<ShaderSourceType, std::vector<std::string>> sources;
		};
		struct CompiledShaderData {
			RenderResourceID id;
			std::unordered_map<ShaderSourceType, std::vector<uint8_t>> byteCode;
		};
		struct ShaderConstantBufferData {
			RenderResourceID id;
			DataBufferView buffer;
		};
		struct ModelData {
			RenderResourceID id;
			std::shared_ptr<Mesh> mesh;
			std::shared_ptr<Material> material;
			std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures;
		};

		void createMesh(RenderResourceID id, VertexFormat format, std::vector<float> vertexData, std::vector<uint32_t> indices);
		void createTexture(RenderResourceID id, glm::ivec2 dimensions, TextureFormat format, std::vector<uint8_t>& data);
		void createTexture(RenderResourceID id, glm::ivec2 dimensions, TextureFormat format, std::vector<uint8_t>&& data);
		void createFrameBuffer(RenderResourceID id, std::unordered_map<FrameBufferTarget, std::shared_ptr<TextureBuffer>> targets);
		void createShader(RenderResourceID id, std::unordered_map<ShaderSourceType, std::vector<std::string>> sources);
		void createShader(RenderResourceID id, std::unordered_map<ShaderSourceType, std::vector<uint8_t>> byteCode);
		void createShaderConstantBuffer(RenderResourceID id, DataBufferView buffer);
		void createModel(RenderResourceID id, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures);

		std::vector<MeshData> drainPendingMeshes();
		std::vector<TextureData> drainPendingTextures();
		std::vector<FrameBufferData> drainPendinFrameBuffers();
		std::vector<UncompiledShaderData> drainPendingUncompiledShaders();
		std::vector<CompiledShaderData> drainPendingCompiledShaders();
		std::vector<ShaderConstantBufferData> drainPendingShaderConstantBuffers();
		std::vector<ModelData> drainPendingModels();

		void destroyMesh();
	}
}