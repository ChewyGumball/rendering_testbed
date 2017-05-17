#pragma once
#include <unordered_map>

#include <GL\glew.h>
#include <memory>
#include <unordered_map>

#include <Resources/RenderResource.h>

namespace Renderer {
	class BufferFormat;
	class Material;
	class TextureBuffer;

	namespace OpenGL {
		class OpenGLRenderMesh;
		class OpenGLShader;

		class OpenGLRenderModel
		{
		private:
			GLuint vao, m_transformVBO;
			uint32_t m_indexCount;
			const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> m_textures;
			const std::shared_ptr<const Material> m_material;

		public:
			OpenGLRenderModel(OpenGLRenderMesh& mesh, OpenGLShader& shader, const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures, std::shared_ptr<const Material> material);
			~OpenGLRenderModel();

			void draw(int instanceCount) const;

			const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>>& textures() const;
			const std::shared_ptr<const Material> material() const;
			uint32_t indexCount() const;

			GLuint transformVBO() const;
		};
	}
}
