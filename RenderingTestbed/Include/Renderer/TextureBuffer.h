#pragma once
#include <GL\glew.h>
class TextureBuffer
{
	int width;
	int height;
	GLuint handle;

public:
	TextureBuffer(int width, int height, GLenum format, GLenum internalFormat, void* data = nullptr);
	~TextureBuffer();

	GLuint id();
};

