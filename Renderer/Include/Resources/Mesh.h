#pragma once
#include <vector>
#include <Resources\RenderResource.h>
#include "Resources/Vertex.h"
#include "Culling/BoundingSphere.h"

namespace Renderer {
	class Mesh : public RenderResource
	{
	protected:
		VertexFormat format;
		std::vector<float> m_vertices;
		std::vector<int> indices;
		Culling::BoundingSphere boundingSphere;
	public:
		Mesh(VertexFormat format, std::vector<float> vertexData, std::vector<int> indices);
		~Mesh();

		const std::vector<float>& vertexData() const;
		const std::vector<int>& indexData() const;
		const VertexFormat vertexFormat() const;
		Culling::BoundingSphere bounds() const;

		uint64_t triangleCount() const;
	};
}

