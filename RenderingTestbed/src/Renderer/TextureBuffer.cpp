#include "Renderer/TextureBuffer.h"


TextureBuffer::TextureBuffer(int width, int height, GLenum format, GLenum internalFormat, void* data): width(width), height(height)
{
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}


TextureBuffer::~TextureBuffer()
{
}

GLuint TextureBuffer::id()
{
	return handle;
}
