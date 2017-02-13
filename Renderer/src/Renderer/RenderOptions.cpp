#include "Renderer/RenderOptions.h"



RenderOptions::RenderOptions() :frameBuffer(std::make_shared<FrameBuffer>()), viewportOrigin(0,0), viewportDimensions(800,800), wireframe(false)
{
}

RenderOptions::RenderOptions(std::shared_ptr<FrameBuffer> frameBuffer, glm::ivec2 viewportOrigin, glm::ivec2 viewportDimensions, bool wireframe)
	:frameBuffer(frameBuffer), viewportOrigin(viewportOrigin), viewportDimensions(viewportDimensions), wireframe(wireframe)
{
}
