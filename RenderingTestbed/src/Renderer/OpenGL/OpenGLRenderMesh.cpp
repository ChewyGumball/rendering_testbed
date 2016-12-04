#include "Renderer/OpenGL/OpenGLRenderMesh.h"

OpenGLRenderMesh::OpenGLRenderMesh()
{
}

OpenGLRenderMesh::OpenGLRenderMesh(std::shared_ptr<const Mesh> mesh) : m_mesh(mesh)
{
	const std::vector<float>& vertexData = mesh->rawVertexData();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

	const VertexFormat format = mesh->vertexFormat();
	int nextVertexAttribute = 0;

	if (format.hasPosition())
	{
		glEnableVertexAttribArray(nextVertexAttribute);
		glVertexAttribPointer(nextVertexAttribute++, 3, GL_FLOAT, GL_FALSE, format.size(), (void*)format.positionOffset());
	}

	if (format.hasNormal())
	{
		glEnableVertexAttribArray(nextVertexAttribute);
		glVertexAttribPointer(nextVertexAttribute++, 3, GL_FLOAT, GL_FALSE, format.size(), (void*)format.normalOffset());
	}

	if (format.hasTextureCoordinates())
	{
		glEnableVertexAttribArray(nextVertexAttribute);
		glVertexAttribPointer(nextVertexAttribute++, 2, GL_FLOAT, GL_FALSE, format.size(), (void*)format.textureCoordinatesOffset());
	}

	if (format.hasColour())
	{
		glEnableVertexAttribArray(nextVertexAttribute);
		glVertexAttribPointer(nextVertexAttribute++, 4, GL_FLOAT, GL_FALSE, format.size(), (void*)format.colourOffset());
	}

	transformAttributeStart = nextVertexAttribute;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indexData().size() * sizeof(int), mesh->indexData().data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

OpenGLRenderMesh::~OpenGLRenderMesh()
{
}

std::shared_ptr<const Mesh> OpenGLRenderMesh::mesh() const
{
	return m_mesh;
}

void OpenGLRenderMesh::draw(int instanceCount) const
{
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, m_mesh->indexData().size(), GL_UNSIGNED_INT, 0, instanceCount);
	glBindVertexArray(0);
}

//Need to instance VAO so different models can bind their mesh transforms to different buffers
void OpenGLRenderMesh::setupTransformAttributes() const
{
	glBindVertexArray(VAO);
	GLsizei vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(transformAttributeStart);
	glVertexAttribPointer(transformAttributeStart, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)0);
	glEnableVertexAttribArray(transformAttributeStart + 1);
	glVertexAttribPointer(transformAttributeStart + 1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(vec4Size));
	glEnableVertexAttribArray(transformAttributeStart + 2);
	glVertexAttribPointer(transformAttributeStart + 2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * vec4Size));
	glEnableVertexAttribArray(transformAttributeStart + 3);
	glVertexAttribPointer(transformAttributeStart + 3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * vec4Size));

	glVertexAttribDivisor(transformAttributeStart, 1);
	glVertexAttribDivisor(transformAttributeStart + 1, 1);
	glVertexAttribDivisor(transformAttributeStart + 2, 1);
	glVertexAttribDivisor(transformAttributeStart + 3, 1);
	glBindVertexArray(0);
}
