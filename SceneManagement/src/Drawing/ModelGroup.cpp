#include "Drawing/ModelGroup.h"

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

#include <Resources/ModelInstance.h>
#include <Resources/Model.h>
#include <Resources/Material.h>
#include <Resources/Shader.h>

namespace {
	uint32_t nextShaderAttributeID = 1;

	std::unordered_map<std::string, Scene::ShaderAttributeID> attributeNames;
	std::unordered_map<std::shared_ptr<const Renderer::Shader>, std::unordered_map<Scene::ShaderAttributeID, std::string>> shaderAttributeMappings;

	std::unordered_map<Scene::ShaderAttributeID, std::string> defaultMappings = {
		{0, "transform"}
	};

	const std::string& mapAttribute(Scene::ShaderAttributeID attribute, std::shared_ptr<const Renderer::Shader> shader) {
		auto& it = shaderAttributeMappings.find(shader);
		if (it != shaderAttributeMappings.end()) {
			auto& name = it->second.find(attribute);
			if (name != it->second.end()) {
				return name->second;
			}
		}

		assert(defaultMappings.count(attribute) > 0);
		return defaultMappings[attribute];
	}
}

namespace Scene {

	namespace ShaderAttribute {
		const ShaderAttributeID Transform = 0;

		ShaderAttributeID Register(const std::string& name, const std::string& defaultValue)
		{
			auto& it = attributeNames.find(name);
			if (it == attributeNames.end()) {
				ShaderAttributeID newID = nextShaderAttributeID++;

				attributeNames.emplace(name, newID);
				defaultMappings[newID] = defaultValue;

				return newID;
			}

			return it->second;
		}

		ShaderAttributeID Named(const std::string& name) {
			assert(attributeNames.count(name) != 0);
			return attributeNames[name];
		}
	}

	ModelGroup::ModelGroup()
	{
	}

	ModelGroup::ModelGroup(size_t reservedSize)
	{
		m_instances.reserve(reservedSize);
	}


	ModelGroup::~ModelGroup()
	{
	}

	const std::vector<std::shared_ptr<Renderer::ModelInstance>>& ModelGroup::instances() const
	{
		return m_instances;
	}

	std::shared_ptr<Renderer::ModelInstance> ModelGroup::addInstance(std::shared_ptr<Renderer::Model> model)
	{
		std::shared_ptr<Renderer::ModelInstance> instance = std::make_shared<Renderer::ModelInstance>(model);
		m_instances.push_back(instance);
		return instance;
	}

	void ModelGroup::addModelGroup(ModelGroup & other)
	{
		auto& otherInsances = other.instances();
		m_instances.reserve(m_instances.size() + otherInsances.size());
		m_instances.insert(m_instances.end(), otherInsances.begin(), otherInsances.end());
	}

	void ModelGroup::removeInstance(std::shared_ptr<Renderer::ModelInstance> instance)
	{
		m_instances.erase(std::remove(m_instances.begin(), m_instances.end(), instance), m_instances.end());
	}

	void ModelGroup::removeInstances(const std::vector<std::shared_ptr<Renderer::ModelInstance>>& instances)
	{
		auto end = m_instances.end();
		for (auto instance : instances) {
			end = std::remove(m_instances.begin(), end, instance);
		}

		m_instances.erase(end, m_instances.end());
	}

	void ModelGroup::translate(ShaderAttributeID attribute, glm::vec3 offset)
	{
		for (auto instance : m_instances) {
			const std::string& name = mapAttribute(attribute, instance->model()->material()->shader());
			instance->instanceData().translate(name, offset);
		}
	}

	void ModelGroup::rotate(ShaderAttributeID attribute, glm::vec3 axis, float angle)
	{
		for (auto instance : m_instances) {
			const std::string& name = mapAttribute(attribute, instance->model()->material()->shader());
			instance->instanceData().rotate(name, axis, angle);
		}
	}

	void ModelGroup::scale(ShaderAttributeID attribute, glm::vec3 amount)
	{
		for (auto instance : m_instances) {
			const std::string& name = mapAttribute(attribute, instance->model()->material()->shader());
			instance->instanceData().scale(name, amount);
		}
	}

	void ModelGroup::RegisterShaderAttributeMappings(std::shared_ptr<const Renderer::Shader> shader, std::unordered_map<ShaderAttributeID, std::string> mappings)
	{
		shaderAttributeMappings.emplace(shader, mappings);
	}
}
