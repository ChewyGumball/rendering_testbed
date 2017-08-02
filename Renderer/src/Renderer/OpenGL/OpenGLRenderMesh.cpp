#include "Renderer/OpenGL/OpenGLRenderMesh.h"

#include <GL/glew.h>

using namespace Renderer;
using namespace Renderer::OpenGL;

OpenGLRenderMesh::OpenGLRenderMesh(): m_vbo(0), m_ebo(0), m_indexCount(0), m_format(VertexFormats::Unknown)
{
}

OpenGLRenderMesh::OpenGLRenderMesh(VertexFormat format, std::vector<float>& vertexData, std::vector<uint32_t>& indices) 
	: m_indexCount(static_cast<uint32_t>(indices.size())), m_format(format)
{

	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/*
	glCreateBuffers(1, &m_vbo);
	glCreateBuffers(1, &m_ebo);

	glNamedBufferData(m_vbo, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
	glNamedBufferData(m_ebo, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
	*/
}

OpenGLRenderMesh::~OpenGLRenderMesh()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

uint32_t OpenGLRenderMesh::indexCount() const
{
	return m_indexCount;
}

Renderer::VertexFormat OpenGLRenderMesh::vertexFormat() const
{
	return m_format;
}

GLuint OpenGLRenderMesh::vbo() const
{
	return m_vbo;
}

GLuint OpenGLRenderMesh::ebo() const
{
	return m_ebo;
}
