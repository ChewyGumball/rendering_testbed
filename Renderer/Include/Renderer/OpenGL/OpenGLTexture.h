#pragma once
#include <GL\glew.h>
#include <memory>
#include <Resources\Texture.h>

namespace Renderer::Backend::OpenGL {
	class OpenGLTexture
	{
	private:
		GLuint m_handle;
	public:
		OpenGLTexture();
		OpenGLTexture(const glm::ivec2& dimensions, TextureFormat format, const std::vector<uint8_t>& data);
		~OpenGLTexture();

		GLuint handle();
	};
}
