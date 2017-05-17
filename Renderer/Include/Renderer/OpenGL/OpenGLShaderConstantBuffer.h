#pragma once
#include <stdint.h>
#include <Buffer/DataBuffer.h>

namespace Renderer::OpenGL {
	class OpenGLShaderConstantBuffer
	{
	private:
		uint32_t m_handle;
		int32_t m_deviceBufferSize;
		DataBufferView constantBuffer;

	public:
		OpenGLShaderConstantBuffer(DataBufferView buffer);
		~OpenGLShaderConstantBuffer();

		void bindTo(uint32_t bindPoint);
		void uploadIfDirty() const;
	};
}
