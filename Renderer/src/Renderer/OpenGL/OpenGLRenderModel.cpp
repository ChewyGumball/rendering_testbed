#include "Renderer/OpenGL/OpenGLRenderModel.h"

#include <Renderer/OpenGL/OpenGLShader.h>
#include <Renderer/OpenGL/OpenGLRenderMesh.h>
#include <Resources/ShaderConstantBuffer.h>
#include <Buffer/BufferFormat.h>
#include <Resources/Material.h>
#include <Resources/Shader.h>
#include <Resources/TextureBuffer.h>

using namespace Renderer;
using namespace Renderer::OpenGL;

namespace {

const GLuint VERTEX_DATA_BINDING_POINT_INDEX    = 0;
const GLuint TRANSFORM_DATA_BINDING_POINT_INDEX = 1;

struct AttributeDetails {
    GLenum  type;
    uint8_t slotCount;
    uint8_t componentCount;
};

const std::unordered_map<BufferElementType, AttributeDetails> glTypes { 
	{ BufferElementType::FLOAT_SCALAR, { GL_FLOAT, 1, 1 } },
    { BufferElementType::FLOAT_VEC2, { GL_FLOAT, 1, 2 } },
    { BufferElementType::FLOAT_VEC3, { GL_FLOAT, 1, 3 } },
    { BufferElementType::FLOAT_VEC4, { GL_FLOAT, 1, 4 } },
    { BufferElementType::DOUBLE_SCALAR, { GL_DOUBLE, 1, 1 } },
    { BufferElementType::DOUBLE_VEC2, { GL_DOUBLE, 1, 2 } },
    { BufferElementType::DOUBLE_VEC3, { GL_DOUBLE, 1, 3 } },
    { BufferElementType::DOUBLE_VEC4, { GL_DOUBLE, 1, 4 } },
    { BufferElementType::BOOL_SCALAR, { GL_BOOL, 1, 1 } },
    { BufferElementType::BOOL_VEC2, { GL_BOOL, 1, 2 } },
    { BufferElementType::BOOL_VEC3, { GL_BOOL, 1, 3 } },
    { BufferElementType::BOOL_VEC4, { GL_BOOL, 1, 4 } },
    { BufferElementType::INT_SCALAR, { GL_INT, 1, 1 } },
    { BufferElementType::INT_VEC2, { GL_INT, 1, 2 } },
    { BufferElementType::INT_VEC3, { GL_INT, 1, 3 } },
    { BufferElementType::INT_VEC4, { GL_INT, 1, 4 } },
    { BufferElementType::UINT_SCALAR, { GL_UNSIGNED_INT, 1, 1 } },
    { BufferElementType::UINT_VEC2, { GL_UNSIGNED_INT, 1, 2 } },
    { BufferElementType::UINT_VEC3, { GL_UNSIGNED_INT, 1, 3 } },
    { BufferElementType::UINT_VEC4, { GL_UNSIGNED_INT, 1, 4 } },
    { BufferElementType::MAT2, { GL_FLOAT, 2, 2} },
    { BufferElementType::MAT3, { GL_FLOAT, 3, 3} },
    { BufferElementType::MAT4, { GL_FLOAT, 4, 4} }
};


// each vertex array attribute need to be:
//	1. enabled for the vao
//	2. bound to a specific binding point in the vao (ie which buffer is going to fill this attribute, as set by glVertexArrayVertexBuffer below)
//	3. formatted for the type of attribute is being bound there
void setupVertexAttribute(GLuint vao, GLuint attribute, GLint componentCount, GLuint offset)
{
    glEnableVertexArrayAttrib(vao, attribute);
    glVertexArrayAttribBinding(vao, attribute, VERTEX_DATA_BINDING_POINT_INDEX);
    glVertexArrayAttribFormat(vao, attribute, componentCount, GL_FLOAT, GL_FALSE, offset);
}

void setupVertexAttributes(GLuint vao, GLuint vbo, VertexFormat vertexFormat)
{
	int nextVertexAttribute = 0;

	// Set up mesh vertex data attributes
	glVertexArrayVertexBuffer(vao, VERTEX_DATA_BINDING_POINT_INDEX, vbo, 0, vertexFormat.size());
	if (vertexFormat.hasPosition()) {
		setupVertexAttribute(vao, nextVertexAttribute++, 3, vertexFormat.positionOffset());
	}

	if (vertexFormat.hasNormal()) {
		setupVertexAttribute(vao, nextVertexAttribute++, 3, vertexFormat.normalOffset());
	}

	if (vertexFormat.hasTextureCoordinates()) {
		setupVertexAttribute(vao, nextVertexAttribute++, 2, vertexFormat.textureCoordinatesOffset());
	}

	if (vertexFormat.hasColour()) {
		setupVertexAttribute(vao, nextVertexAttribute++, 4, vertexFormat.colourOffset());
	}
}

void setupInstanceStateAttribute(GLuint vao, GLuint attribute, BufferElementType type, GLuint offset)
{
    const AttributeDetails& details = glTypes.at(type);
	uint64_t size = BufferFormat::sizeOfType(type) / details.slotCount;
    for (int i = 0; i < details.slotCount; ++i) {
        glEnableVertexArrayAttrib(vao, attribute + i);
        glVertexArrayAttribBinding(vao, attribute + i, TRANSFORM_DATA_BINDING_POINT_INDEX);
        glVertexArrayAttribFormat(vao, attribute + i, details.componentCount, details.type, GL_FALSE, static_cast<GLuint>(offset + (i * size)));
    }
}

void setupInstanceStateAttributes(GLuint vao, GLuint vbo, OpenGLShader& shader, std::shared_ptr<const BufferFormat> format)
{
	glVertexArrayVertexBuffer(vao, TRANSFORM_DATA_BINDING_POINT_INDEX, vbo, 0, static_cast<GLsizei>(format->size()));
	glVertexArrayBindingDivisor(vao, TRANSFORM_DATA_BINDING_POINT_INDEX, 1);

	for (auto& variable : format->offsets()) {
		GLint attributeLocation = shader.getAttributeLocation(variable.first);
		if (attributeLocation != -1) {
			setupInstanceStateAttribute(vao, attributeLocation, variable.second.second, static_cast<GLuint>(variable.second.first));
		}
	}
}

}

OpenGLRenderModel::OpenGLRenderModel(OpenGLRenderMesh& mesh, OpenGLShader& shader, const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures, std::shared_ptr<const Material> material)
    : m_indexCount(mesh.indexCount()), m_textures(textures), m_material(material)
{
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &m_transformVBO);

	setupVertexAttributes(vao, mesh.vbo(), mesh.vertexFormat());
	setupInstanceStateAttributes(vao, m_transformVBO, shader, material->shader()->instanceStateFormat());

    // Set up vertex index buffer
    glVertexArrayElementBuffer(vao, mesh.ebo());
}

OpenGLRenderModel::~OpenGLRenderModel() 
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &m_transformVBO);
}

void OpenGLRenderModel::draw(int instanceCount) const
{
    glBindVertexArray(vao);
    glDrawElementsInstanced(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0, instanceCount);
    glBindVertexArray(0);
}

const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>>& OpenGLRenderModel::textures() const { return m_textures; }

const std::shared_ptr<const Material> OpenGLRenderModel::material() const
{
	return m_material;
}

uint32_t OpenGLRenderModel::indexCount() const { return m_indexCount; }

GLuint OpenGLRenderModel::transformVBO() const { return m_transformVBO; }
