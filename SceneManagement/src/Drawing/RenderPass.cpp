#include "Drawing/RenderPass.h"
#include <iostream>
#include <unordered_set>

#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Cameras/Camera.h"
#include "Resources/Model.h"
#include "Resources/Mesh.h"
#include "Resources/ModelInstance.h"
#include "Resources/Material.h"
#include "Resources/ShaderConstantBuffer.h"
#include "Resources/FrameBuffer.h"
#include "Culling/Frustum.h"
#include "Culling/BoundingSphere.h"
#include <Buffer/DataBufferArrayView.h>
#include <Buffer/BufferFormat.h>

using namespace Renderer;

namespace {
	Culling::BoundingSphere modelInstanceBounds(std::shared_ptr<const ModelInstance> instance) {
		Culling::BoundingSphere meshBounds = instance->model()->mesh()->bounds();
		return Culling::BoundingSphere(glm::vec3(instance->instanceData().getMat4("transform") * glm::vec4(meshBounds.center(), 1)), meshBounds.radius());
	}


	std::unordered_map<std::string, std::shared_ptr<BufferFormat>> systemBufferFormats;

	std::unordered_map<std::shared_ptr<const Model>, std::vector<std::shared_ptr<const ModelInstance>>> cullAgainstCameraFrustum(const std::shared_ptr<Scene::Cameras::Camera> camera, std::vector<std::shared_ptr<const ModelInstance>> instances) {
		Culling::Frustum frustum(camera->projection() * camera->transform());

		std::vector<glm::vec3> centers(instances.size());
		std::vector<float>     radii(instances.size());

		for (size_t i = 0; i < instances.size(); ++i) {
			Culling::BoundingSphere s = modelInstanceBounds(instances[i]);
			centers[i] = s.center();
			radii[i] = s.radius();
		}

		std::vector<bool> intersections = frustum.intersects(centers, radii);

		std::unordered_map<std::shared_ptr<const Model>, std::vector<std::shared_ptr<const ModelInstance>>> visibleModels;

		for (size_t i = 0; i < instances.size(); ++i) {
			if (intersections[i]) {
				visibleModels[instances[i]->model()].push_back(instances[i]);
			}
		}

		return visibleModels;
	}

	void updatePassConstantBuffers(std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>> passConstants, std::shared_ptr<Scene::Cameras::Camera> camera, std::vector<PointLight>& lights) {
		auto cameraBuffer = passConstants["camera"];
		cameraBuffer->set("projection", camera->projection());
		cameraBuffer->set("view", camera->transform());
		cameraBuffer->set("position", camera->position());

		if (lights.size() > 0) {
			auto& lightsBuffer = passConstants["lights"]->getArray("pointLights");
			for (int i = 0; i < 2; i++) {
				PointLight light = lights[i];
				auto& lightBuffer = lightsBuffer.getBufferAt(i);
				lightBuffer.set("position", light.position());
				lightBuffer.set("intensity", light.intensity());
				lightBuffer.set("power", light.power());
			}

			passConstants["lights"]->makeDirty();
		}
	}
	bool isInitialized = false;
	void initialize() {
		if (isInitialized) return;

		isInitialized = true;
		systemBufferFormats = std::unordered_map<std::string, std::shared_ptr<BufferFormat>>{
			{"camera", std::make_shared<BufferFormat>(
				std::vector<std::pair<std::string, BufferElementType>> {
					std::make_pair("projection", BufferElementType::MAT4),
					std::make_pair("view", BufferElementType::MAT4),
					std::make_pair("position", BufferElementType::FLOAT_VEC3)
				},
				std::unordered_map<std::string, std::shared_ptr<const BufferFormat>>(),
				BufferPackingType::OPENGL_STD140
			)},
			{ "lights", std::make_shared<BufferFormat>(
				std::vector<std::pair<std::string, BufferElementType>> {
						std::make_pair("pointLights", BufferElementType::ARRAY)
				},
				std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> {
					{ "pointLights", std::make_shared<const BufferFormat>(
						2,
						std::make_shared<BufferFormat>(
							std::vector<std::pair<std::string, BufferElementType>> {
								std::make_pair("position", BufferElementType::FLOAT_VEC3),
								std::make_pair("intensity", BufferElementType::FLOAT_VEC3),
								std::make_pair("power", BufferElementType::FLOAT_SCALAR)
							},
							std::unordered_map<std::string, std::shared_ptr<const BufferFormat>>(),
							BufferPackingType::OPENGL_STD140
						)
					)}
				},
				BufferPackingType::OPENGL_STD140
			)}
		};
	}
}

namespace Scene {
	RenderPass::RenderPass(std::shared_ptr<Renderer::IRenderer> renderer) : renderer(renderer), m_camera(std::make_shared<Scene::Cameras::Camera>()), options(RenderOptions()), cullingEnabled(false)
	{
		initialize();
		for (auto& constant : systemBufferFormats) {
			passConstants.emplace(constant.first, std::make_shared<ShaderConstantBuffer>(constant.second));
		}
	}

	RenderPass::~RenderPass()
	{}

	uint64_t RenderPass::draw()
	{
		uint64_t trianglesDrawn = 0;

		std::unordered_map<std::shared_ptr<const Model>, std::vector<std::shared_ptr<const ModelInstance>>> culledInstances;

		//Cull (or not) instances against the camera
		if (cullingEnabled) {
			culledInstances = cullAgainstCameraFrustum(m_camera, m_modelInstances);
		}
		else {
			for (auto instance : m_modelInstances) {
				culledInstances[instance->model()].push_back(instance);
			}
		}

		//Set up renderer with rendering options
		renderer->processRenderingOptions(options);

		updatePassConstantBuffers(passConstants, m_camera, lights);
		std::unordered_set<std::shared_ptr<ShaderConstantBuffer>> constantBuffersToUpdate;
		//update system constant buffer data
		for (auto s : passConstants) {
			//insert camera and shit into the buffers
			constantBuffersToUpdate.insert(s.second);
		}

		//Update shader constant buffers
		for (auto& instanceList : culledInstances) {
			for (auto& shaderConstant : instanceList.first->material()->constants()) {
				if (shaderConstant.second->isDirty()) {
					constantBuffersToUpdate.insert(shaderConstant.second);
				}
			}
		}

		renderer->updateConstantBuffers(constantBuffersToUpdate);

		for (auto shaderConstantBuffer : constantBuffersToUpdate) {
			shaderConstantBuffer->clean();
		}

		//Draw instances
		for (auto& instanceList : culledInstances) {
			renderer->draw(instanceList.second, passConstants);
			trianglesDrawn += instanceList.second.size() * instanceList.second[0]->model()->triangleCount();
		}

		return trianglesDrawn;
	}

	void RenderPass::clearBuffers(bool enabled)
	{
		options.clearBuffers = enabled;
	}

	void RenderPass::clearColour(glm::vec4 colour)
	{
		options.clearColour = colour;
	}

	void RenderPass::depthTest(bool enabled)
	{
		options.depthTest = enabled;
	}

	void RenderPass::cull(bool enabled)
	{
		cullingEnabled = enabled;
	}

	void RenderPass::wireframe(bool enabled)
	{
		options.wireframe = enabled;
	}

	void RenderPass::camera(std::shared_ptr<Scene::Cameras::Camera> c)
	{
		m_camera = c;
	}

	void RenderPass::frameBuffer(std::shared_ptr<FrameBuffer> buffer)
	{
		options.frameBuffer = buffer;
	}

	void RenderPass::addModelInstances(std::vector<std::shared_ptr<ModelInstance>> modelInstances)
	{
		for (auto modelInstance : modelInstances) {
			m_modelInstances.push_back(modelInstance);
		}
	}

	void RenderPass::addModelInstance(std::shared_ptr<ModelInstance> modelInstance) {
		addModelInstances({ modelInstance });
	}

	void RenderPass::addPointLight(PointLight light) {
		lights.push_back(light);
	}

	void RenderPass::removeModelInstances(std::vector<std::shared_ptr<ModelInstance>> modelInstances)
	{
		for (auto modelInstance : modelInstances) {
			m_modelInstances.erase(std::remove(m_modelInstances.begin(), m_modelInstances.end(), modelInstance), m_modelInstances.end());
		}
	}

	void RenderPass::removeModelInstance(std::shared_ptr<ModelInstance> modelInstance) {
		removeModelInstances({ modelInstance });
	}


	void RenderPass::viewport(glm::vec2 origin, glm::vec2 size)
	{
		options.viewportOrigin = origin;
		options.viewportDimensions = size;
	}
}
