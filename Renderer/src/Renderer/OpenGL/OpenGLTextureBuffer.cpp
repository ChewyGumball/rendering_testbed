#include "Renderer/OpenGL/OpenGLTextureBuffer.h"

#include <unordered_map>

namespace {
	std::unordered_map<TextureFormat, GLenum> formats{
		{ TextureFormat::RGB8, GL_RGB },
		{ TextureFormat::DEPTH, GL_DEPTH_COMPONENT }
	};

	std::unordered_map<TextureFormat, GLenum> internalFormats {
		{ TextureFormat::RGB8, GL_RGB8 },
		{ TextureFormat::DEPTH, GL_DEPTH_COMPONENT24 }
	};
}


OpenGLTextureBuffer::OpenGLTextureBuffer() : m_handle(0)
{
}

OpenGLTextureBuffer::OpenGLTextureBuffer(std::shared_ptr<TextureBuffer> texture)
{
	glm::ivec2 dimensions = texture->dimensions();

	glGenTextures(1, &m_handle);
	glBindTexture(GL_TEXTURE_2D, m_handle);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormats[texture->format()], dimensions.x, dimensions.y, 0, formats[texture->format()], GL_UNSIGNED_BYTE, texture->data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
