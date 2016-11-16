#include "Models/Mesh.h"

namespace {
	std::size_t nextID = 0;
}

Mesh::Mesh(VertexFormat format, std::vector<float> vertices, std::vector<int> indices) : format(format), m_id(nextID++), m_vertices(vertices), indices(indices)
{
}

Mesh::~Mesh()
{
}

const std::vector<float>& Mesh::rawVertexData() const
{
	return m_vertices;
}

const const std::vector<int>& Mesh::indexData() const
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
