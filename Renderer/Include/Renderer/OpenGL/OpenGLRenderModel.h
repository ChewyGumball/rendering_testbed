#pragma once
#include <unordered_map>

#include "OpenGLShader.h"
#include "OpenGLRenderMesh.h"
#include "OpenGLShader.h"
#include "Renderer/TextureBuffer.h"

class OpenGLRenderModel
{
private:
	GLuint vao, m_transformVBO;
	RenderResourceID m_shaderID;
	std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> m_textures;
	uint32_t m_indexCount;
public:

	OpenGLRenderModel();
	OpenGLRenderModel(const OpenGLRenderMesh& mesh, const OpenGLShader& shader, const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures);
	~OpenGLRenderModel();

	void draw(int instanceCount) const;

	const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>>& textures() const;
	const RenderResourceID& shaderID() const;
	uint32_t indexCount() const;

	GLuint transformVBO() const;
};

