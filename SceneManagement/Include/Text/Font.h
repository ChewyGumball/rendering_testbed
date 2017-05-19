#pragma once
#include <stdint.h>
#include <memory>
#include <string>
#include <vector>
#include <glm\vec4.hpp>

namespace Renderer {
	class TextureBuffer;
	class ModelInstance;
	class Model;
}

struct stbtt_packedchar_DIDNT_NAME_THIS_STRUCT;

namespace Scene {
	class ModelGroup;
}

namespace Scene::Text {
	class Font
	{
	private:
		float scale;
		float lineHeight;
		std::unique_ptr<stbtt_packedchar_DIDNT_NAME_THIS_STRUCT[]> packedCharData;
		std::shared_ptr<Renderer::TextureBuffer> m_bitmap;
		std::shared_ptr<Renderer::Model> m_model;
		uint16_t m_height;
		std::string fontFile;
	public:
		Font(std::string fontFile, uint16_t height);
		~Font();

		const std::shared_ptr<Renderer::TextureBuffer> bitmap();
		uint16_t height();

		ModelGroup createString(std::string string, glm::vec4 colour = glm::vec4(1, 1, 1, 1)) const;

		void modifyColour(const ModelGroup& text, int startInclusive, int endExclusive, glm::vec4 newColour) const;
		void modifyString(const ModelGroup& currentText, std::string newString, glm::vec4 colour = glm::vec4(1, 1, 1, 1)) const;
		ModelGroup createCharacters(size_t instanceCount) const;

		uint32_t drawableCharacters(std::string& string) const;
	};
}
