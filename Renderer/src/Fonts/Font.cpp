#include "Fonts/Font.h"

#include <unordered_map>
#include <iostream>

#include <Util\FileUtils.h>

#define STB_RECT_PACK_IMPLEMENTATION
#include <lib/stb_truetype/stb_rect_pack.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <lib/stb_truetype/stb_truetype.h>

#include "Renderer/TextureBuffer.h"
#include "Renderer/ModelInstance.h"

namespace {
	std::unordered_map<std::string, stbtt_fontinfo> fontInfos;

	std::shared_ptr<Mesh> quad;

	std::shared_ptr<Shader> fontShader;

	int atlasWidth = 512;
	int atlasHeight = 512;

	bool initialized = false;
	void initializeFonts() {
		if (!initialized) {
			quad = std::make_shared<Mesh>(VertexFormats::Position, std::vector<float> {
				0.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f
			}, std::vector<int> {
				0, 1, 2,
				0, 2, 3
			});

			fontShader = std::make_shared<Shader>(
				"F:/Users/Ben/Documents/Projects/RenderingTestbed/Renderer/src/Fonts/font.vert",
				"F:/Users/Ben/Documents/Projects/RenderingTestbed/Renderer/src/Fonts/font.frag",
				BufferFormat(std::vector<std::pair<std::string, BufferElementType>> {
					{ "transform", BufferElementType::MAT4},
					{ "meshBounds", BufferElementType::FLOAT_VEC4 },
					{ "textureBounds", BufferElementType::FLOAT_VEC4 },
					{ "letterColour", BufferElementType::FLOAT_VEC4 }
				})
			);

			initialized = true;
		}
	}
}

Font::Font(std::string fontFile, uint16_t height) : m_height(height), fontFile(fontFile)
{
	initializeFonts();
	packedCharData = std::make_unique<stbtt_packedchar_DIDNT_NAME_THIS_STRUCT[]>(94);
	std::vector<uint8_t> fontFileData = Util::File::ReadBinary(fontFile);
	if (fontInfos.count(fontFile) == 0) {
		stbtt_fontinfo& fontInfo = fontInfos[fontFile];
		stbtt_InitFont(&fontInfo, fontFileData.data(), 0);
	}

	stbtt_fontinfo fontInfo = fontInfos[fontFile];

	std::vector<uint8_t> pixelData(atlasWidth * atlasHeight);
	scale = stbtt_ScaleForPixelHeight(&fontInfo, height);

	stbtt_pack_context context;
	assert(stbtt_PackBegin(&context, pixelData.data(), atlasWidth, atlasHeight, 0, 1, nullptr));
	stbtt_PackSetOversampling(&context, 2, 2);

	//all printable ascii characters 32 (space) to 126 (~)
	assert(stbtt_PackFontRange(&context, fontFileData.data(), 0, height, 32, 94, packedCharData.get()));

	stbtt_PackEnd(&context);
	
	m_bitmap = std::make_shared<TextureBuffer>(glm::ivec2(atlasWidth, atlasHeight), TextureFormat::GREYSCALE, pixelData);
	m_model = std::make_shared<Model>(quad, fontShader, std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> {{ "font", m_bitmap }});
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

std::vector<std::shared_ptr<ModelInstance>> Font::createString(std::string string, glm::vec4 colour)
{
	std::vector<std::shared_ptr<ModelInstance>> instances;
	instances.reserve(string.size());
	stbtt_fontinfo& fontInfo = fontInfos[fontFile];	

	float xPosition = 0;
	float yPosition = 0;

	for (int i = 0; i < string.size(); ++i) {
		char currentChar = string[i];
		float offset = 0;
		if (i > 0) {
			offset = stbtt_GetCodepointKernAdvance(&fontInfo, currentChar, string[i - 1]) * scale;
		}

		std::shared_ptr<ModelInstance> letter = std::make_shared<ModelInstance>(m_model);

		stbtt_aligned_quad q;
		stbtt_GetPackedQuad(packedCharData.get(), atlasWidth, atlasHeight, currentChar - 32, &xPosition, &yPosition, &q, true);
				
		letter->setState("meshBounds", glm::vec4(q.x0 + offset, -q.y1, q.x1 + offset, -q.y0));
		letter->setState("textureBounds", glm::vec4(q.s0, q.t1, q.s1, q.t0));
		letter->setState("letterColour", colour);

		instances.push_back(letter);
	}

	return instances;
}
