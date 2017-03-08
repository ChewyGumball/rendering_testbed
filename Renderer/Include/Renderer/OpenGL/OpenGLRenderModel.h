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
	uint32_t m_indexCount;
	RenderResourceID m_shaderID;
	const std::unordered_map<std::string, RenderResourceID> m_textures;
	const std::unordered_map<std::string, RenderResourceID> m_shaderConstants;

public:

	OpenGLRenderModel();
	OpenGLRenderModel(std::shared_ptr<OpenGLRenderMesh> mesh, std::shared_ptr<OpenGLShader> shader, const std::unordered_map<std::string, RenderResourceID> textures, const std::unordered_map<std::string, RenderResourceID> shaderConstants);
	~OpenGLRenderModel();

	void draw(int instanceCount) const;

	const std::unordered_map<std::string, RenderResourceID>& textures() const;
	const std::unordered_map<std::string, RenderResourceID>& shaderConstants() const;

	const RenderResourceID& shaderID() const;
	uint32_t indexCount() const;

	GLuint transformVBO() const;
};

