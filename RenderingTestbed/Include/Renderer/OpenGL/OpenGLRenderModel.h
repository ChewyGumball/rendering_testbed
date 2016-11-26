#pragma once
#include <unordered_map>

#include "OpenGLShader.h"
#include "OpenGLRenderMesh.h"
#include "Renderer/TextureBuffer.h"

class OpenGLRenderModel
{
public:
	const std::size_t meshID;
	const std::size_t shaderID;
	const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures;

	OpenGLRenderModel();
	OpenGLRenderModel(std::size_t meshID, std::size_t shaderID, const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures);
	~OpenGLRenderModel();

	void bindTextures(const OpenGLShader& shader) const;
};

