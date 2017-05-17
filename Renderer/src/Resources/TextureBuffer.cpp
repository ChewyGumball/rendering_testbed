#include "Resources/TextureBuffer.h"

#include <Resources/RenderResourceManagement.h>

namespace Renderer {
	TextureBuffer::TextureBuffer(glm::ivec2 dimensions, TextureFormat format, std::vector<uint8_t> data)
	{
		RenderResourceManagement::createTexture(m_id, dimensions, format, data);
	}

	TextureBuffer::~TextureBuffer()
	{
	}
}
