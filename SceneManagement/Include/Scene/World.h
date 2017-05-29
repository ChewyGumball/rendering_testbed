#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include <Cameras/Camera.h>
#include <Drawing/RenderPass.h>

namespace Scene {
	class ModelGroup;

	class World {
	private:
		std::unordered_map<std::string, std::shared_ptr<ModelGroup>> m_modelInstanceGroups;
		std::unordered_map<std::string, std::shared_ptr<RenderPass>> m_Passes;
		std::unordered_map<std::string, std::shared_ptr<Cameras::Camera>> m_Cameras;
		std::vector<std::shared_ptr<RenderPass>> orderedPasses;

	public:
		World(std::unordered_map<std::string, std::shared_ptr<RenderPass>> passes,
			std::unordered_map<std::string, std::vector<std::string>> passDependencies,
			std::unordered_map<std::string, std::shared_ptr<Cameras::Camera>> cameras,
			std::unordered_map<std::string, std::shared_ptr<ModelGroup>> modelInstances);
		~World();

		std::shared_ptr<Cameras::Camera> camera(std::string name);
		std::shared_ptr<RenderPass> pass(std::string name);
		std::shared_ptr<ModelGroup> modelInstances(std::string name);

		const std::vector<std::shared_ptr<RenderPass>> &passes();
	};
}
