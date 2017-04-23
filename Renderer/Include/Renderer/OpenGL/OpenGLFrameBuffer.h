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
		OpenGLFrameBuffer();
		OpenGLFrameBuffer(std::shared_ptr<const FrameBuffer> frameBuffer, std::unordered_map<RenderResourceID, std::shared_ptr<OpenGLTextureBuffer>>& textures);
		~OpenGLFrameBuffer();

		void bind();
		void unbind();
	};
}
