#include "Fonts/Font.h"

#include <unordered_map>

#include <Util\FileUtils.h>

#include <lib/stb_truetype/stb_rect_pack.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <lib/stb_truetype/stb_truetype.h>

#include "Renderer/TextureBuffer.h"
#include "Models/ModelInstance.h"

namespace {
	std::unordered_map<std::string, stbtt_fontinfo> fontInfos;
}

Font::Font(std::string fontFile, uint16_t height) : m_height(height)
{
	std::vector<uint8_t> fontFileData = Util::File::ReadBinary(fontFile);
	if (fontInfos.count(fontFile) == 0) {
		stbtt_fontinfo fontInfo = fontInfos[fontFile];
		stbtt_InitFont(&fontInfo, fontFileData.data(), 0);
	}

	stbtt_fontinfo fontInfo = fontInfos[fontFile];

	std::vector<uint8_t> pixelData(512 * 512);
	scale = stbtt_ScaleForPixelHeight(&fontInfo, height);

	stbtt_pack_context context;
	stbtt_PackBegin(&context, fontFileData.data(), 512, 512, 0, 1, nullptr);

	//all printable ascii characters 32 (space) to 126 (~)
	stbtt_PackFontRange(&context, fontFileData.data(), 0, scale, 32, 94, packedCharData); 

	stbtt_PackEnd(&context);

	m_bitmap = std::make_shared<TextureBuffer>(glm::ivec2(512, 512), TextureFormat::GREYSCALE, pixelData);
}


Font::~Font()
{
}

const std::shared_ptr<TextureBuffer> Font::bitmap()
{
	return m_bitmap;
}

uint16_t Font::height()
{
	return m_height;
}

std::vector<std::shared_ptr<ModelInstance>> Font::createString(std::string string)
{
	return std::vector<std::shared_ptr<ModelInstance>>();
}
