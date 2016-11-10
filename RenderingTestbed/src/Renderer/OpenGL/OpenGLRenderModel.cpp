#include "Renderer/OpenGL/OpenGLRenderModel.h"

OpenGLRenderModel::OpenGLRenderModel(): meshID(0), shaderID(0)
{
}

OpenGLRenderModel::OpenGLRenderModel(std::size_t meshID, std::size_t shaderID) : meshID(meshID), shaderID(shaderID)
{
}

OpenGLRenderModel::~OpenGLRenderModel()
{
}
