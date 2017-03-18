#pragma once
#include <glm\glm.hpp>

#include <Buffer/BufferFormat.h>
#include <Buffer/DataBufferView.h>

class DataBufferArrayView
{
protected:
	BufferElementType m_type;
	std::shared_ptr<const BufferFormat> m_elementFormat;
	uint8_t* data;
	uint64_t m_count;
public:
	DataBufferArrayView(uint8_t* data, uint64_t elementCount, BufferElementType elementType, std::shared_ptr<const BufferFormat> elementFormat = nullptr);
	~DataBufferArrayView();

	BufferElementType elementType() const;
	uint64_t elementCount() const;

	void setAt(uint64_t index, const glm::vec2& value);
	void setAt(uint64_t index, const glm::vec3& value);
	void setAt(uint64_t index, const glm::vec4& value);
	void setAt(uint64_t index, const uint32_t& value);
	void setAt(uint64_t index, const int32_t& value);
	void setAt(uint64_t index, const bool& value);
	void setAt(uint64_t index, const float& value);
	void setAt(uint64_t index, const glm::mat3& value);
	void setAt(uint64_t index, const glm::mat4& value);
	void setAt(uint64_t index, const DataBufferView& value);
	void setAt(uint64_t index, const DataBufferArrayView& value);

	const glm::vec2& getVec2At(uint64_t index) const;
	const glm::vec3& getVec3At(uint64_t index) const;
	const glm::vec4& getVec4At(uint64_t index) const;
	const uint32_t& getUIntAt(uint64_t index) const;
	const int32_t& getIntAt(uint64_t index) const;
	const bool& getBoolAt(uint64_t index) const;
	const float& getFloatAt(uint64_t index) const;
	const glm::mat3& getMat3At(uint64_t index) const;
	const glm::mat4& getMat4At(uint64_t index) const;

	const DataBufferView getBufferAt(uint64_t index) const;
	DataBufferView getBufferAt(uint64_t index);

	const DataBufferArrayView getArrayAt(uint64_t index) const;
	DataBufferArrayView getArrayAt(uint64_t index);

	uint8_t* begin();
	uint8_t* end();
	const uint8_t* begin() const;
	const uint8_t* end() const;
};

