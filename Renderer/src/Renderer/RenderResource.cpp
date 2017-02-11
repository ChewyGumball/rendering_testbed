#include "Renderer/RenderResource.h"

#include <atomic>

namespace {
	std::atomic<uint32_t> nextID = 1;
}


RenderResource::RenderResource() : m_id(nextID++)
{
}

RenderResource::RenderResource(uint32_t id) : m_id(id)
{
}

uint32_t RenderResource::id() const
{
	return m_id;
}
