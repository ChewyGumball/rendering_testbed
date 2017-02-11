#pragma once
#include <stdint.h>
class RenderResource
{
protected:
	uint32_t m_id;
public:
	RenderResource();
	RenderResource(uint32_t id);
	uint32_t id() const;
};

