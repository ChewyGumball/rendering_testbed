#pragma once
#include <GL\glew.h>
#include <glm/vec2.hpp>
class TextureBuffer
{
	int m_width;
	int m_height;
	GLuint handle;

public:
	TextureBuffer(int width, int height, GLenum format, GLenum internalFormat, void* data = nullptr);
	~TextureBuffer();

	GLuint id() const;
	glm::vec2 dimensions() const;
};

