#pragma once
#include <memory>
#include <glm\vec2.hpp>
#include <glm\vec4.hpp>

#include <Resources\FrameBuffer.h>

struct RenderOptions
{
	std::shared_ptr<FrameBuffer> frameBuffer;
	glm::ivec2 viewportOrigin;
	glm::ivec2 viewportDimensions;
	glm::vec4 clearColour;
	bool clearBuffers;
	bool wireframe;
	bool depthTest;

	RenderOptions();
	RenderOptions(std::shared_ptr<FrameBuffer> frameBuffer, glm::ivec2 viewportOrigin, glm::ivec2 viewportDimensions, glm::vec4 clearColour, bool clearBuffers, bool wireframe, bool depthTest);
};

