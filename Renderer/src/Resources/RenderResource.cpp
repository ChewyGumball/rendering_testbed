#include "Resources/RenderResource.h"

#include <atomic>

namespace {
	std::atomic<RenderResourceID> nextID = 1;
}


RenderResource::RenderResource() : m_id(nextID++)
{
}

RenderResource::RenderResource(RenderResourceID id) : m_id(id)
{
}

RenderResourceID RenderResource::id() const
{
	return m_id;
}
