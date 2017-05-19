#pragma warning(disable:4996) //MSVC does not like std::copy with bare pointers
#include "Resources/TextureBuffer.h"

#include <Resources/RenderResourceManagement.h>

#include <Util/FileUtils.h>

#define STB_IMAGE_IMPLEMENTATION
#include <lib/stb_image/stb_image.h>

namespace {
	std::unordered_map<int, Renderer::TextureFormat> stbTextureFormats = {
		{1, Renderer::TextureFormat::GREYSCALE},
		{2, Renderer::TextureFormat::GREYSCALE_ALPHA},
		{3, Renderer::TextureFormat::RGB8},
		{4, Renderer::TextureFormat::RGBA8 }
	};
}

namespace Renderer {
	TextureBuffer::TextureBuffer(glm::ivec2 dimensions, TextureFormat format, std::vector<uint8_t> data)
	{
		RenderResourceManagement::createTexture(m_id, dimensions, format, data);
	}

	TextureBuffer::TextureBuffer(const std::string & filename)
	{
		stbi_set_flip_vertically_on_load(true);
		std::vector<uint8_t> data = Util::File::ReadBinary(filename);
		int x, y, channels;
		uint8_t* image = stbi_load_from_memory(data.data(), static_cast<int>(data.size()), &x, &y, &channels, 0);

		std::vector<uint8_t> imageAsVector(x * y * channels);

		std::copy(image, image + x * y * channels, imageAsVector.data());

		RenderResourceManagement::createTexture(m_id, glm::ivec2(x, y), stbTextureFormats[channels], std::move(imageAsVector));
	}

	TextureBuffer::~TextureBuffer()
	{
	}
}
