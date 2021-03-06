#include "Scene/World.h"

#include <algorithm>

namespace {

void linearize(std::string current, std::unordered_map<std::string, std::vector<std::string>>& dependencies, std::vector<std::string>& linearized)
{
    if (std::find(linearized.begin(), linearized.end(), current) == linearized.end()) {
        for (std::string& dependency : dependencies[current]) {
            linearize(dependency, dependencies, linearized);
        }
        linearized.push_back(current);
    }
}

std::vector<std::string> linearizeDependencies(std::unordered_map<std::string, std::vector<std::string>>& dependencies)
{
    std::vector<std::string> linearized;
    for (auto& key : dependencies) {
        linearize(key.first, dependencies, linearized);
    }
    return linearized;
}
}

namespace Scene {

	World::World(std::unordered_map<std::string, std::shared_ptr<RenderPass>>        passes,
		std::unordered_map<std::string, std::vector<std::string>>                    passDependencies,
		std::unordered_map<std::string, std::shared_ptr<Cameras::Camera>>                     cameras,
		std::unordered_map<std::string, std::shared_ptr<Scene::ModelGroup>> modelInstances)
		: m_Passes(passes), m_Cameras(cameras), m_modelInstanceGroups(modelInstances)
	{
		std::vector<std::string> linearizedPasses = linearizeDependencies(passDependencies);
		for (std::string pass : linearizedPasses) {
			orderedPasses.push_back(passes[pass]);
		}

		for (auto& pass : passes) {
			if (std::find(linearizedPasses.begin(), linearizedPasses.end(), pass.first) == linearizedPasses.end()) {
				orderedPasses.push_back(pass.second);
			}
		}
	}

	World::~World() {}

	std::shared_ptr<Cameras::Camera> World::camera(std::string name) { return m_Cameras[name]; }

	std::shared_ptr<RenderPass> World::pass(std::string name) { return m_Passes[name]; }

	std::shared_ptr<Scene::ModelGroup> World::modelInstances(std::string name) { return m_modelInstanceGroups[name]; }

	const std::vector<std::shared_ptr<RenderPass>>& World::passes() { return orderedPasses; }
}
