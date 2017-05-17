#pragma once
#include <vector>

#include <Buffer/DataBufferView.h>

namespace Renderer {
	class BufferFormat;

	class DataBuffer : public DataBufferView {

	private:
		std::vector<uint8_t> buffer;

	public:
		DataBuffer(std::shared_ptr<const BufferFormat> format);
		~DataBuffer();
	};
}
