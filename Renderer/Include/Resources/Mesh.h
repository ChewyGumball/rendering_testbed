#pragma once
#include <vector>
#include <Resources\RenderResource.h>
#include "Resources/Vertex.h"
#include "Culling/BoundingSphere.h"

namespace Renderer {
	class Mesh : public RenderResource
	{
	protected:
		uint64_t m_triangleCount;

		Culling::BoundingSphere boundingSphere;
	public:
		Mesh(VertexFormat format, const std::vector<float>& vertexData, const std::vector<uint32_t>& indices);
		Mesh(VertexFormat format, std::vector<float>&& vertexData, std::vector<uint32_t>&& indices);
		~Mesh();

		Culling::BoundingSphere bounds() const;

		uint64_t triangleCount() const;
	};
}

