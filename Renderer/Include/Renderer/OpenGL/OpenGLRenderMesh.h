#pragma once
#include <GL\glew.h>
#include <memory>
#include <Models\Mesh.h>

class OpenGLRenderMesh
{
protected:
	int m_indexCount;
	VertexFormat m_format;
	GLuint m_vbo, m_ebo;
public:
	OpenGLRenderMesh();
	OpenGLRenderMesh(std::shared_ptr<const Mesh> mesh);
	~OpenGLRenderMesh();

	GLuint vbo() const;
	GLuint ebo() const;
	int indexCount() const;
	VertexFormat vertexFormat() const;

};

