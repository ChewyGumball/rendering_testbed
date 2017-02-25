#include "Buffer/DataBufferArray.h"

#include <glm/gtc/type_ptr.hpp>

namespace {

	std::vector<uint8_t> initialData(const std::shared_ptr<BufferFormat> format)
	{
		std::vector<uint8_t> data(format->size());

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

DataBufferArray::DataBufferArray(uint64_t elementCount, BufferElementType elementType, std::shared_ptr<BufferFormat> elementFormat) : DataBufferArrayView(nullptr, elementCount, elementType, elementFormat), buffer(initialData(elementFormat))
{
	data = buffer.data();
}


DataBufferArray::~DataBufferArray()
{
}
