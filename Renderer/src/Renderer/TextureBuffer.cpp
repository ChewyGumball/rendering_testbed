#include "Renderer/TextureBuffer.h"

TextureBuffer::TextureBuffer(glm::vec2 dimensions, TextureFormat format, void * data) : m_dimensions(dimensions), m_format(format), m_data(data)
{
}

TextureBuffer::~TextureBuffer()
{
}

void* const TextureBuffer::data() const {
	return m_data;
}

TextureFormat TextureBuffer::format() const
{
	return m_format;
}

glm::vec2 TextureBuffer::dimensions() const
{
	return m_dimensions;
}
