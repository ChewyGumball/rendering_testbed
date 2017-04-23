#pragma once
#include <glm/glm.hpp>
#include <ostream>
#include <vector>

namespace Renderer {

	typedef char VertexAttribute;
	namespace VertexAttributes {
		extern const VertexAttribute Position;
		extern const VertexAttribute Normal;
		extern const VertexAttribute Texture;
		extern const VertexAttribute Colour;
	}

	class VertexFormat {
	private:
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
		extern const VertexFormat Unknown;
		extern const VertexFormat Position;
		extern const VertexFormat Position_Normal;
		extern const VertexFormat Position_Texture;
		extern const VertexFormat Position_Colour;
		extern const VertexFormat Position_Normal_Colour;
		extern const VertexFormat Position_Normal_Texture;
		extern const VertexFormat Position_Texture_Colour;
		extern const VertexFormat Position_Normal_Texture_Colour;
	};

	class Vertex {
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

		static std::vector<float> flatten(VertexFormat format, std::vector<Vertex>& vertices);

		friend std::ostream& operator<<(std::ostream& stream, const Vertex& obj);
	};
}
