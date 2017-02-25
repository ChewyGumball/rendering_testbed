#pragma once
#include <memory>
#include <Renderer/Model.h>
#include <glm/glm.hpp>
#include <Buffer/DataBuffer.h>

class ModelInstance
{
private:
	std::shared_ptr<Model> baseModel;
	DataBuffer m_instanceData;

public:
	ModelInstance(std::shared_ptr<Model> model);
	~ModelInstance();


	void rotate(glm::vec3 axis, float degrees);
	void translate(glm::vec3 offset);
	void scale(glm::vec3 scales);

	glm::vec3 position() const;

	void model(std::shared_ptr<Model> newModel);
	std::shared_ptr<const Model> model() const;
	DataBufferView& instanceData();
	const DataBufferView& instanceData() const;
	Culling::BoundingSphere bounds() const;
};

