#include "Renderer/RenderPass.h"
#include <Renderer\OpenGL\OpenGLRenderer.h>

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
		culledInstances = cullAgainstCameraFrustum(m_camera, modelInstances);
	}
	else {
		for (size_t i = 0; i < modelInstances.size(); ++i) {
			culledInstances[modelInstances[i]->model()->id()].push_back(modelInstances[i]);
		}
	}

	if (m_clearBuffers) {
		renderer->clearFrameBuffer(options.frameBuffer, m_clearColour);
	}

	for (auto instanceList : culledInstances) {
		renderer->draw(instanceList.second, m_camera, options);
		trianglesDrawn += instanceList.second.size() * instanceList.second[0]->model()->triangleCount();
	}

	return trianglesDrawn;
}

void RenderPass::clearBuffers(bool enabled)
{
	m_clearBuffers = enabled;
}

void RenderPass::clearColour(glm::vec4 colour)
{
	m_clearColour = colour;
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
	renderer->addFrameBufferResources(buffer);
}

void RenderPass::addModelInstance(std::shared_ptr<const ModelInstance> modelInstance) { 
	modelInstances.push_back(modelInstance);
	renderer->addModelInstanceResources(modelInstance); 
}

void RenderPass::addPointLight(PointLight light) { renderer->addPointLight(light); }

void RenderPass::removeModelInstance(std::shared_ptr<const ModelInstance> modelInstance) { 
	modelInstances.erase(std::remove(modelInstances.begin(), modelInstances.end(), modelInstance), modelInstances.end());
}


void RenderPass::viewport(glm::vec2 origin, glm::vec2 size)
{	
	options.viewportOrigin = origin;
	options.viewportDimensions = size;
}
