#include "Resources/FrameBuffer.h"

namespace Renderer {
	FrameBuffer::FrameBuffer() : RenderResource(0) { }
	FrameBuffer::FrameBuffer(std::unordered_map<FrameBufferTarget, std::shared_ptr<TextureBuffer>> targets) : m_targets(targets) { }
	FrameBuffer::~FrameBuffer() { }

	const std::unordered_map<FrameBufferTarget, std::shared_ptr<TextureBuffer>>& FrameBuffer::targets() const
	{
		return m_targets;
	}
}
