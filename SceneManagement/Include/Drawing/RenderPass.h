#pragma once
#include <vector>

#include <Renderer\IRenderer.h>
#include <Renderer\RenderOptions.h>

#include <Lights\PointLight.h>

namespace Scene::Cameras {
	class Camera;
}

namespace Renderer {
	class ModelInstance;
	class FrameBuffer;
}

namespace Scene {
	class RenderPass
	{
	private:
		std::shared_ptr<Renderer::IRenderer> renderer;
		std::shared_ptr<Cameras::Camera> m_camera;
		Renderer::RenderOptions options;
		bool cullingEnabled;

		std::unordered_map<std::string, std::shared_ptr<Renderer::ShaderConstantBuffer>> passConstants;
		std::vector<std::shared_ptr<const Renderer::ModelInstance>> m_opaqueInstances;
		std::vector<std::shared_ptr<const Renderer::ModelInstance>> m_transparentInsances;
		std::vector<PointLight> lights;

		uint64_t drawInstances(const std::vector<std::shared_ptr<const Renderer::ModelInstance>>& instances, Renderer::SortType sortType);

	public:
		RenderPass(std::shared_ptr<Renderer::IRenderer> renderer);
		~RenderPass();

		//Returns how many triangles were drawn (after culling if culling is enabled)
		uint64_t draw();

		void clearBuffers(bool enabled);
		void clearColour(glm::vec4 colour);
		void depthTest(bool enabled);
		void cull(bool enabled);
		void wireframe(bool enabled);
		void camera(std::shared_ptr<Cameras::Camera> c);
		void frameBuffer(std::shared_ptr<Renderer::FrameBuffer> buffer);

		void addModelInstances(std::vector<std::shared_ptr<Renderer::ModelInstance>> modelInstances);
		void addModelInstance(std::shared_ptr<Renderer::ModelInstance> modelInstance);
		void addPointLight(PointLight light);

		void removeModelInstances(std::vector<std::shared_ptr<Renderer::ModelInstance>> modelInstances);
		void removeModelInstance(std::shared_ptr<Renderer::ModelInstance> modelInstance);

		void viewport(glm::vec2 origin, glm::vec2 size);
	};
}
