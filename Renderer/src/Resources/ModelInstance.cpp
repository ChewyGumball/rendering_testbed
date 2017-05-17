#include <algorithm>
#include <Resources/ModelInstance.h>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Buffer/BufferFormat.h>
#include <glm/gtc/type_ptr.hpp>

#include <Resources/Material.h>
#include <Resources/Mesh.h>
#include <Resources/Shader.h>

namespace Renderer {
	ModelInstance::ModelInstance(std::shared_ptr<Model> model) : baseModel(model), m_instanceData(model->material()->shader()->instanceStateFormat())
	{
	}

	ModelInstance::~ModelInstance()
	{
	}
	void ModelInstance::model(std::shared_ptr<Model> newModel)
	{
		baseModel = newModel;
	}
	std::shared_ptr<const Model> ModelInstance::model() const { return baseModel; }

	DataBufferView& ModelInstance::instanceData()
	{
		return m_instanceData;
	}

	const DataBufferView& ModelInstance::instanceData() const
	{
		return m_instanceData;
	}
}
