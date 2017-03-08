#pragma once

#include <GL\glew.h>
#include <unordered_map>
#include <memory>

#include <Renderer/FrameBuffer.h>
#include <Renderer\OpenGL/OpenGLTextureBuffer.h>

class OpenGLFrameBuffer
{
private:
	GLuint buffer;

public:
	OpenGLFrameBuffer();
	OpenGLFrameBuffer(std::shared_ptr<const FrameBuffer> frameBuffer, std::unordered_map<RenderResourceID, std::shared_ptr<OpenGLTextureBuffer>>& textures);
	~OpenGLFrameBuffer();

	void bind();
	void unbind();
};

