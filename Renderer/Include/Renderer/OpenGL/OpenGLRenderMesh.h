#pragma once
#include <stdint.h>
#include <vector>

#include <Resources/Vertex.h>

namespace Renderer::OpenGL {
	class OpenGLRenderMesh
	{
	protected:
		uint32_t m_indexCount;
		VertexFormat m_format;
		uint32_t m_vbo, m_ebo;
	public:
		OpenGLRenderMesh();
		OpenGLRenderMesh(VertexFormat format, std::vector<float>& vertexData, std::vector<uint32_t>& indices);
		~OpenGLRenderMesh();

		uint32_t vbo() const;
		uint32_t ebo() const;
		uint32_t indexCount() const;
		VertexFormat vertexFormat() const;
	};
}
