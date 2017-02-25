#pragma once
#include <unordered_map>

#include "OpenGLShader.h"
#include "OpenGLRenderMesh.h"
#include "OpenGLShader.h"
#include "Renderer/TextureBuffer.h"
#include <Buffer/DataBufferView.h>

class OpenGLRenderModel
{
private:
	GLuint vao, m_transformVBO;
	RenderResourceID m_shaderID;
	//TODO: Make these OpenGLTextureBuffer handles
	std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> m_textures;
	uint32_t m_indexCount;
	DataBufferView m_shaderUniforms;

public:

	OpenGLRenderModel();
	OpenGLRenderModel(const OpenGLRenderMesh& mesh, const OpenGLShader& shader, const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures, DataBufferView shaderUniforms);
	~OpenGLRenderModel();

	void draw(int instanceCount) const;

	const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>>& textures() const;
	const RenderResourceID& shaderID() const;
	const DataBufferView& uniformBuffer() const;
	uint32_t indexCount() const;

	GLuint transformVBO() const;
};

