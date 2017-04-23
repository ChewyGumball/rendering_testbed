#include "Resources/RenderResource.h"

#include <atomic>

namespace {
	std::atomic<Renderer::RenderResourceID> nextID = 1;
}

namespace Renderer {

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
}
