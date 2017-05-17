#include "Buffer\DataBuffer.h"

#include <assert.h>
#include <algorithm>

#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Buffer/BufferFormat.h>

using namespace Renderer;

namespace {



	std::vector<uint8_t> initialData(std::shared_ptr<const BufferFormat> format)
	{
		//Save one extra byte for the dirty flag
		std::vector<uint8_t> data(format->size() + 1);

		for (auto& field : format->offsets()) {
			switch (field.second.second) {
			case BufferElementType::MAT4: {
				const uint8_t* matrix = reinterpret_cast<const uint8_t*>(glm::value_ptr(glm::mat4()));
				std::copy(matrix, matrix + format->sizeOfType(BufferElementType::MAT4), data.begin() + field.second.first);
				break;
			}
			case BufferElementType::MAT3: {
				const uint8_t* matrix = reinterpret_cast<const uint8_t*>(glm::value_ptr(glm::mat3()));
				std::copy(matrix, matrix + format->sizeOfType(BufferElementType::MAT3), data.begin() + field.second.first);
				break;
			}
			case BufferElementType::BUFFER: {
				std::vector<uint8_t> bufferData = initialData(format->nestedFormat(field.first));
				std::copy(bufferData.begin(), bufferData.end(), data.begin() + field.second.first);
			}
			}
		}

		return data;
	}
}

//Can't give the view a pointer to our buffer until the buffer is initialized, otherwise it could (will probably) move when 
//	it is filled with data
DataBuffer::DataBuffer(std::shared_ptr<const BufferFormat> format) : DataBufferView(nullptr, format), buffer(initialData(format))
{
	data = buffer.data();
}

DataBuffer::~DataBuffer()
{
}