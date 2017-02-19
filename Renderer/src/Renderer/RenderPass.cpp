#include "Renderer/RenderPass.h"

#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Renderer/Camera.h"
#include "Renderer/ModelInstance.h"
#include "Renderer/FrameBuffer.h"
#include "Culling/Frustum.h"


namespace {
	std::unordered_map<RenderResourceID, std::vector<std::shared_ptr<const ModelInstance>>> cullAgainstCameraFrustum(const std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<const ModelInstance>> instances) {
		Culling::Frustum frustum(camera->projection() * camera->transform());

		std::vector<glm::vec3> centers(instances.size());
		std::vector<float>     radii(instances.size());

		for (size_t i = 0; i < instances.size(); ++i) {
			Culling::BoundingSphere s = instances[i]->bounds();
			centers[i] = s.center();
			radii[i] = s.radius();
		}

		std::vector<bool> intersections = frustum.intersects(centers, radii);

		std::unordered_map<RenderResourceID, std::vector<std::shared_ptr<const ModelInstance>>> visibleModels;

		for (size_t i = 0; i < instances.size(); ++i) {
			if (intersections[i]) {
				visibleModels[instances[i]->model()->id()].push_back(instances[i]);
			}
		}

		return visibleModels;
	}
}

RenderPass::RenderPass() : renderer(new OpenGLRenderer()), m_camera(std::make_shared<Camera>()), options(RenderOptions()), cullingEnabled(false) {}

RenderPass::~RenderPass()
{
    delete renderer;
}

uint64_t RenderPass::draw()
{
	uint64_t trianglesDrawn = 0;

	std::unordered_map<RenderResourceID, std::vector<std::shared_ptr<const ModelInstance>>> culledInstances;
	if (cullingEnabled) {
		culledInstances = cullAgainstCameraFrustum(m_camera, m_modelInstances);
	}
	else {
		for (size_t i = 0; i < m_modelInstances.size(); ++i) {
			culledInstances[m_modelInstances[i]->model()->id()].push_back(m_modelInstances[i]);
		}
	}

	renderer->processRenderingOptions(options);

	for (auto instanceList : culledInstances) {
		renderer->draw(instanceList.second, m_camera);
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

void RenderPass::camera(std::shared_ptr<Camera> c)
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

void RenderPass::addPointLight(PointLight light) { renderer->addPointLight(light); }

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
