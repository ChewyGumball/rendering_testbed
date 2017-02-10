#include "Renderer/RenderPass.h"
#include <Renderer\OpenGL\OpenGLRenderer.h>

namespace {
void setBuffer(GLuint& frameBuffer, GLenum attatchment, std::shared_ptr<TextureBuffer> buffer)
{
    if (frameBuffer == 0) {
        glGenFramebuffers(1, &frameBuffer);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attatchment, GL_TEXTURE_2D, buffer->id(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
}

RenderPass::RenderPass() : framebuffer(0), renderer(new OpenGLRenderer()), m_camera(std::make_shared<Camera>()), culllingEnabled(true), m_wireframe(false) {}

RenderPass::~RenderPass()
{
    if (framebuffer != 0) {
        glDeleteFramebuffers(1, &framebuffer);
    }

    delete renderer;
}

void RenderPass::draw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glViewport(viewportOrigin.x, viewportOrigin.y, viewportSize.x, viewportSize.y);
	glClearColor(m_clearColour.r, m_clearColour.g, m_clearColour.b, m_clearColour.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_wireframe)
	{
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
	}

    renderer->draw(m_camera, culllingEnabled);

	if (m_wireframe)
	{
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
	}

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass::clearColour(glm::vec4 colour)
{
	m_clearColour = colour;
}

void RenderPass::cull(bool enabled)
{
	culllingEnabled = enabled;
}

void RenderPass::wireframe(bool enabled)
{
	m_wireframe = enabled;
}

void RenderPass::camera(std::shared_ptr<Camera> c)
{
	m_camera = c;
}

void RenderPass::addModelInstance(std::shared_ptr<const ModelInstance> modelInstance) { renderer->addModelInstance(modelInstance); }

void RenderPass::addPointLight(PointLight light) { renderer->addPointLight(light); }

void RenderPass::removeModelInstance(std::shared_ptr<const ModelInstance> modelInstance) { renderer->removeModelInstance(modelInstance); }

void RenderPass::clearBuffers(GLuint buffers)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClear(buffers);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass::setColourBuffer(std::shared_ptr<TextureBuffer> buffer) { setBuffer(framebuffer, GL_COLOR_ATTACHMENT0, buffer); viewport(glm::vec2(), buffer->dimensions()); }

void RenderPass::setDepthBuffer(std::shared_ptr<TextureBuffer> buffer) { setBuffer(framebuffer, GL_DEPTH_ATTACHMENT, buffer); }

void RenderPass::viewport(glm::vec2 origin, glm::vec2 size)
{
	viewportOrigin = origin;
	viewportSize = size;
}

uint64_t RenderPass::trianglesDrawn() const { return renderer->trianglesDrawn; }
