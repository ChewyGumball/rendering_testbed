#include "Renderer/LayerPass.h"
#include <Renderer\OpenGL\OpenGLRenderer.h>

namespace {
	void setBuffer(GLuint &frameBuffer, GLenum attatchment, std::shared_ptr<TextureBuffer> buffer)
	{
		if (frameBuffer == 0)
		{
			glGenFramebuffers(1, &frameBuffer);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attatchment, GL_TEXTURE_2D, buffer->id(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}


LayerPass::LayerPass() : framebuffer(0), renderer(new OpenGLRenderer())
{
}


LayerPass::~LayerPass()
{
	if (framebuffer != 0)
	{
		glDeleteFramebuffers(1, &framebuffer);
	}

	delete renderer;
}

void LayerPass::draw(const Camera & c)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	renderer->draw(c);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LayerPass::addModelInstance(std::shared_ptr<const ModelInstance> modelInstance)
{
	renderer->addModelInstance(modelInstance);
}

void LayerPass::addPointLight(PointLight light)
{
	renderer->addPointLight(light);
}

void LayerPass::removeModelInstance(std::shared_ptr<const ModelInstance> modelInstance)
{
	renderer->removeModelInstance(modelInstance);
}

void LayerPass::clearBuffers(GLuint buffers)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClear(buffers);
}

void LayerPass::setColourBuffer(std::shared_ptr<TextureBuffer> buffer)
{
	setBuffer(framebuffer, GL_COLOR_ATTACHMENT0, buffer);
}

void LayerPass::setDepthBuffer(std::shared_ptr<TextureBuffer> buffer)
{
	setBuffer(framebuffer, GL_DEPTH_ATTACHMENT, buffer);
}


