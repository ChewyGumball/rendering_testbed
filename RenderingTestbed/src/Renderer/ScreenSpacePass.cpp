#include "Renderer/ScreenSpacePass.h"



ScreenSpacePass::ScreenSpacePass(Shader shader) : shader(shader)
{
}


ScreenSpacePass::~ScreenSpacePass()
{
}

void ScreenSpacePass::addBufferInput(TextureBuffer * buffer, std::string name)
{
	inputs.push_back(std::make_pair(name, buffer));
}

void ScreenSpacePass::setBufferAsOutput(TextureBuffer * buffer, GLenum outputSlot)
{
	outputs[outputSlot] = buffer;
}

void ScreenSpacePass::draw()
{
}
