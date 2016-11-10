#include "Models/Mesh.h"

namespace {
	std::size_t nextID = 0;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<int> indices) : m_id(nextID++), vertices(vertices), indices(indices)
{
}

Mesh::~Mesh()
{
}

std::vector<float> Mesh::rawVertexData() const
{
	std::vector<float> data;
	for (const Vertex& v : vertices)
	{
		v.append(data);
	}
	return data;
}

const const std::vector<int>& Mesh::indexData() const
{
	return indices;
}

const Vertex Mesh::vertex() const
{
	return vertices[0];
}

std::size_t Mesh::id() const
{
	return m_id;
}
