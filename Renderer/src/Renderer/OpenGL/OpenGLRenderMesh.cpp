#include "Renderer/OpenGL/OpenGLRenderMesh.h"

namespace {
	GLuint tempVAO = -1;
}

OpenGLRenderMesh::OpenGLRenderMesh(): m_vbo(0), m_ebo(0), m_indexCount(0), m_format(VertexFormats::Unknown)
{
}

OpenGLRenderMesh::OpenGLRenderMesh(std::shared_ptr<const Mesh> mesh) : m_indexCount(static_cast<uint32_t>(mesh->indexData().size())), m_format(mesh->vertexFormat())
{
	const std::vector<float>& vertexData = mesh->vertexData();
	const std::vector<int>& indexData = mesh->indexData();

	glCreateBuffers(1, &m_vbo);
	glCreateBuffers(1, &m_ebo);

	glNamedBufferData(m_vbo, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
	glNamedBufferData(m_ebo, indexData.size() * sizeof(int), indexData.data(), GL_STATIC_DRAW);
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

VertexFormat OpenGLRenderMesh::vertexFormat() const
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
