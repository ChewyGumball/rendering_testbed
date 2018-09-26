#pragma once
#include <glm/vec2.hpp>
#include <Resources\RenderResource.h>
#include <string>
#include <vector>
namespace Renderer::Backend {

	enum struct TextureFormat {
		RGB8,
		RGBA8,
		DEPTH,
		GREYSCALE,
		GREYSCALE_ALPHA
	};

	class Texture : public RenderResource
	{
	private: 
		TextureFormat m_format;
	public:
		Texture(glm::ivec2 dimensions, TextureFormat format, const std::vector<uint8_t>& data = std::vector<uint8_t>());
		Texture(glm::ivec2 dimensions, TextureFormat format, std::vector<uint8_t>&& data);
		Texture(const std::string& filename);
		~Texture();

		TextureFormat format();
	};
}
