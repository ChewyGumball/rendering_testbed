#pragma once
#include <Resources/RenderResource.h>
#include <Buffer/DataBuffer.h>

namespace Renderer {
	class DataBufferArrayView;

	class ShaderConstantBuffer : public RenderResource
	{
	private:
		DataBuffer m_buffer;
		
	public:
		ShaderConstantBuffer(std::shared_ptr<BufferFormat> format);
		~ShaderConstantBuffer();

		void set(const std::string& name, const glm::vec2& value);
		void set(const std::string& name, const glm::vec3& value);
		void set(const std::string& name, const glm::vec4& value);
		void set(const std::string& name, const uint32_t& value);
		void set(const std::string& name, const int32_t& value);
		void set(const std::string& name, const bool& value);
		void set(const std::string& name, const float& value);
		void set(const std::string& name, const glm::mat3& value);
		void set(const std::string& name, const glm::mat4& value);

		DataBufferView getBuffer(const std::string& name);
		DataBufferArrayView getArray(const std::string& name);
		void set(const std::string& name, const DataBufferView& value);
		void set(const std::string& name, const DataBufferArrayView& value);

		const DataBufferView& buffer() const;

		void makeDirty();
		bool isDirty() const;
		void clean();
	};
}

