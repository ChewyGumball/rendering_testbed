#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include <Renderer/Camera.h>
#include <Renderer/RenderPass.h>

class Scene {
  private:
    std::unordered_map<std::string, std::vector<std::shared_ptr<ModelInstance>>> m_modelInstanceGroups;
    std::unordered_map<std::string, std::shared_ptr<RenderPass>> m_Passes;
    std::unordered_map<std::string, Camera> m_Cameras;
    std::vector<std::shared_ptr<RenderPass>> orderedPasses;

  public:
    Scene(std::unordered_map<std::string, std::shared_ptr<RenderPass>> passes,
          std::unordered_map<std::string, std::vector<std::string>> passDependencies,
          std::unordered_map<std::string, Camera> cameras,
          std::unordered_map<std::string, std::vector<std::shared_ptr<ModelInstance>>> modelInstances);
    ~Scene();

    Camera &camera(std::string name);
	std::shared_ptr<RenderPass> pass(std::string name);
    std::vector<std::shared_ptr<ModelInstance>> &modelInstances(std::string name);

    const std::vector<std::shared_ptr<RenderPass>> &passes();
};
