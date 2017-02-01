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

	void setupVertexAttribute(GLuint vao, GLuint attribute, GLuint bindingPoint, GLint componentCount, GLuint offset)
	{
		glEnableVertexArrayAttrib(vao, attribute);
		glVertexArrayAttribBinding(vao, attribute, bindingPoint);
		glVertexArrayAttribFormat(vao, attribute, componentCount, GL_FLOAT, GL_FALSE, offset);
	}

}

OpenGLRenderModel::OpenGLRenderModel(): shaderID(0)
{
}

OpenGLRenderModel::OpenGLRenderModel(const OpenGLRenderMesh& mesh, std::size_t shaderID, const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures) 
	: shaderID(shaderID), textures(textures), indexCount(mesh.indexCount())
{
	glCreateVertexArrays(1, &vao);
	glCreateBuffers(1, &m_transformVBO);

	const VertexFormat format = mesh.vertexFormat();
	int nextVertexAttribute = 0;

	//Set up mesh vertex data attributes
	glVertexArrayVertexBuffer(vao, VERTEX_DATA_BINDING_POINT_INDEX, mesh.vbo(), 0, format.size());
	if (format.hasPosition()) {
		setupVertexAttribute(vao, nextVertexAttribute++, VERTEX_DATA_BINDING_POINT_INDEX, 3, format.positionOffset());
	}

	if (format.hasNormal())	{
		setupVertexAttribute(vao, nextVertexAttribute++, VERTEX_DATA_BINDING_POINT_INDEX, 3, format.normalOffset());
	}

	if (format.hasTextureCoordinates())	{
		setupVertexAttribute(vao, nextVertexAttribute++, VERTEX_DATA_BINDING_POINT_INDEX, 2, format.textureCoordinatesOffset());
	}

	if (format.hasColour())	{
		setupVertexAttribute(vao, nextVertexAttribute++, VERTEX_DATA_BINDING_POINT_INDEX, 4, format.colourOffset());
	}

	//Set up instancing transform buffer attributes
	GLsizei vec4Size = sizeof(glm::vec4);
	glVertexArrayVertexBuffer(vao, TRANSFORM_DATA_BINDING_POINT_INDEX, m_transformVBO, 0, 4 * vec4Size); 
	glVertexArrayBindingDivisor(vao, TRANSFORM_DATA_BINDING_POINT_INDEX, 1);

	for (int i = 0; i < 4; ++i)	{
		setupVertexAttribute(vao, nextVertexAttribute++, TRANSFORM_DATA_BINDING_POINT_INDEX, 4, i * vec4Size);
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
