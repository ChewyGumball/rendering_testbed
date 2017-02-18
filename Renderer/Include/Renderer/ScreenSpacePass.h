#pragma once
#include <Renderer\Shader.h>
#include <vector>
#include <unordered_map>

class TextureBuffer;

class ScreenSpacePass
{
protected:

	std::vector<std::pair<std::string, TextureBuffer*>> inputs;
	std::unordered_map<GLenum, TextureBuffer*> outputs;
	Shader shader;
public:
	ScreenSpacePass(Shader shader);
	virtual ~ScreenSpacePass();

	void draw();

	void addBufferInput(TextureBuffer* buffer, std::string name);
	void setBufferAsOutput(TextureBuffer* buffer, GLenum outputSlot);
};

