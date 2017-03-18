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
#include <Renderer/Shader.h>
#include <Renderer/Mesh.h>
#include <Renderer/Material.h>

namespace {
	//font infos require the font data to stick around so it has to be stored somewhere
	std::unordered_map<std::string, std::vector<uint8_t>> fontData;
	std::unordered_map<std::string, stbtt_fontinfo> fontInfos;

	std::shared_ptr<Mesh> quad;

	std::shared_ptr<Shader> fontShader;
	std::shared_ptr<Material> fontMaterial;

	int atlasWidth = 512;
	int atlasHeight = 512;

	char minCharacter = ' ';
	char maxCharacter = '~';

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
				std::vector<std::string> { "F:/Users/Ben/Documents/Projects/RenderingTestbed/Renderer/src/Fonts/font.vert" },
				std::vector<std::string> { "F:/Users/Ben/Documents/Projects/RenderingTestbed/Renderer/src/Fonts/font.frag" },
				std::make_shared<BufferFormat>(std::vector<std::pair<std::string, BufferElementType>> {
					{ "transform", BufferElementType::MAT4},
					{ "meshBounds", BufferElementType::FLOAT_VEC4 },
					{ "textureBounds", BufferElementType::FLOAT_VEC4 },
					{ "letterColour", BufferElementType::FLOAT_VEC4 }
				}),
				std::unordered_map<std::string, std::shared_ptr<const BufferFormat>>(),
				std::vector<std::string> {"camera"}
			);

			fontMaterial = std::make_shared<Material>(fontShader);

			initialized = true;
		}
	}
}

Font::Font(std::string fontFile, uint16_t height) : m_height(height), fontFile(fontFile)
{
	initializeFonts();
	packedCharData = std::make_unique<stbtt_packedchar_DIDNT_NAME_THIS_STRUCT[]>(94);
	if (fontData.count(fontFile) == 0) {
		fontData[fontFile].swap(Util::File::ReadBinary(fontFile));
	}
	std::vector<uint8_t>& fontFileData = fontData[fontFile];

	if (fontInfos.count(fontFile) == 0) {
		stbtt_fontinfo& fontInfo = fontInfos[fontFile];
		stbtt_InitFont(&fontInfo, fontFileData.data(), 0);
	}

	stbtt_fontinfo fontInfo = fontInfos[fontFile];

	scale = stbtt_ScaleForPixelHeight(&fontInfo, height);

	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);
	lineHeight = (ascent - descent + lineGap) * scale;

	std::vector<uint8_t> pixelData(atlasWidth * atlasHeight);

	stbtt_pack_context context;
	assert(stbtt_PackBegin(&context, pixelData.data(), atlasWidth, atlasHeight, 0, 1, nullptr));
	stbtt_PackSetOversampling(&context, 2, 2);

	//all printable ascii characters 32 (space) to 126 (~)
	assert(stbtt_PackFontRange(&context, fontFileData.data(), 0, height, minCharacter, maxCharacter - minCharacter, packedCharData.get()));

	stbtt_PackEnd(&context);
	
	m_bitmap = std::make_shared<TextureBuffer>(glm::ivec2(atlasWidth, atlasHeight), TextureFormat::GREYSCALE, pixelData);
	m_model = std::make_shared<Model>(quad, fontMaterial, std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> {{ "font", m_bitmap }});
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

std::vector<std::shared_ptr<ModelInstance>> Font::createString(std::string string, glm::vec4 colour) const
{
	std::vector<std::shared_ptr<ModelInstance>> instances = createInstances(string.size());
	modifyString(instances, string, colour);
	return instances;
}

void Font::modifyColour(std::vector<std::shared_ptr<ModelInstance>>& text, int startInclusive, int endExclusive, glm::vec4 newColour) const
{
	for (size_t i = startInclusive; i < endExclusive; ++i) {
		text[i]->instanceData().set("letterColour", newColour);
	}
}

void Font::modifyString(const std::vector<std::shared_ptr<ModelInstance>>& currentText, std::string newString, glm::vec4 colour) const
{
	assert(currentText.size() == newString.size());

	stbtt_fontinfo& fontInfo = fontInfos[fontFile];


	float xPosition = 0;
	float yPosition = 0 + lineHeight;
	char previousChar = 0;
	for (int i = 0; i < newString.size(); ++i) {
		char currentChar = newString[i];
		if (currentChar == '\n') {
			yPosition += lineHeight;
			xPosition = 0;
			continue;
		}
		else if (currentChar == '\r' && i < newString.size() - 1 && newString[i + 1] == '\n') {
			i += 1;
			yPosition += lineHeight;
			xPosition = 0;
			continue;
		}
		else if (currentChar < minCharacter || currentChar > maxCharacter) {
			continue;
		}

		float offset = 0;
		//Don't need kerning when we are at the start of the line
		if (xPosition > 0) {
			offset = stbtt_GetCodepointKernAdvance(&fontInfo, currentChar, previousChar) * scale;
		}

		std::shared_ptr<ModelInstance> letter = currentText[i];

		stbtt_aligned_quad q;
		stbtt_GetPackedQuad(packedCharData.get(), atlasWidth, atlasHeight, currentChar - 32, &xPosition, &yPosition, &q, true);

		//I don't know why the y coordinates are weird, but these modifications are required otherwise the quads are upside down and backwards
		letter->instanceData().set("meshBounds", glm::vec4(q.x0 + offset, -q.y1, q.x1 + offset, -q.y0));
		letter->instanceData().set("textureBounds", glm::vec4(q.s0, q.t1, q.s1, q.t0));
		letter->instanceData().set("letterColour", colour);

		previousChar = currentChar;
	}
}

std::vector<std::shared_ptr<ModelInstance>> Font::createInstances(size_t instanceCount) const
{
	std::vector<std::shared_ptr<ModelInstance>> instances;
	instances.reserve(instanceCount);

	for (int i = 0; i < instanceCount; ++i) {
		instances.push_back(std::make_shared<ModelInstance>(m_model));
	}

	return instances;
}
