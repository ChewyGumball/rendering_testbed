#include "Renderer/OpenGL/OpenGLTextureBuffer.h"

#include <unordered_map>

using namespace Renderer;
using namespace Renderer::OpenGL;

namespace {
	std::unordered_map<TextureFormat, GLenum> formats{
		{ TextureFormat::RGB8, GL_RGB },
		{ TextureFormat::RGBA8, GL_RGBA},
		{ TextureFormat::DEPTH, GL_DEPTH_COMPONENT },
		{ TextureFormat::GREYSCALE, GL_RED }
	};

	std::unordered_map<TextureFormat, GLenum> internalFormats {
		{ TextureFormat::RGB8, GL_RGB8 },
		{ TextureFormat::RGBA8, GL_RGBA8 },
		{ TextureFormat::DEPTH, GL_DEPTH_COMPONENT24 },
		{ TextureFormat::GREYSCALE, GL_RGB }
	};
}


OpenGLTextureBuffer::OpenGLTextureBuffer() : m_handle(0)
{
}

OpenGLTextureBuffer::OpenGLTextureBuffer(const glm::ivec2& dimensions, TextureFormat format, const std::vector<uint8_t>& data)
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


OpenGLTextureBuffer::~OpenGLTextureBuffer()
{
	glDeleteTextures(1, &m_handle);
}

GLuint OpenGLTextureBuffer::handle()
{
	return m_handle;
}
