#include "Renderer/OpenGL/OpenGLFrameBuffer.h"
#include <Renderer\OpenGL\OpenGLTextureBuffer.h>

namespace {
	std::unordered_map<FrameBufferTarget, GLenum> attachments = {
		{ FrameBufferTarget::COLOUR, GL_COLOR_ATTACHMENT0 },
		{ FrameBufferTarget::DEPTH, GL_DEPTH_ATTACHMENT }
	};
}


OpenGLFrameBuffer::OpenGLFrameBuffer() : buffer(0)
{
}

OpenGLFrameBuffer::OpenGLFrameBuffer(std::shared_ptr<const FrameBuffer> frameBuffer, std::unordered_map<uint32_t, OpenGLTextureBuffer>& textures) : buffer(0)
{
	if (frameBuffer->targets().size() > 0) {
		glGenFramebuffers(1, &buffer);

		glBindFramebuffer(GL_FRAMEBUFFER, buffer);
		for (auto& target : frameBuffer->targets())
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[target.first], GL_TEXTURE_2D, textures[target.second->id()].handle(), 0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}


OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
}

void OpenGLFrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
}

void OpenGLFrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
