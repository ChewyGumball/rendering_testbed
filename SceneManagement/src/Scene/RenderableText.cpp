#include "Scene/RenderableText.h"

#include <algorithm>

#include <Renderer/RenderPass.h>

RenderableText::RenderableText(std::string text, std::shared_ptr<Font> font, glm::vec3 position, glm::vec4 colour) 
	: m_position(glm::vec3()), m_text(text), m_colour(colour), m_font(font), instances(font->createString(text, colour))
{
	translate(position);
}

RenderableText::~RenderableText()
{
}

void RenderableText::addToRenderPass(std::shared_ptr<RenderPass> pass)
{
	pass->addModelInstances(instances);
	passesContainingThisText.push_back(pass);
}

void RenderableText::removeFromRenderPass(std::shared_ptr<RenderPass> pass)
{
	pass->removeModelInstances(instances);
	passesContainingThisText.erase(std::remove(passesContainingThisText.begin(), passesContainingThisText.end(), pass), passesContainingThisText.end());
}

void RenderableText::translate(glm::vec3 offset)
{
	for (auto instance : instances) {
		instance->translate(offset);
	}
	m_position += offset;
}

void RenderableText::text(std::string newText)
{
	size_t newSize = newText.size();
	size_t oldSize = m_text.size();

	m_text = newText;

	//Instances need to be created and added to render passes
	if (newSize > oldSize) {
		std::vector<std::shared_ptr<ModelInstance>> instancesToAdd = m_font->createInstances(newSize - oldSize);
		//the new instances need to be positioned properly
		for (auto instance : instancesToAdd) {
			instance->translate(m_position);
		}

		for (auto pass : passesContainingThisText) {
			pass->addModelInstances(instancesToAdd);
		}
		instances.reserve(newSize);
		instances.insert(instances.end(), instancesToAdd.begin(), instancesToAdd.end());
	}
	//Instances need to be removed from render passes
	else if (newSize < oldSize) {
		std::vector<std::shared_ptr<ModelInstance>> instancesToErase(instances.begin() + newSize, instances.end());
		instances.erase(instances.begin() + newSize, instances.end());

		for (auto pass : passesContainingThisText) {
			pass->removeModelInstances(instancesToErase);
		}
	}

	m_font->modifyString(instances, m_text, m_colour);
}

const std::string RenderableText::text() const
{
	return m_text;
}

void RenderableText::font(std::shared_ptr<Font> newFont)
{
	m_font = newFont;
	newFont->modifyString(instances, m_text, m_colour);
}

std::shared_ptr<Font> RenderableText::font() const
{
	return m_font;
}

void RenderableText::colour(glm::vec4 newColour)
{
	m_colour = newColour;
}

glm::vec4 RenderableText::colour() const
{
	return m_colour;
}

glm::vec3 RenderableText::position()
{
	return m_position;
}

void RenderableText::position(glm::vec3 newPosition)
{
	translate(newPosition - m_position);
}
