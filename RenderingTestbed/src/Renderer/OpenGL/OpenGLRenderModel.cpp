#include "Renderer/OpenGL/OpenGLRenderModel.h"

namespace {
	std::vector<GLenum> textureUnits{
		GL_TEXTURE0,
		GL_TEXTURE1,
		GL_TEXTURE2,
		GL_TEXTURE3,
		GL_TEXTURE4,
		GL_TEXTURE5,
		GL_TEXTURE6,
		GL_TEXTURE7,
		GL_TEXTURE8
	};

	const GLuint VERTEX_DATA_BINDING_POINT_INDEX = 0;
	const GLuint TRANSFORM_DATA_BINDING_POINT_INDEX = 1;

}

OpenGLRenderModel::OpenGLRenderModel(): shaderID(0)
{
}

OpenGLRenderModel::OpenGLRenderModel(const OpenGLRenderMesh& mesh, std::size_t shaderID, const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures) 
	: shaderID(shaderID), textures(textures), indexCount(mesh.indexCount())
{
	glCreateVertexArrays(1, &vao);
	glCreateBuffers(1, &m_transformVBO);

	GLsizei vec4Size = sizeof(glm::vec4);
	GLuint meshVBO = mesh.vbo();
	const VertexFormat format = mesh.vertexFormat();
	int nextVertexAttribute = 0;

	//Set up mesh vertex data attributes
	glVertexArrayVertexBuffer(vao, VERTEX_DATA_BINDING_POINT_INDEX, meshVBO, 0, format.size());
	if (format.hasPosition())
	{
		glEnableVertexArrayAttrib(vao, nextVertexAttribute);
		glVertexArrayAttribBinding(vao, nextVertexAttribute, VERTEX_DATA_BINDING_POINT_INDEX);
		glVertexArrayAttribFormat(vao, nextVertexAttribute++, 3, GL_FLOAT, GL_FALSE, format.positionOffset());
	}

	if (format.hasNormal())
	{
		glEnableVertexArrayAttrib(vao, nextVertexAttribute);
		glVertexArrayAttribBinding(vao, nextVertexAttribute, VERTEX_DATA_BINDING_POINT_INDEX);
		glVertexArrayAttribFormat(vao, nextVertexAttribute++, 3, GL_FLOAT, GL_FALSE, format.normalOffset());
	}

	if (format.hasTextureCoordinates())
	{
		glEnableVertexArrayAttrib(vao, nextVertexAttribute);
		glVertexArrayAttribBinding(vao, nextVertexAttribute, VERTEX_DATA_BINDING_POINT_INDEX);
		glVertexArrayAttribFormat(vao, nextVertexAttribute++, 2, GL_FLOAT, GL_FALSE, format.textureCoordinatesOffset());
	}

	if (format.hasColour())
	{
		glEnableVertexArrayAttrib(vao, nextVertexAttribute);
		glVertexArrayAttribBinding(vao, nextVertexAttribute, VERTEX_DATA_BINDING_POINT_INDEX);
		glVertexArrayAttribFormat(vao, nextVertexAttribute++, 4, GL_FLOAT, GL_FALSE, format.colourOffset());
	}

	//Set up instancing transform buffer attributes
	glVertexArrayVertexBuffer(vao, TRANSFORM_DATA_BINDING_POINT_INDEX, m_transformVBO, 0, 4 * vec4Size); 
	glVertexArrayBindingDivisor(vao, TRANSFORM_DATA_BINDING_POINT_INDEX, 1);

	for (int i = 0; i < 4; ++i)
	{
		glEnableVertexArrayAttrib(vao, nextVertexAttribute);
		glVertexArrayAttribBinding(vao, nextVertexAttribute, TRANSFORM_DATA_BINDING_POINT_INDEX);
		glVertexArrayAttribFormat(vao, nextVertexAttribute++, 4, GL_FLOAT, GL_FALSE, i * vec4Size);
	}

	//Set up vertex index buffer
	glVertexArrayElementBuffer(vao, mesh.ebo());
}

OpenGLRenderModel::~OpenGLRenderModel()
{
}

void OpenGLRenderModel::reload(std::shared_ptr<OpenGLRenderMesh> mesh, std::size_t shaderID)
{
}

void OpenGLRenderModel::draw(int instanceCount) const
{
	glBindVertexArray(vao);
	//glDrawArraysInstanced(GL_TRIANGLES, 0, indexCount, instanceCount);
	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, instanceCount);
	glBindVertexArray(0);
}

GLuint OpenGLRenderModel::transformVBO() const
{
	return m_transformVBO;
}

void OpenGLRenderModel::bindTextures(const OpenGLShader& shader) const
{
	int textureUnit = 0;
	for (auto &texture : textures)
	{
		glActiveTexture(textureUnits[textureUnit]);
		glBindTexture(GL_TEXTURE_2D, texture.second->id());
		shader.setUniform1i(texture.first, textureUnit);

		if (textureUnit++ >= textureUnits.size())
		{
			break;
		}
	}
}
