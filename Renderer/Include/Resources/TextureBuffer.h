#pragma once
#include <glm/vec2.hpp>
#include <Resources\RenderResource.h>
#include <string>
#include <vector>

enum struct TextureFormat {
	RGB8,
	DEPTH,
	GREYSCALE
};

class TextureBuffer : public RenderResource
{
	glm::ivec2 m_dimensions;
	TextureFormat m_format;
	std::string m_filename; 
	mutable std::vector<uint8_t> m_data;
	bool fileTexture;

public:
	//For textures which can not be hotloaded from a file
	TextureBuffer(glm::ivec2 dimensions, TextureFormat format, std::vector<uint8_t> data = std::vector<uint8_t>());
	//For textures which can be hotloaded from a file
	TextureBuffer(glm::ivec2 dimensions, TextureFormat format, std::string filename);
	~TextureBuffer();

	const std::string& filename() const;
	bool isFileTexture() const;
	void* data() const;
	TextureFormat format() const;
	glm::ivec2 dimensions() const;
};

