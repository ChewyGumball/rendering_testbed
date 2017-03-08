#pragma once
#include <Renderer/RenderResource.h>
#include <Buffer/DataBuffer.h>

class ShaderConstantBuffer : public RenderResource
{
private:
	bool m_dirty;
	std::string m_name;
	DataBuffer m_buffer;
public:
	ShaderConstantBuffer(std::string name, std::shared_ptr<BufferFormat> format = std::make_shared<BufferFormat>());
	~ShaderConstantBuffer();

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

	const std::string& name() const;
	const DataBufferView& buffer() const;

	bool isDirty() const;
	void clean();
};

