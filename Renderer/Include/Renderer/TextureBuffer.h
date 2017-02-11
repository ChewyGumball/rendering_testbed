#pragma once
#include <glm/vec2.hpp>
#include <Renderer\RenderResource.h>

enum struct TextureFormat {
	RGB8,
	DEPTH
};

class TextureBuffer : public RenderResource
{
	glm::vec2 m_dimensions;
	TextureFormat m_format;
	void* m_data;
public:
	TextureBuffer(glm::vec2 dimensions, TextureFormat format, void* data = nullptr);
	~TextureBuffer();

	void* const data() const;
	TextureFormat format() const;
	glm::vec2 dimensions() const;
};

