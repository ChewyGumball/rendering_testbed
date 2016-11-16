#include "Renderer/Renderer.h"

namespace {
	std::string finalRenderVertexSource = R"(
#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f); 
    TexCoords = texCoords;
}  
		)";

	std::string finalRenderFragmentSource = R"(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;

void main()
{ 
    color = texture(screenTexture, TexCoords);
} 
		)";
	std::vector < Vertex > finalRenderMeshVertices = {
		Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec2(1.0f, 1.0f))
	};

	std::vector<int> finalRenderMeshIndicies = {
		0,1,2,

		3,4,5
	};
}


Renderer::Renderer(int width, int height) 
	: finalRenderShader(finalRenderVertexSource, finalRenderFragmentSource), 
	finalRenderMesh(VertexFormats::Position_Texture, Vertex::flatten(VertexFormats::Position_Texture, finalRenderMeshVertices),
	finalRenderMeshIndicies)
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	resize(width, height);
}


Renderer::~Renderer()
{
}

void Renderer::resize(int width, int height)
{
	glDeleteTextures(1, &colourBufferTexture);
	glDeleteTextures(1, &depthBufferTexture);

	glGenTextures(1, &colourBufferTexture);
	glGenTextures(1, &depthBufferTexture);

	glBindTexture(GL_TEXTURE_2D, colourBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, depthBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourBufferTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBufferTexture, 0);
}

void Renderer::addLayer(LayerPass layer)
{
	layers.push_back(layer);
}

void Renderer::addScreenSpacePass(ScreenSpacePass pass)
{
	screenSpaceEffects.push_back(pass);
}

void Renderer::draw()
{
	for (LayerPass layer : layers)
	{
		layer.draw();
	}
}
