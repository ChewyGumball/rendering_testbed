#include "Renderer/RenderOptions.h"



RenderOptions::RenderOptions() :frameBuffer(std::make_shared<FrameBuffer>()), viewportOrigin(0,0), viewportDimensions(800,800), wireframe(false)
{
}

RenderOptions::RenderOptions(std::shared_ptr<FrameBuffer> frameBuffer, glm::vec2 viewportOrigin, glm::vec2 viewportDimensions, bool wireframe)
	:frameBuffer(frameBuffer), viewportOrigin(viewportOrigin), viewportDimensions(viewportDimensions), wireframe(wireframe)
{
}
