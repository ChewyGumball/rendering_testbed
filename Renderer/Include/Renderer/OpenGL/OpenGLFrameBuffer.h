#pragma once

#include <GL\glew.h>
#include <unordered_map>
#include <memory>

#include <Resources/FrameBuffer.h>
#include <Renderer\OpenGL/OpenGLTextureBuffer.h>

namespace Renderer::OpenGL {
	class OpenGLFrameBuffer
	{
	private:
		GLuint buffer;

	public:
		OpenGLFrameBuffer(std::unordered_map<FrameBufferTarget, std::shared_ptr<TextureBuffer>>& targets, std::unordered_map<RenderResourceID, OpenGLTextureBuffer>& textures);
		~OpenGLFrameBuffer();

		void bind();
		void unbind();
	};
}
