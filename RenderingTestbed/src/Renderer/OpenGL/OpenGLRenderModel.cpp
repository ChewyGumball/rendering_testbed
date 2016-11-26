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
}

OpenGLRenderModel::OpenGLRenderModel(): meshID(0), shaderID(0)
{
}

OpenGLRenderModel::OpenGLRenderModel(std::size_t meshID, std::size_t shaderID, const std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures) : meshID(meshID), shaderID(shaderID), textures(textures)
{
}

OpenGLRenderModel::~OpenGLRenderModel()
{
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
