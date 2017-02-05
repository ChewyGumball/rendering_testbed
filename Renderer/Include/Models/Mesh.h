#pragma once
#include <vector>
#include "Vertex.h"
#include "Culling/BoundingSphere.h"

class Mesh
{
protected:
	VertexFormat format;
	std::size_t m_id;
	std::vector<float> m_vertices;
	std::vector<int> indices;
	Culling::BoundingSphere boundingSphere;
public:
	Mesh(VertexFormat format, std::vector<float> vertexData, std::vector<int> indices);
	~Mesh();

	const std::vector<float>& vertexData() const;
	const std::vector<int>& indexData() const;
	const VertexFormat vertexFormat() const;
	std::size_t id() const;
	Culling::BoundingSphere bounds() const;

	uint64_t triangleCount() const;
};


