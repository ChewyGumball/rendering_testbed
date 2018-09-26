#include "Renderer/OpenGL/OpenGLTextureBuffer.h"

#include <unordered_map>

namespace {
	std::unordered_map<Renderer::Backend::TextureFormat, GLenum> formats{
		{ Renderer::Backend::TextureFormat::RGB8, GL_RGB },
		{ Renderer::Backend::TextureFormat::RGBA8, GL_RGBA},
		{ Renderer::Backend::TextureFormat::DEPTH, GL_DEPTH_COMPONENT },
		{ Renderer::Backend::TextureFormat::GREYSCALE, GL_RED }
	};

	std::unordered_map<Renderer::Backend::TextureFormat, GLenum> internalFormats {
		{ Renderer::Backend::TextureFormat::RGB8, GL_RGB8 },
		{ Renderer::Backend::TextureFormat::RGBA8, GL_RGBA8 },
		{ Renderer::Backend::TextureFormat::DEPTH, GL_DEPTH_COMPONENT24 },
		{ Renderer::Backend::TextureFormat::GREYSCALE, GL_RGB }
	};
}

namespace Renderer::Backend::OpenGL {
	OpenGLTexture::OpenGLTexture() : m_handle(0)
	{
	}

	OpenGLTexture::OpenGLTexture(const glm::ivec2& dimensions, TextureFormat format, const std::vector<uint8_t>& data)
	{
		glGenTextures(1, &m_handle);
		glBindTexture(GL_TEXTURE_2D, m_handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormats[format], dimensions.x, dimensions.y, 0, formats[format], GL_UNSIGNED_BYTE, data.data());

		glBindTexture(GL_TEXTURE_2D, 0);
	}


	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_handle);
	}

	GLuint OpenGLTexture::handle()
	{
		return m_handle;
	}
}
