#pragma once
#include <memory>
#include <unordered_map>

#include <stdint.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <Resources/RenderResource.h>

namespace Renderer {
	class BufferFormat;
	enum struct ShaderSourceType;
}

namespace Renderer::OpenGL {
	class OpenGLShader : public RenderResource
	{
	private:
		uint32_t programHandle;
		std::unordered_map<std::string, uint32_t> boundUniformBufferBindPoints;

	public:
		OpenGLShader(RenderResourceID id, std::unordered_map<ShaderSourceType, std::vector<std::string>>& sources);
		~OpenGLShader();

		void bind();

		void setUniform1i(const std::string uniformName, const int32_t& data) const;

		void setUniform1f(const std::string uniformName, const float& data) const;
		void setUniform2f(const std::string uniformName, const glm::vec2& data) const;
		void setUniform3f(const std::string uniformName, const glm::vec3& data) const;
		void setUniform4f(const std::string uniformName, const glm::vec4& data) const;
		void setUniformMatrix4f(const std::string uniformName, const glm::mat4& data) const;

		uint32_t program() const;
		int32_t getAttributeLocation(const std::string name) const;

		void bindUniformBufferToBindPoint(const std::string name, uint32_t bindPoint);
	};
}
