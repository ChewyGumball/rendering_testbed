#pragma once
#include <stdint.h>
#include <memory>
#include <string>
#include <vector>

class TextureBuffer;
class ModelInstance;
struct stbtt_packedchar_DIDNT_NAME_THIS_STRUCT;

class Font
{
private:
	float scale;
	stbtt_packedchar_DIDNT_NAME_THIS_STRUCT* packedCharData;
	std::shared_ptr<TextureBuffer> m_bitmap;
	uint16_t m_height;
public:
	Font(std::string fontFile, uint16_t height);
	~Font();

	const std::shared_ptr<TextureBuffer> bitmap();
	uint16_t height();

	std::vector<std::shared_ptr<ModelInstance>> createString(std::string string);
};
