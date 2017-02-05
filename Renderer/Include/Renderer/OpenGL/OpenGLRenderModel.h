#pragma once
#include <unordered_map>

#include "OpenGLShader.h"
#include "OpenGLRenderMesh.h"
#include "Renderer/TextureBuffer.h"

class OpenGLRenderModel
{
private:
	GLuint vao, m_transformVBO;
public:
	int indexCount;

	const std::size_t shaderID;
	const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures;

	OpenGLRenderModel();
	OpenGLRenderModel(const OpenGLRenderMesh& mesh, std::size_t shaderID, const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures);
	~OpenGLRenderModel();

	void reload(std::shared_ptr<OpenGLRenderMesh> mesh, std::size_t shaderID);

	void draw(int instanceCount) const;

	GLuint transformVBO() const;

	void bindTextures(const OpenGLShader& shader) const;
};

