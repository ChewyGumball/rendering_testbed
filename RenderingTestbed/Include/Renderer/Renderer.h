#pragma once
#include <vector>
#include <Renderer/LayerPass.h>
#include <Renderer/ScreenSpacePass.h>
#include <Models/Mesh.h>

class Renderer
{
private:
	Shader finalRenderShader;
	Mesh finalRenderMesh;

	GLuint colourBufferTexture;
	GLuint depthBufferTexture;
	GLuint frameBuffer;

protected:
	std::vector<LayerPass> layers;
	std::vector<ScreenSpacePass> screenSpaceEffects;
public:
	Renderer(int width, int height);
	~Renderer();

	void resize(int width, int height);

	void addLayer(LayerPass layer);
	void addScreenSpacePass(ScreenSpacePass pass);
	void draw();
};

