#pragma once
#include <memory>
#include <string>
#include <glm\glm.hpp>

namespace Renderer {

	class DataBufferArrayView;
	class BufferFormat;

	class DataBufferView
	{
	protected:
		std::shared_ptr<const BufferFormat> m_format;
		uint8_t* data;

	public:
		DataBufferView(uint8_t* data, std::shared_ptr<const BufferFormat> format);
		~DataBufferView();

		std::shared_ptr<const BufferFormat> format() const;

		void set(const std::string& name, const glm::vec2& value);
		void set(const std::string& name, const glm::vec3& value);
		void set(const std::string& name, const glm::vec4& value);
		void set(const std::string& name, const uint32_t& value);
		void set(const std::string& name, const int32_t& value);
		void set(const std::string& name, const bool& value);
		void set(const std::string& name, const float& value);
		void set(const std::string& name, const glm::mat3& value);
		void set(const std::string& name, const glm::mat4& value);
		void set(const std::string& name, const DataBufferView& value);
		void set(const std::string& name, const DataBufferArrayView& value);

		const glm::vec2& getVec2(const std::string& name) const;
		const glm::vec3& getVec3(const std::string& name) const;
		const glm::vec4& getVec4(const std::string& name) const;
		const uint32_t& getUInt(const std::string& name) const;
		const int32_t& getInt(const std::string& name) const;
		const bool& getBool(const std::string& name) const;
		const float& getFloat(const std::string& name) const;
		const glm::mat3& getMat3(const std::string& name) const;
		const glm::mat4& getMat4(const std::string& name) const;

		const DataBufferView getBuffer(const std::string& name) const;
		DataBufferView getBuffer(const std::string& name);

		const DataBufferArrayView getArray(const std::string& name) const;
		DataBufferArrayView getArray(const std::string& name);

		uint8_t* begin();
		uint8_t* end();
		const uint8_t* begin() const;
		const uint8_t* end() const;

		void markDirty();
		bool isDirty() const;
		void clean();
	};
}
