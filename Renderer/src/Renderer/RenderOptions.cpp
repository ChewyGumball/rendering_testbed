#include "Renderer/RenderOptions.h"



RenderOptions::RenderOptions() : viewportOrigin(0,0), viewportDimensions(800,800), clearColour(0,0,0,1.0), clearBuffers(true), wireframe(false), cullingEnabled(false)
{
}

RenderOptions::RenderOptions(glm::vec2 viewportOrigin, glm::vec2 viewportDimensions, glm::vec4 clearColour, bool clearBuffers, bool wireframe, bool cullingEnabled)
	:viewportOrigin(viewportOrigin), viewportDimensions(viewportDimensions), clearColour(clearColour), wireframe(wireframe), cullingEnabled(cullingEnabled)
{
}
