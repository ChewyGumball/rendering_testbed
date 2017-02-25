#pragma once
#include <stdint.h>
#include <memory>
#include <string>
#include <vector>
#include <glm\vec4.hpp>

class TextureBuffer;
class ModelInstance;
class Model;
struct stbtt_packedchar_DIDNT_NAME_THIS_STRUCT;

class Font
{
private:
	float scale;
	float lineHeight;
	std::unique_ptr<stbtt_packedchar_DIDNT_NAME_THIS_STRUCT[]> packedCharData;
	std::shared_ptr<TextureBuffer> m_bitmap;	
	std::shared_ptr<Model> m_model;
	uint16_t m_height;
	std::string fontFile;
public:
	Font(std::string fontFile, uint16_t height);
	~Font();

	const std::shared_ptr<TextureBuffer> bitmap();
	uint16_t height();

	std::vector<std::shared_ptr<ModelInstance>> createString(std::string string, glm::vec4 colour = glm::vec4(1,1,1,1)) const;

	void modifyColour(std::vector<std::shared_ptr<ModelInstance>>& text, int startInclusive, int endExclusive, glm::vec4 newColour) const;
	void modifyString(const std::vector<std::shared_ptr<ModelInstance>>& currentText, std::string newString, glm::vec4 colour = glm::vec4(1, 1, 1, 1)) const;
	std::vector<std::shared_ptr<ModelInstance>> createInstances(int instanceCount) const;
};
