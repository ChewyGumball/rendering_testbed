#pragma once
#include <Buffer/DataBufferArrayView.h>

class DataBufferArray : public DataBufferArrayView {

private:
	std::vector<uint8_t> buffer;

public:
	DataBufferArray(uint64_t elementCount, BufferElementType elementType, std::shared_ptr<BufferFormat> elementFormat = nullptr);
	~DataBufferArray();
};

