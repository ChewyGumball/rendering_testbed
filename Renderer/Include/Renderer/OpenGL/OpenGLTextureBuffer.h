#pragma once
#include <GL\glew.h>
#include <memory>
#include <Renderer\TextureBuffer.h>

class OpenGLTextureBuffer
{
private:
	GLuint m_handle;
public:
	OpenGLTextureBuffer();
	OpenGLTextureBuffer(std::shared_ptr<TextureBuffer> texture);
	~OpenGLTextureBuffer();

	GLuint handle();
};

