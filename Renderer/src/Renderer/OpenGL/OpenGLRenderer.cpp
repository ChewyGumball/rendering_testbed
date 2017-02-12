#include "Renderer/OpenGL/OpenGLRenderer.h"

#include <algorithm>
#include <utility>

#include "Util/FileUtils.h"
#include "Util/StringUtils.h"

#include "Models/Mesh.h"
#include "Models/Model.h"
#include "Models/ModelInstance.h"
#include "Renderer/Camera.h"
#include "Renderer/OpenGL/OpenGLFrameBuffer.h"
#include "Renderer/OpenGL/OpenGLTextureBuffer.h"

namespace {

std::vector<GLenum> textureUnits{ GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5, GL_TEXTURE6, GL_TEXTURE7, GL_TEXTURE8 };

std::unordered_map<uint32_t, OpenGLRenderMesh>    meshes;
std::unordered_map<uint32_t, OpenGLShader>        shaders;
std::unordered_map<uint32_t, OpenGLRenderModel>   models;
std::unordered_map<uint32_t, OpenGLTextureBuffer> textures;
std::unordered_map<uint32_t, OpenGLFrameBuffer>   frameBuffers;

void addLightUniforms(OpenGLShader& shader, std::vector<PointLight>& lights)
{
    for (int i = 0; i < lights.size(); i++) {
        PointLight& light = lights[i];
        shader.setUniform3f(Util::String::Format("pointLights[%d].position", i), light.position());
        shader.setUniform3f(Util::String::Format("pointLights[%d].intensity", i), light.intensity());
        shader.setUniform1f(Util::String::Format("pointLights[%d].power", i), light.power());
    }
}

void bindTextures(OpenGLShader& shader, std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> modelTextures)
{
    int textureUnit = 0;
    for (auto& texture : modelTextures) {
        glActiveTexture(textureUnits[textureUnit]);
        glBindTexture(GL_TEXTURE_2D, textures[texture.second->id()].handle());
        shader.setUniform1i(texture.first, textureUnit);

        if (textureUnit++ >= textureUnits.size()) {
            break;
        }
    }
}

void reloadTexture(std::shared_ptr<TextureBuffer> texture) {
	textures[texture->id()] = OpenGLTextureBuffer(texture);
}

void createTexture(std::shared_ptr<TextureBuffer> texture)
{
    if (textures.count(texture->id()) == 0) {
        textures.emplace(texture->id(), texture);

		if (texture->isFileTexture()) {
			Util::File::WatchForChanges(texture->filename(), [=]() { reloadTexture(texture); });
		}
    }
}

void createFrameBuffer(std::shared_ptr<const FrameBuffer> frameBuffer)
{
    for (auto target : frameBuffer->targets()) {
        createTexture(target.second);
    }
    if (frameBuffers.count(frameBuffer->id()) == 0) {
        frameBuffers.emplace(frameBuffer->id(), OpenGLFrameBuffer(frameBuffer, textures));
    }
}

void createMesh(std::shared_ptr<const Mesh> mesh)
{
    if (meshes.count(mesh->id()) == 0) {
        meshes.emplace(mesh->id(), mesh);
    }
}

void reloadShader(std::shared_ptr<const Shader> shader) {
	shaders[shader->id()] = OpenGLShader(shader);
}

void createShader(std::shared_ptr<const Shader> shader)
{
    if (shaders.count(shader->id()) == 0) {
		shaders.emplace(shader->id(), shader);

		for (auto& type : shader->filenames()) {
			for (auto& file : type.second) {
				Util::File::WatchForChanges(file, [=]() { reloadShader(shader); });
			}
		}
    }
}

void createModel(std::shared_ptr<const Model> model)
{
    createMesh(model->mesh());
    createShader(model->shader());
    for (auto texture : model->textures()) {
        createTexture(texture.second);
    }

    if (models.count(model->id()) == 0) {
        models.emplace(model->id(), OpenGLRenderModel(meshes[model->mesh()->id()], model->shader()->id(), model->textures()));
    }
}
}

OpenGLRenderer::OpenGLRenderer() {}

OpenGLRenderer::~OpenGLRenderer() {}

void OpenGLRenderer::addFrameBuffer(std::shared_ptr<const FrameBuffer> frameBuffer) { createFrameBuffer(frameBuffer); }

void OpenGLRenderer::addModelInstance(std::shared_ptr<const ModelInstance> modelInstance)
{
    createModel(modelInstance->model());
    modelInstances.emplace_back(modelInstance);
}

void OpenGLRenderer::addPointLight(PointLight light) { lights.push_back(light); }

void OpenGLRenderer::removeModelInstance(std::shared_ptr<const ModelInstance> modelInstance)
{
    modelInstances.erase(std::remove(modelInstances.begin(), modelInstances.end(), modelInstance), modelInstances.end());
}

void OpenGLRenderer::draw(std::shared_ptr<FrameBuffer> frameBuffer, const std::shared_ptr<Camera> camera, RenderOptions& options)
{
    trianglesDrawn = 0;
    std::unordered_map<size_t, std::vector<std::shared_ptr<const ModelInstance>>> culledInstances;
    if (options.cullingEnabled) {
        culledInstances = cull(camera, modelInstances);
    } else {
        for (size_t i = 0; i < modelInstances.size(); ++i) {
            culledInstances[modelInstances[i]->model()->id()].push_back(modelInstances[i]);
        }
    }

    frameBuffers[frameBuffer->id()].bind();

    glViewport(options.viewportOrigin.x, options.viewportOrigin.y, options.viewportDimensions.x, options.viewportDimensions.y);
    glClearColor(options.clearColour.r, options.clearColour.g, options.clearColour.b, options.clearColour.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (options.wireframe) {
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);
    }

    for (auto instanceList : culledInstances) {
        OpenGLRenderModel& model = models[instanceList.first];
        // update transform data and draw models
        std::vector<glm::mat4> transforms;
        for (auto instance : instanceList.second) {
            transforms.push_back(instance->transformMatrix());
            // transforms.push_back(glm::mat4(0));
            trianglesDrawn += instance->triangleCount();
        }
        glNamedBufferData(model.transformVBO(), transforms.size() * sizeof(glm::mat4), transforms.data(), GL_STATIC_DRAW);

        OpenGLShader& shader = shaders[model.shaderID];
        shader.bind();
        bindTextures(shader, model.textures);
        shader.setUniformMatrix4f("view", camera->transform());
        shader.setUniformMatrix4f("projection", camera->projection());
        shader.setUniform3f("cameraPosition", camera->position());
        addLightUniforms(shader, lights);

        model.draw(static_cast<int>(transforms.size()));
    }

    if (options.wireframe) {
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
    }
    frameBuffers[frameBuffer->id()].unbind();
}
