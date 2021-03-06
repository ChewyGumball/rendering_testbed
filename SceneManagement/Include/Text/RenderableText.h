#pragma once
#include <string>
#include <vector>
#include <Text/Font.h>
#include <glm/vec3.hpp>

#include <Drawing/ModelGroup.h>

namespace Scene {
	class RenderPass;
}

namespace Scene::Text {
	class RenderableText
	{
	private:
		glm::vec3 m_position;
		std::string m_text;
		glm::vec4 m_colour;
		std::shared_ptr<Scene::Text::Font> m_font;
		ModelGroup characters;
		std::vector<std::shared_ptr<RenderPass>> passesContainingThisText;

	public:
		RenderableText(std::string text, std::shared_ptr<Scene::Text::Font> font, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec4 colour = glm::vec4(1, 1, 1, 1));
		~RenderableText();

		void addToRenderPass(std::shared_ptr<RenderPass> pass);
		void removeFromRenderPass(std::shared_ptr<RenderPass> pass);

		glm::vec3 position();
		void position(glm::vec3 newPosition);
		void translate(glm::vec3 offset);

		void text(std::string newText);
		const std::string text() const;

		void font(std::shared_ptr<Scene::Text::Font> newFont);
		std::shared_ptr<Scene::Text::Font> font() const;

		void colour(glm::vec4 newColour);
		glm::vec4 colour() const;

	};
}

