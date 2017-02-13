#include "Renderer/TextureBuffer.h"
#include <Util/FileUtils.h>

TextureBuffer::TextureBuffer(glm::ivec2 dimensions, TextureFormat format) : m_dimensions(dimensions), m_format(format), fileTexture(false)
{
}
TextureBuffer::TextureBuffer(glm::ivec2 dimensions, TextureFormat format, std::string name) : m_dimensions(dimensions), m_format(format), m_filename(name), fileTexture(true)
{
}

TextureBuffer::~TextureBuffer()
{
}

const std::string & TextureBuffer::filename() const
{
	return m_filename;
}

bool TextureBuffer::isFileTexture() const
{
	return fileTexture;
}

void * TextureBuffer::data() const
{
	if (fileTexture) {
		return Util::File::ReadBinary(m_filename);
	}
	else {
		return nullptr;
	}
}

TextureFormat TextureBuffer::format() const
{
	return m_format;
}

glm::ivec2 TextureBuffer::dimensions() const
{
	return m_dimensions;
}
