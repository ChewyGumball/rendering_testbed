#pragma once
#include <glm\glm.hpp>

#include <Buffer/BufferFormat.h>

class DataBufferArrayView;

class DataBufferView
{
protected:
	const std::shared_ptr<BufferFormat> m_format;
	uint8_t* data;

public:
	DataBufferView(uint8_t* data, const std::shared_ptr<BufferFormat> format);
	~DataBufferView();

	const std::shared_ptr<BufferFormat> format() const;

	void set(std::string name, const glm::vec2& value);
	void set(std::string name, const glm::vec3& value);
	void set(std::string name, const glm::vec4& value);
	void set(std::string name, const uint32_t& value);
	void set(std::string name, const int32_t& value);
	void set(std::string name, const bool& value);
	void set(std::string name, const float& value);
	void set(std::string name, const glm::mat3& value);
	void set(std::string name, const glm::mat4& value);
	void set(std::string name, const DataBufferView& value);
	void set(std::string name, const DataBufferArrayView& value);

	const glm::vec2& getVec2(std::string name) const;
	const glm::vec3& getVec3(std::string name) const;
	const glm::vec4& getVec4(std::string name) const;
	const uint32_t& getUInt(std::string name) const;
	const int32_t& getInt(std::string name) const;
	const bool& getBool(std::string name) const;
	const float& getFloat(std::string name) const;
	const glm::mat3& getMat3(std::string name) const;
	const glm::mat4& getMat4(std::string name) const;

	const DataBufferView getBuffer(std::string name) const;
	DataBufferView getBuffer(std::string name);

	const DataBufferArrayView getArray(std::string name) const;
	DataBufferArrayView getArray(std::string name);

	uint8_t* begin();
	uint8_t* end();
	const uint8_t* begin() const;
	const uint8_t* end() const;
};

