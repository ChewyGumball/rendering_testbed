#pragma once
#include <unordered_map>
#include <memory>
#include <Renderer/RenderResource.h>
#include <Renderer\TextureBuffer.h>

enum struct FrameBufferTarget {
	COLOUR,
	DEPTH
};

class FrameBuffer : public RenderResource
{
private:
	std::unordered_map<FrameBufferTarget, std::shared_ptr<TextureBuffer>> m_targets;
public:
	FrameBuffer();
	FrameBuffer(std::unordered_map<FrameBufferTarget, std::shared_ptr<TextureBuffer>> targets);
	~FrameBuffer();

	const std::unordered_map<FrameBufferTarget, std::shared_ptr<TextureBuffer>>& targets() const;
};

