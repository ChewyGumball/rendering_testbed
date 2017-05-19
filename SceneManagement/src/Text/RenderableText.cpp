#include "Text/RenderableText.h"

#include <algorithm>

#include <Resources/ModelInstance.h>
#include <Drawing/RenderPass.h>

#include <glm/gtc/matrix_transform.hpp>

namespace Scene::Text {
	RenderableText::RenderableText(std::string text, std::shared_ptr<Scene::Text::Font> font, glm::vec3 position, glm::vec4 colour)
		: m_position(glm::vec3()), m_text(text), m_colour(colour), m_font(font), characters(font->createString(text, colour))
	{
		translate(position);
	}

	RenderableText::~RenderableText()
	{
	}

	void RenderableText::addToRenderPass(std::shared_ptr<RenderPass> pass)
	{
		pass->addModelInstances(characters.instances());
		passesContainingThisText.push_back(pass);
	}

	void RenderableText::removeFromRenderPass(std::shared_ptr<RenderPass> pass)
	{
		pass->removeModelInstances(characters.instances());
		passesContainingThisText.erase(std::remove(passesContainingThisText.begin(), passesContainingThisText.end(), pass), passesContainingThisText.end());
	}

	void RenderableText::translate(glm::vec3 offset)
	{
		characters.translate(ShaderAttribute::Transform, offset);
		m_position += offset;
	}

	void RenderableText::text(std::string newText)
	{
		uint32_t newSize = m_font->drawableCharacters(newText);
		uint32_t oldSize = m_font->drawableCharacters(m_text);

		m_text = newText;

		//Instances need to be created and added to render passes
		if (newSize > oldSize) {
			ModelGroup newCharacters = m_font->createCharacters(newSize - oldSize);
			//the new instances need to be positioned properly
			newCharacters.translate(ShaderAttribute::Transform, m_position);

			for (auto pass : passesContainingThisText) {
				pass->addModelInstances(newCharacters.instances());
			}

			characters.addModelGroup(newCharacters);
		}
		//Instances need to be removed from render passes
		else if (newSize < oldSize) {
			std::vector<std::shared_ptr<Renderer::ModelInstance>> instancesToErase(characters.instances().begin() + newSize, characters.instances().end());
			characters.removeInstances(instancesToErase);

			for (auto pass : passesContainingThisText) {
				pass->removeModelInstances(instancesToErase);
			}
		}

		m_font->modifyString(characters, m_text, m_colour);
	}

	const std::string RenderableText::text() const
	{
		return m_text;
	}

	void RenderableText::font(std::shared_ptr<Scene::Text::Font> newFont)
	{
		m_font = newFont;
		newFont->modifyString(characters, m_text, m_colour);
	}

	std::shared_ptr<Scene::Text::Font> RenderableText::font() const
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
}
