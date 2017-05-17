#pragma once
#include <glm/vec2.hpp>
#include <Resources\RenderResource.h>
#include <string>
#include <vector>
namespace Renderer {

	enum struct TextureFormat {
		RGB8,
		DEPTH,
		GREYSCALE
	};

	class TextureBuffer : public RenderResource
	{
	public:
		TextureBuffer(glm::ivec2 dimensions, TextureFormat format, std::vector<uint8_t> data = std::vector<uint8_t>());
		~TextureBuffer();
	};
}
