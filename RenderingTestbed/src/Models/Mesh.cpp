#include "Models/Mesh.h"

#include <limits>

#include <glm/common.hpp>

namespace {
	std::size_t nextID = 0;
	Culling::BoundingSphere calculateBoundingSphere(VertexFormat& format, std::vector<float>& verts)
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

		return Culling::BoundingSphere(glm::vec3((max.x + min.x) / 2, (max.y + min.y) / 2, (max.z + min.z) / 2), glm::distance(min, max) / 2);
	}
}

Mesh::Mesh(VertexFormat format, std::vector<float> vertices, std::vector<int> indices) : format(format), m_id(nextID++), m_vertices(vertices), indices(indices), boundingSphere(calculateBoundingSphere(format, vertices))
{
}

Mesh::~Mesh()
{
}

const std::vector<float>& Mesh::vertexData() const
{
	return m_vertices;
}

const std::vector<int>& Mesh::indexData() const
{
	return indices;
}

const VertexFormat Mesh::vertexFormat() const
{
	return format;
}

std::size_t Mesh::id() const
{
	return m_id;
}

Culling::BoundingSphere Mesh::bounds() const
{
	return boundingSphere;
}

uint64_t Mesh::triangleCount() const
{
	return indices.size() / 3;
}
