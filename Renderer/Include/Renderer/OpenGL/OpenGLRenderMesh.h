#pragma once
#include <GL\glew.h>
#include <memory>
#include <Resources\Mesh.h>

class OpenGLRenderMesh
{
protected:
	uint32_t m_indexCount;
	VertexFormat m_format;
	GLuint m_vbo, m_ebo;
public:
	OpenGLRenderMesh();
	OpenGLRenderMesh(std::shared_ptr<const Mesh> mesh);
	~OpenGLRenderMesh();

	GLuint vbo() const;
	GLuint ebo() const;
	uint32_t indexCount() const;
	VertexFormat vertexFormat() const;

};

