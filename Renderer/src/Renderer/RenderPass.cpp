#include "Renderer/RenderPass.h"
#include <Renderer\OpenGL\OpenGLRenderer.h>

RenderPass::RenderPass() : m_frameBuffer(std::make_shared<FrameBuffer>()), renderer(new OpenGLRenderer()), m_camera(std::make_shared<Camera>()), options(RenderOptions()) {}

RenderPass::~RenderPass()
{
    delete renderer;
}

void RenderPass::draw()
{
    renderer->draw(m_frameBuffer, m_camera, options);
}

void RenderPass::clearColour(glm::vec4 colour)
{
	options.clearColour = colour;
}

void RenderPass::cull(bool enabled)
{
	options.cullingEnabled = enabled;
}

void RenderPass::wireframe(bool enabled)
{
	options.wireframe = enabled;
}

void RenderPass::camera(std::shared_ptr<Camera> c)
{
	m_camera = c;
}

void RenderPass::frameBuffer(std::shared_ptr<FrameBuffer> buffer)
{
	m_frameBuffer = buffer;
	renderer->addFrameBuffer(m_frameBuffer);
}

void RenderPass::addModelInstance(std::shared_ptr<const ModelInstance> modelInstance) { renderer->addModelInstance(modelInstance); }

void RenderPass::addPointLight(PointLight light) { renderer->addPointLight(light); }

void RenderPass::removeModelInstance(std::shared_ptr<const ModelInstance> modelInstance) { renderer->removeModelInstance(modelInstance); }


void RenderPass::viewport(glm::vec2 origin, glm::vec2 size)
{	
	options.viewportOrigin = origin;
	options.viewportDimensions = size;
}

uint64_t RenderPass::trianglesDrawn() const { return renderer->trianglesDrawn; }
