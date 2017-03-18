#include "Renderer/ModelInstanceGroup.h"


ModelInstanceGroup::ModelInstanceGroup(std::shared_ptr<Model> model, const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>> shaderConstantBuffers)
	:m_model(model), m_shaderConstantBuffers(shaderConstantBuffers)
{
}

ModelInstanceGroup::~ModelInstanceGroup()
{
}

void ModelInstanceGroup::addModelInstance(std::shared_ptr<ModelInstance> modelInstance)
{
	addModelInstances({ modelInstance });
}

void ModelInstanceGroup::removeModelInstance(std::shared_ptr<ModelInstance> modelInstance)
{
	removeModelInstances({ modelInstance });
}

void ModelInstanceGroup::addModelInstances(std::vector<std::shared_ptr<ModelInstance>> modelInstances)
{
}

void ModelInstanceGroup::removeModelInstances(std::vector<std::shared_ptr<ModelInstance>> modelInstances)
{
}

const std::shared_ptr<Model> ModelInstanceGroup::model() const
{
	return m_model;
}

const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>>& ModelInstanceGroup::shaderConstantBuffers() const
{
	return m_shaderConstantBuffers;
}

const std::vector<std::shared_ptr<ModelInstance>>& ModelInstanceGroup::modelInstances() const
{
	return m_modelInstances;
}
