#pragma once
#include <GL\glew.h>
#include <memory>
#include <unordered_map>

class OpenGLShader;
class ShaderConstantBuffer;

class OpenGLShaderConstantBuffer
{
private:
	GLuint m_handle;
	GLint m_deviceBufferSize;
	std::shared_ptr<const ShaderConstantBuffer> constantBuffer;

public:
	OpenGLShaderConstantBuffer();
	OpenGLShaderConstantBuffer(std::shared_ptr<const ShaderConstantBuffer> shaderConstantBuffer);
	~OpenGLShaderConstantBuffer();

	void uploadIfDirty() const;
};

