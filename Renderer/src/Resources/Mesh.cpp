#include "Resources/Mesh.h"

#include <limits>

#include <glm/common.hpp>
#include <Resources/RenderResourceManagement.h>

namespace {
	Renderer::Culling::BoundingSphere calculateBoundingSphere(Renderer::VertexFormat& format, std::vector<float>& verts)
	{
		float infinity = std::numeric_limits<float>::infinity();
		glm::vec3 max(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
		glm::vec3 min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		for (int i = format.positionOffset(); i < verts.size(); i += format.size())
		{
			glm::vec3 vertex(verts[i], verts[i + 1], verts[i + 2]);
			if (vertex.x == -infinity || vertex.y == -infinity || vertex.z == -infinity) continue;
			max = glm::max(max, vertex);
			min = glm::min(min, vertex);
		}

		return Renderer::Culling::BoundingSphere(glm::vec3((max.x + min.x) / 2, (max.y + min.y) / 2, (max.z + min.z) / 2), glm::distance(min, max) / 2);
	}
}

namespace Renderer {
	Mesh::Mesh(VertexFormat format, std::vector<float>& vertexData, std::vector<uint32_t>& indices)
		: m_triangleCount(indices.size() / 3), boundingSphere(calculateBoundingSphere(format, vertexData))
	{
		RenderResourceManagement::createMesh(m_id, format, vertexData, indices);
	}

	Mesh::~Mesh()
	{
		RenderResourceManagement::destroyMesh();
	}


	Culling::BoundingSphere Mesh::bounds() const
	{
		return boundingSphere;
	}

	uint64_t Mesh::triangleCount() const
	{
		return m_triangleCount;
	}
}
