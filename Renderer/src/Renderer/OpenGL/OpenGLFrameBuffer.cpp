#include "Renderer/OpenGL/OpenGLFrameBuffer.h"
#include <Renderer\OpenGL\OpenGLTextureBuffer.h>

#include <iostream>

namespace {
	std::unordered_map<FrameBufferTarget, GLenum> attachments = {
		{ FrameBufferTarget::COLOUR0, GL_COLOR_ATTACHMENT0 },
		{ FrameBufferTarget::COLOUR1, GL_COLOR_ATTACHMENT1 },
		{ FrameBufferTarget::COLOUR2, GL_COLOR_ATTACHMENT2 },
		{ FrameBufferTarget::COLOUR3, GL_COLOR_ATTACHMENT3 },
		{ FrameBufferTarget::COLOUR4, GL_COLOR_ATTACHMENT4 },
		{ FrameBufferTarget::COLOUR5, GL_COLOR_ATTACHMENT5 },
		{ FrameBufferTarget::DEPTH, GL_DEPTH_ATTACHMENT }
	};
}


OpenGLFrameBuffer::OpenGLFrameBuffer() : buffer(0)
{
}

OpenGLFrameBuffer::OpenGLFrameBuffer(std::shared_ptr<const FrameBuffer> frameBuffer, std::unordered_map<RenderResourceID, std::shared_ptr<OpenGLTextureBuffer>>& textures) : buffer(0)
{
	if (frameBuffer->targets().size() > 0) {
		glGenFramebuffers(1, &buffer);

		std::vector<GLenum> colourBuffers;

		glBindFramebuffer(GL_FRAMEBUFFER, buffer);
		for (auto& target : frameBuffer->targets())
		{
			glFramebufferTexture(GL_FRAMEBUFFER, attachments[target.first], textures[target.second->id()]->handle(), 0);
			if (target.first != FrameBufferTarget::DEPTH) {
				colourBuffers.push_back(attachments[target.first]);
			}
		}
		glDrawBuffers(static_cast<GLsizei>(colourBuffers.size()), colourBuffers.data());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}


OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
	glDeleteFramebuffers(1, &buffer);
}

void OpenGLFrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
}

void OpenGLFrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
