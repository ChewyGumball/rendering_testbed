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

RenderPass::RenderPass() : framebuffer(0), renderer(new OpenGLRenderer()) {}

RenderPass::~RenderPass()
{
    if (framebuffer != 0) {
        glDeleteFramebuffers(1, &framebuffer);
    }

    delete renderer;
}

void RenderPass::draw(const Camera& c, bool doFrustumCulling)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    renderer->draw(c, doFrustumCulling);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

void RenderPass::setColourBuffer(std::shared_ptr<TextureBuffer> buffer) { setBuffer(framebuffer, GL_COLOR_ATTACHMENT0, buffer); }

void RenderPass::setDepthBuffer(std::shared_ptr<TextureBuffer> buffer) { setBuffer(framebuffer, GL_DEPTH_ATTACHMENT, buffer); }

uint64_t RenderPass::trianglesDrawn() const { return renderer->trianglesDrawn; }
