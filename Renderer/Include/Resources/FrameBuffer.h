#pragma once
#include <unordered_map>
#include <memory>
#include <Resources/RenderResource.h>
#include <Resources\TextureBuffer.h>

enum struct FrameBufferTarget {
	COLOUR0,
	COLOUR1,
	COLOUR2,
	COLOUR3,
	COLOUR4,
	COLOUR5,
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

