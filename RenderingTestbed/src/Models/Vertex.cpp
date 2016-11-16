#include "Models/Vertex.h"

Vertex::~Vertex()
{
}

void Vertex::append(VertexFormat format, std::vector<float>& data) const
{
	if (format.hasPosition())
	{
		data.push_back(position.x);
		data.push_back(position.y);
		data.push_back(position.z);
	}

	if (format.hasNormal())
	{
		data.push_back(normal.x);
		data.push_back(normal.y);
		data.push_back(normal.z);
	}

	if (format.hasTextureCoordinates())
	{
		data.push_back(textureCoodinate.s);
		data.push_back(textureCoodinate.t);
	}

	if (format.hasColour())
	{
		data.push_back(colour.r);
		data.push_back(colour.g);
		data.push_back(colour.b);
		data.push_back(colour.a);
	}
}

std::vector<float> Vertex::flatten(VertexFormat format, std::vector<Vertex> vertices)
{
	std::vector<float> flattened;
	for (Vertex& v : vertices)
	{
		v.append(format, flattened);
	}

	return flattened;
}

Vertex::Vertex(const glm::vec3& position): position(position)
{}

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal): position(position), normal(normal)
{
}

Vertex::Vertex(const glm::vec3& position, const glm::vec2& textureCoodinate): position(position), textureCoodinate(textureCoodinate)
{
}

Vertex::Vertex(const glm::vec3& position, const glm::vec4& colour): position(position), colour(colour)
{
}

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec4& colour): position(position), normal(normal), colour(colour)
{
}

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& textureCoodinate): position(position), normal(normal), textureCoodinate(textureCoodinate)
{
}

Vertex::Vertex(const glm::vec3& position, const glm::vec2& textureCoodinate, const glm::vec4& colour): position(position), textureCoodinate(textureCoodinate), colour(colour)
{
}

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& textureCoodinate, const glm::vec4& colour): position(position), normal(normal), textureCoodinate(textureCoodinate), colour(colour)
{
}

VertexFormat::VertexFormat(char format) : format(format)
{
}

int VertexFormat::size() const
{
	return (hasPosition() ? sizeof(glm::vec3) : 0) +
		(hasNormal() ? sizeof(glm::vec3) : 0) +
		(hasTextureCoordinates() ? sizeof(glm::vec2) : 0) +
		(hasColour() ? sizeof(glm::vec4) : 0);
}

char VertexFormat::formatData() const
{
	return format;
}

bool VertexFormat::hasPosition() const
{
	return (format & VertexAttributes::Position) != 0;
}

bool VertexFormat::hasNormal() const
{
	return (format & VertexAttributes::Normal) != 0;
}

bool VertexFormat::hasTextureCoordinates() const
{
	return (format & VertexAttributes::Texture) != 0;
}

bool VertexFormat::hasColour() const
{
	return (format & VertexAttributes::Colour) != 0;
}

int VertexFormat::positionOffset() const
{
	return 0;
}

int VertexFormat::normalOffset() const
{
	return hasNormal() ? sizeof(glm::vec3) : 0;
}

int VertexFormat::textureCoordinatesOffset() const
{
	if (hasTextureCoordinates())
	{
		if (hasNormal())
		{
			return sizeof(glm::vec3) + sizeof(glm::vec3);
		}
		else
		{
			return sizeof(glm::vec3);
		}
	}
	return 0;
}

int VertexFormat::colourOffset() const
{
	if (hasColour())
	{
		if (hasTextureCoordinates())
		{
			if (hasNormal())
			{
				return sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2);
			}
			else
			{
				return sizeof(glm::vec3) + sizeof(glm::vec2);
			}
		}
		else
		{
			if (hasNormal())
			{
				return sizeof(glm::vec3) + sizeof(glm::vec3);
			}
			else
			{
				return sizeof(glm::vec3);
			}
		}
	}
	return 0;
}