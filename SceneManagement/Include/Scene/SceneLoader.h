#pragma once
#include <vector>
#include <Scene\World.h>
#include <Renderer/IRenderer.h>

namespace Scene::SceneLoader {
	World loadWorld(std::shared_ptr<Renderer::IRenderer> renderer, std::string filename);
}

