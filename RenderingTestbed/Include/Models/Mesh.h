#pragma once
#include <vector>
#include "Vertex.h"

class Mesh
{
protected:
	std::size_t m_id;
	std::vector<Vertex> vertices;
	std::vector<int> indices;
public:
	Mesh(std::vector<Vertex> vertices, std::vector<int> indices);
	~Mesh();

	std::vector<float> rawVertexData() const;
	const std::vector<int>& indexData() const;
	const Vertex vertex() const;
	std::size_t id() const;
};


