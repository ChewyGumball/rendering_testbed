#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

namespace Renderer {
	class Model;
	class ModelInstance;
	class Shader;
}

namespace Scene {
	typedef uint32_t ShaderAttributeID;

	namespace ShaderAttribute {
		extern const ShaderAttributeID Transform;

		//Registers a shader attribute and sets the default value if it doesn't already exists. 
		//Returns the new or existing ShaderAttributeID.
		ShaderAttributeID Register(const std::string& name, const std::string& defaultValue);
		ShaderAttributeID Named(const std::string& name);
	}

	class ModelGroup
	{
	private:
		std::vector<std::shared_ptr<Renderer::ModelInstance>> m_instances;
	public:
		ModelGroup();
		ModelGroup(size_t reservedSize);
		~ModelGroup();

		const std::vector<std::shared_ptr<Renderer::ModelInstance>>& instances() const;

		std::shared_ptr<Renderer::ModelInstance> addInstance(std::shared_ptr<Renderer::Model> model);
		void addModelGroup(ModelGroup& other);

		void removeInstance(std::shared_ptr<Renderer::ModelInstance> instance);
		void removeInstances(const std::vector<std::shared_ptr<Renderer::ModelInstance>>& instances);

		void translate(ShaderAttributeID attribute, glm::vec3 offset);
		void rotate(ShaderAttributeID attribute, glm::vec3 axis, float angle);
		void scale(ShaderAttributeID attribute, glm::vec3 amount);

		static void RegisterShaderAttributeMappings(std::shared_ptr<const Renderer::Shader> shader, std::unordered_map<ShaderAttributeID, std::string> mappings);
	};
}

