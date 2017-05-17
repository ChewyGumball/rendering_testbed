#pragma once
#include <memory>
#include <Resources/Model.h>
#include <glm/glm.hpp>
#include <Buffer/DataBuffer.h>
#include <Culling/BoundingSphere.h>

namespace Renderer {
	class ModelInstance
	{
	private:
		std::shared_ptr<Model> baseModel;
		DataBuffer m_instanceData;

	public:
		ModelInstance(std::shared_ptr<Model> model);
		~ModelInstance();

		void model(std::shared_ptr<Model> newModel);
		std::shared_ptr<const Model> model() const;
		DataBufferView& instanceData();
		const DataBufferView& instanceData() const;
	};
}
