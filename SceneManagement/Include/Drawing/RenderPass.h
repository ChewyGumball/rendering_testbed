#pragma once
#include <vector>

#include <Renderer\Renderer.h>
#include <Renderer\RenderOptions.h>

#include <Lights\PointLight.h>

namespace Scene::Cameras {
	class Camera;
}
class ModelInstance;
class FrameBuffer;

namespace Scene {
	class RenderPass
	{
	private:
		Renderer *renderer;
		std::shared_ptr<Cameras::Camera> m_camera;
		RenderOptions options;
		bool cullingEnabled;

		std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>> passConstants;
		std::vector<std::shared_ptr<const ModelInstance>> m_modelInstances;
		std::vector<PointLight> lights;

	public:
		RenderPass();
		~RenderPass();

		//Returns how many triangles were drawn (after culling if culling is enabled)
		uint64_t draw();

		void clearBuffers(bool enabled);
		void clearColour(glm::vec4 colour);
		void depthTest(bool enabled);
		void cull(bool enabled);
		void wireframe(bool enabled);
		void camera(std::shared_ptr<Cameras::Camera> c);
		void frameBuffer(std::shared_ptr<FrameBuffer> buffer);

		void addModelInstances(std::vector<std::shared_ptr<ModelInstance>> modelInstances);
		void addModelInstance(std::shared_ptr<ModelInstance> modelInstance);
		void addPointLight(PointLight light);

		void removeModelInstances(std::vector<std::shared_ptr<ModelInstance>> modelInstances);
		void removeModelInstance(std::shared_ptr<ModelInstance> modelInstance);

		void viewport(glm::vec2 origin, glm::vec2 size);
	};
}
