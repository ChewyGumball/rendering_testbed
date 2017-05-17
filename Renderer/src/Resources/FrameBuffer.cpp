#include "Resources/FrameBuffer.h"
#include <Resources/RenderResourceManagement.h>

namespace Renderer {
	FrameBuffer::FrameBuffer(std::unordered_map<FrameBufferTarget, std::shared_ptr<TextureBuffer>> targets) : m_targets(targets)
	{
		RenderResourceManagement::createFrameBuffer(m_id, targets);
	}
	FrameBuffer::~FrameBuffer() { }

	const std::unordered_map<FrameBufferTarget, std::shared_ptr<TextureBuffer>>& FrameBuffer::targets() const
	{
		return m_targets;
	}
}
