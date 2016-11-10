#pragma once
#include<vector>
#include <glm/glm.hpp>

typedef const char VertexAttribute;
namespace VertexAttributes {
	static const char Position = 0b0001;
	static const char Normal = 0b0010;
	static const char Texture = 0b0100;
	static const char Colour = 0b1000;
}

typedef const char VertexFormat;
namespace VertexFormats {
	static const char Position = VertexAttributes::Position;
	static const char Position_Normal = VertexAttributes::Position | VertexAttributes::Normal;
	static const char Position_Texture = VertexAttributes::Position | VertexAttributes::Texture;
	static const char Position_Colour = VertexAttributes::Position | VertexAttributes::Colour;
	static const char Position_Normal_Colour = VertexAttributes::Position | VertexAttributes::Normal | VertexAttributes::Colour;
	static const char Position_Normal_Texture = VertexAttributes::Position | VertexAttributes::Normal | VertexAttributes::Texture;
	static const char Position_Texture_Colour = VertexAttributes::Position | VertexAttributes::Texture | VertexAttributes::Colour;
	static const char Position_Normal_Texture_Colour = VertexAttributes::Position | VertexAttributes::Normal | VertexAttributes::Texture | VertexAttributes::Colour;
};

class Vertex
{
	VertexFormat format;
	glm::vec3 position;
	glm::vec4 colour;
	glm::vec3 normal;
	glm::vec2 textureCoodinate;

public:
	Vertex(glm::vec3 position);
	Vertex(glm::vec3 position, glm::vec3 normal);
	Vertex(glm::vec3 position, glm::vec2 textureCoodinate);
	Vertex(glm::vec3 position, glm::vec4 colour);
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec4 colour); 
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoodinate);
	Vertex(glm::vec3 position, glm::vec2 textureCoodinate, glm::vec4 colour);
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoodinate, glm::vec4 colour);
	~Vertex();

	int size() const;

	bool hasPosition() const;
	bool hasNormal() const;
	bool hasTextureCoordinates() const;
	bool hasColour() const;

	int positionOffset() const;
	int normalOffset() const;
	int textureCoordinatesOffset() const;
	int colourOffset() const;

	void append(std::vector<float>& data) const;
};

