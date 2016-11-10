#pragma once
#include <GL\glew.h>
#include <memory>
#include <Models\Mesh.h>

class OpenGLRenderMesh
{
protected:
	int transformAttributeStart;
	GLuint VAO, VBO, EBO;
	std::shared_ptr<const Mesh> m_mesh;
public:
	OpenGLRenderMesh();
	OpenGLRenderMesh(std::shared_ptr<const Mesh> mesh);
	~OpenGLRenderMesh();

	std::shared_ptr<const Mesh> mesh() const;

	void draw(int instanceCount) const;

	void setupTransformAttributes() const;
};

