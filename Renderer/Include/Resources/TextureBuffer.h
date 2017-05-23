#pragma once
#include <glm/vec2.hpp>
#include <Resources\RenderResource.h>
#include <string>
#include <vector>
namespace Renderer {

	enum struct TextureFormat {
		RGB8,
		RGBA8,
		DEPTH,
		GREYSCALE,
		GREYSCALE_ALPHA
	};

	class TextureBuffer : public RenderResource
	{
	private: 
		TextureFormat m_format;
	public:
		TextureBuffer(glm::ivec2 dimensions, TextureFormat format, const std::vector<uint8_t>& data = std::vector<uint8_t>());
		TextureBuffer(glm::ivec2 dimensions, TextureFormat format, std::vector<uint8_t>&& data);
		TextureBuffer(const std::string& filename);
		~TextureBuffer();

		TextureFormat format();
	};
}
