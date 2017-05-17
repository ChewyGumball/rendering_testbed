#pragma once
#include <GL\glew.h>
#include <memory>
#include <Resources\TextureBuffer.h>

namespace Renderer::OpenGL {
	class OpenGLTextureBuffer
	{
	private:
		GLuint m_handle;
	public:
		OpenGLTextureBuffer();
		OpenGLTextureBuffer(glm::ivec2 dimensions, TextureFormat format, std::vector<uint8_t> data);
		~OpenGLTextureBuffer();

		GLuint handle();
	};
}
