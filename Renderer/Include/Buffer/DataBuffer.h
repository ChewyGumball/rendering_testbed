#pragma once
#include <vector>

#include <glm\glm.hpp>

#include <Buffer/BufferFormat.h>
#include <Buffer/DataBufferView.h>

//typedef uint16_t DataBufferOffset;

class DataBuffer : public DataBufferView {

private:
	std::vector<uint8_t> buffer;

public:
	DataBuffer(const std::shared_ptr<BufferFormat> format);
	~DataBuffer();
};
