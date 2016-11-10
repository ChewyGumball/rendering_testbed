#include "Models/Vertex.h"

Vertex::~Vertex()
{
}

int Vertex::size() const
{
	return (hasPosition() ? sizeof(position) : 0) +
		   (hasNormal() ? sizeof(normal) : 0) +
		   (hasTextureCoordinates() ? sizeof(textureCoodinate) : 0) +
		   (hasColour() ? sizeof(colour) : 0);
}

bool Vertex::hasPosition() const
{
	return (format & VertexAttributes::Position) != 0;
}

bool Vertex::hasNormal() const
{
	return (format & VertexAttributes::Normal) != 0;
}

bool Vertex::hasTextureCoordinates() const
{
	return (format & VertexAttributes::Texture) != 0;
}

bool Vertex::hasColour() const
{
	return (format & VertexAttributes::Colour) != 0;
}

int Vertex::positionOffset() const
{
	return 0;
}

int Vertex::normalOffset() const
{
	return hasNormal() ? sizeof(position) : 0;
}

int Vertex::textureCoordinatesOffset() const
{
	if (hasTextureCoordinates())
	{
		if (hasNormal())
		{
			return sizeof(position) + sizeof(normal);
		}
		else
		{
			return sizeof(position);
		}
	}
	return 0;
}

int Vertex::colourOffset() const
{
	if (hasColour())
	{
		if (hasTextureCoordinates())
		{
			if (hasNormal())
			{
				return sizeof(position) + sizeof(normal) + sizeof(textureCoodinate);
			}
			else
			{
				return sizeof(position) + sizeof(textureCoodinate);
			}
		}
		else
		{
			if (hasNormal())
			{
				return sizeof(position) + sizeof(normal);
			}
			else
			{
				return sizeof(position);
			}
		}
	}
	return 0;
}

void Vertex::append(std::vector<float>& data) const
{
	if (hasPosition())
	{
		data.push_back(position.x);
		data.push_back(position.y);
		data.push_back(position.z);
	}

	if (hasNormal())
	{
		data.push_back(normal.x);
		data.push_back(normal.y);
		data.push_back(normal.z);
	}

	if (hasTextureCoordinates())
	{
		data.push_back(textureCoodinate.s);
		data.push_back(textureCoodinate.t);
	}

	if (hasColour())
	{
		data.push_back(colour.r);
		data.push_back(colour.g);
		data.push_back(colour.b);
		data.push_back(colour.a);
	}
}

Vertex::Vertex(glm::vec3 position) : format(VertexFormats::Position), position(position)
{}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal) : format(VertexFormats::Position_Normal), position(position), normal(normal)
{
}

Vertex::Vertex(glm::vec3 position, glm::vec2 textureCoodinate) : format(VertexFormats::Position_Texture), position(position), textureCoodinate(textureCoodinate)
{
}

Vertex::Vertex(glm::vec3 position, glm::vec4 colour) : format(VertexFormats::Position_Colour), position(position), colour(colour)
{
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec4 colour) : format(VertexFormats::Position_Normal_Colour), position(position), normal(normal), colour(colour)
{
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoodinate) : format(VertexFormats::Position_Normal_Texture), position(position), normal(normal), textureCoodinate(textureCoodinate)
{
}

Vertex::Vertex(glm::vec3 position, glm::vec2 textureCoodinate, glm::vec4 colour) : format(VertexFormats::Position_Texture_Colour), position(position), textureCoodinate(textureCoodinate), colour(colour)
{
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoodinate, glm::vec4 colour) : format(VertexFormats::Position_Normal_Texture_Colour), position(position), normal(normal), textureCoodinate(textureCoodinate), colour(colour)
{
}
