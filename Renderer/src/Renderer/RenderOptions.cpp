#include "Renderer/RenderOptions.h"


namespace Renderer {

	RenderOptions::RenderOptions() :frameBuffer(std::make_shared<FrameBuffer>()), viewportOrigin(0, 0), viewportDimensions(800, 800), clearColour(glm::vec4(0, 0, 0, 1)), clearBuffers(true), wireframe(false), depthTest(true)
	{
	}

	RenderOptions::RenderOptions(std::shared_ptr<FrameBuffer> frameBuffer, glm::ivec2 viewportOrigin, glm::ivec2 viewportDimensions, glm::vec4 clearColour, bool clearBuffers, bool wireframe, bool depthTest)
		: frameBuffer(frameBuffer), viewportOrigin(viewportOrigin), viewportDimensions(viewportDimensions), clearColour(clearColour), clearBuffers(clearBuffers), wireframe(wireframe), depthTest(depthTest)
	{
	}
}
