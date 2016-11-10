#pragma once
#include "Shaders/Shader.h"
#include "OpenGLRenderMesh.h"

class OpenGLRenderModel
{
public:
	const std::size_t meshID;
	const std::size_t shaderID;
	OpenGLRenderModel();
	OpenGLRenderModel(std::size_t meshID, std::size_t shaderID);
	~OpenGLRenderModel();
};

