#pragma once
#include <memory>
#include <glm\vec2.hpp>
#include <glm\vec4.hpp>

#include <Renderer\FrameBuffer.h>

struct RenderOptions
{
	std::shared_ptr<FrameBuffer> frameBuffer;
	glm::vec2 viewportOrigin;
	glm::vec2 viewportDimensions;
	bool wireframe;

	RenderOptions();
	RenderOptions(std::shared_ptr<FrameBuffer> frameBuffer, glm::vec2 viewportOrigin, glm::vec2 viewportDimensions, bool wireframe);
};

