#pragma once
#include <glm/vec2.hpp>
#include <Renderer\RenderResource.h>
#include <string>

enum struct TextureFormat {
	RGB8,
	DEPTH
};

class TextureBuffer : public RenderResource
{
	glm::ivec2 m_dimensions;
	TextureFormat m_format;
	std::string m_filename;
	bool fileTexture;

public:
	TextureBuffer(glm::ivec2 dimensions, TextureFormat format);
	TextureBuffer(glm::ivec2 dimensions, TextureFormat format, std::string filename);
	~TextureBuffer();

	const std::string& filename() const;
	bool isFileTexture() const;
	void* data() const;
	TextureFormat format() const;
	glm::ivec2 dimensions() const;
};

