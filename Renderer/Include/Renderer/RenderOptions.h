#pragma once
#include <glm\vec2.hpp>
#include <glm\vec4.hpp>
struct RenderOptions
{
	glm::vec2 viewportOrigin;
	glm::vec2 viewportDimensions;
	glm::vec4 clearColour;
	bool clearBuffers;
	bool wireframe;
	bool cullingEnabled;

	RenderOptions();
	RenderOptions(glm::vec2 viewportOrigin, glm::vec2 viewportDimensions, glm::vec4 clearColour, bool clearBuffers, bool wireframe, bool cullingEnabled);
};

