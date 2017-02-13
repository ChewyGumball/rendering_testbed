#pragma once
#include <stdint.h>
typedef uint32_t RenderResourceID;

class RenderResource
{
protected:
	RenderResourceID m_id;
public:
	RenderResource();
	RenderResource(RenderResourceID id);
	RenderResourceID id() const;
};

