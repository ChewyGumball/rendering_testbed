#pragma once
#include <unordered_map>

#include <GL\glew.h>
#include <memory>
#include <unordered_map>

#include <Resources/RenderResource.h>

namespace Renderer {

	class Material;
	namespace OpenGL {
		class OpenGLRenderMesh;
		class OpenGLShader;

		class OpenGLRenderModel
		{
		private:
			GLuint vao, m_transformVBO;
			uint32_t m_indexCount;
			const std::shared_ptr<OpenGLShader> m_shader;
			const std::unordered_map<std::string, RenderResourceID> m_textures;
			const std::shared_ptr<const Material> m_material;

		public:

			OpenGLRenderModel();
			OpenGLRenderModel(std::shared_ptr<OpenGLRenderMesh> mesh, std::shared_ptr<OpenGLShader> shader, const std::unordered_map<std::string, RenderResourceID> textures, std::shared_ptr<const Material> material);
			~OpenGLRenderModel();

			void draw(int instanceCount) const;

			const std::unordered_map<std::string, RenderResourceID>& textures() const;
			const std::shared_ptr<const Material> material() const;
			const std::shared_ptr<OpenGLShader> shader() const;
			uint32_t indexCount() const;

			GLuint transformVBO() const;
		};
	}
}
