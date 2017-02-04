#pragma once
#include <ostream>
#include<vector>
#include <glm/glm.hpp>

typedef char VertexAttribute;
namespace VertexAttributes {
	static VertexAttribute Position = 0b0001;
	static VertexAttribute Normal = 0b0010;
	static VertexAttribute Texture = 0b0100;
	static VertexAttribute Colour = 0b1000;
}

class VertexFormat
{
	VertexAttribute format;
public:
	VertexFormat();
	VertexFormat(VertexAttribute format);

	bool hasPosition() const;
	bool hasNormal() const;
	bool hasTextureCoordinates() const;
	bool hasColour() const;

	int positionOffset() const;
	int normalOffset() const;
	int textureCoordinatesOffset() const;
	int colourOffset() const;

	int size() const;

	VertexAttribute formatData() const;
};

bool operator!=(const VertexFormat& a, const VertexFormat& b);
bool operator==(const VertexFormat& a, const VertexFormat& b);

namespace VertexFormats {
	static const VertexFormat Unknown(0);
	static const VertexFormat Position(VertexAttributes::Position);
	static const VertexFormat Position_Normal(VertexAttributes::Position | VertexAttributes::Normal);
	static const VertexFormat Position_Texture(VertexAttributes::Position | VertexAttributes::Texture);
	static const VertexFormat Position_Colour(VertexAttributes::Position | VertexAttributes::Colour);
	static const VertexFormat Position_Normal_Colour(VertexAttributes::Position | VertexAttributes::Normal | VertexAttributes::Colour);
	static const VertexFormat Position_Normal_Texture(VertexAttributes::Position | VertexAttributes::Normal | VertexAttributes::Texture);
	static const VertexFormat Position_Texture_Colour(VertexAttributes::Position | VertexAttributes::Texture | VertexAttributes::Colour);
	static const VertexFormat Position_Normal_Texture_Colour(VertexAttributes::Position | VertexAttributes::Normal | VertexAttributes::Texture | VertexAttributes::Colour);
};

class Vertex
{
	glm::vec3 position;
	glm::vec4 colour;
	glm::vec3 normal;
	glm::vec2 textureCoodinate;

public:
	Vertex(const glm::vec3& position);
	Vertex(const glm::vec3& position, const glm::vec3& normal);
	Vertex(const glm::vec3& position, const glm::vec2& textureCoodinate);
	Vertex(const glm::vec3& position, const glm::vec4& colour);
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec4& colour);
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& textureCoodinate);
	Vertex(const glm::vec3& position, const glm::vec2& textureCoodinate, const glm::vec4& colour);
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& textureCoodinate, const glm::vec4& colour);
	~Vertex();

	void append(VertexFormat format, std::vector<float>& data) const;

	static std::vector<float> flatten(VertexFormat format, std::vector<Vertex> vertices);

	friend std::ostream& operator<< (std::ostream& stream, const Vertex& obj);
};

