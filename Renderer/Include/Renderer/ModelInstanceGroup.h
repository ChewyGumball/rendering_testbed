#pragma once

#include <vector>
#include <unordered_map>

#include <Renderer/Model.h>
#include <Renderer/ModelInstance.h>
#include <Renderer/ShaderConstantBuffer.h>

class ModelInstanceGroup
{
private:
	const std::shared_ptr<Model> m_model;
	std::vector<std::shared_ptr<ModelInstance>> m_modelInstances;
	const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>> m_shaderConstantBuffers;

public:
	ModelInstanceGroup(std::shared_ptr<Model> model, const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>> shaderConstantBuffers);
	~ModelInstanceGroup();

	void addModelInstance(std::shared_ptr<ModelInstance> modelInstance);
	void removeModelInstance(std::shared_ptr<ModelInstance> modelInstance);

	void addModelInstances(std::vector<std::shared_ptr<ModelInstance>> modelInstances);
	void removeModelInstances(std::vector<std::shared_ptr<ModelInstance>> modelInstances);

	const std::shared_ptr<Model> model() const;
	const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>>& shaderConstantBuffers() const;
	const std::vector<std::shared_ptr<ModelInstance>>& modelInstances() const;
};

