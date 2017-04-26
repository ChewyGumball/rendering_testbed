#include "Scene/SceneLoader.h"

#include <iostream>
#include <string>

#include <Resources/Shader.h>
#include <Resources/Material.h>
#include <Cameras/QuaternionCamera.h>
#include <Resources/Model.h>
#include <Resources/ModelInstance.h>

#include <Buffer/BufferFormat.h>

#include <lib/rapidjson/document.h>

#include <ModelLoader.h>
#include <Util/FileUtils.h>
#include <Util/StringUtils.h>

using namespace Renderer;

namespace {
std::unordered_map<std::string, TextureFormat> textureFormats{ { "rgb", TextureFormat::RGB8 }, { "depth", TextureFormat::DEPTH } };
std::unordered_map<std::string, VertexFormat>  vertexFormats{ { "Position_Normal", VertexFormats::Position_Normal },
    { "Position_Texture", VertexFormats::Position_Texture } };
std::unordered_map<std::string, BufferElementType> elementTypes{
	{ "buffer", BufferElementType::BUFFER},
	{ "array", BufferElementType::ARRAY},
	{ "float1", BufferElementType::FLOAT_SCALAR},
	{ "float2", BufferElementType::FLOAT_VEC2 },
	{ "float3", BufferElementType::FLOAT_VEC3 },
	{ "float4", BufferElementType::FLOAT_VEC4 },
	{ "int1", BufferElementType::INT_SCALAR},
	{ "uint1", BufferElementType::UINT_SCALAR },
	{ "mat4", BufferElementType::MAT4 },
};

glm::vec3 objectToVec3(rapidjson::Value& json) { return glm::vec3(json["x"].GetFloat(), json["y"].GetFloat(), json["z"].GetFloat()); }
glm::vec4 objectToVec4(rapidjson::Value& json) { return glm::vec4(json["x"].GetFloat(), json["y"].GetFloat(), json["z"].GetFloat(), json["w"].GetFloat()); }
glm::vec4 objectToColour(rapidjson::Value& json) { return glm::vec4(json["r"].GetFloat(), json["g"].GetFloat(), json["b"].GetFloat(), json["a"].GetFloat()); }

int getIntFromPath(rapidjson::Value* json, std::string path)
{
    rapidjson::Value* v = json;
    for (std::string next : Util::String::Split(path, '.')) {
        if (v->IsObject()) {
            v = &((*v)[next.c_str()]);
        }
    }

    return v->GetInt();
}

std::shared_ptr<const BufferFormat> readFormat(rapidjson::Value& json, BufferPackingType packingType = BufferPackingType::PACKED) {
	std::vector<std::pair<std::string, BufferElementType>> format; 
	std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> nestedBufferFormats;

	for (auto& member : json.GetObject()) {
		std::string name(member.name.GetString());

		if (member.value.IsObject()) {
			BufferElementType type = elementTypes[member.value["type"].GetString()];
			BufferPackingType nestedPackingType = packingType;
			if (member.value.HasMember("packing")) {
				std::string packingTypeString(member.value["packing"].GetString());
				if (packingTypeString == "std140") {
					nestedPackingType = BufferPackingType::OPENGL_STD140;
				}
				else if (packingTypeString == "packed") {
					nestedPackingType = BufferPackingType::PACKED;
				}
				else {
					assert(false); //unknown packing type
				}
			}

			if (type == BufferElementType::BUFFER) {
				nestedBufferFormats.emplace(name, readFormat(member.value["format"], nestedPackingType));
			}
			else if (type == BufferElementType::ARRAY) {
				auto& arrayType = member.value["arrayType"];
				if (arrayType.IsObject()) {
					nestedBufferFormats.emplace(name, std::make_shared<BufferFormat>(member.value["arrayCount"].GetInt64(), readFormat(member.value["arrayType"]["format"])));
				}
				else {
					nestedBufferFormats.emplace(name, std::make_shared<BufferFormat>(member.value["arrayCount"].GetInt64(), elementTypes[member.value["arrayType"].GetString()], nestedPackingType));
				}
			}
			format.push_back(std::make_pair(name, type));
		}
		else {
			BufferElementType type = elementTypes[member.value.GetString()];
			assert(type != BufferElementType::BUFFER && type != BufferElementType::ARRAY);
			format.push_back(std::make_pair(name, type));
		}
	}

	return std::make_shared<BufferFormat>(format, nestedBufferFormats);
}

std::unordered_map<std::string, std::shared_ptr<Mesh>> loadMeshes(rapidjson::Document& json)
{
    std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;
    for (auto& mesh : json["meshes"].GetObject()) {
        std::string filename(mesh.value["filename"].GetString());
        std::string extension = filename.substr(filename.find_last_of(".") + 1);
        std::string name(mesh.name.GetString());

        if (extension == "obj") {
            VertexFormat format = VertexFormats::Unknown;
            if (mesh.value.HasMember("format")) {
                format = vertexFormats[mesh.value["format"].GetString()];
            }
            meshes[name] = ModelLoader::loadOBJFile(filename, format);
        } else if (extension == "mbin") {
            meshes[name] = ModelLoader::loadBinFile(filename);
        } else if (extension == "fbx") {
            meshes[name] = ModelLoader::loadFBXFile(filename);
        }
    }
    return meshes;
}

std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> loadTextures(rapidjson::Document& json)
{
    std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures;
	if (json.HasMember("textures")) {
		for (auto& texture : json["textures"].GetObject()) {
			int width, height;

			if (texture.value["width"].IsInt()) {
				width = texture.value["width"].GetInt();
			}
			else {
				std::string widthString(texture.value["width"].GetString());
				width = getIntFromPath(&json, widthString.substr(1, widthString.size() - 2));
			}
			if (texture.value["height"].IsInt()) {
				height = texture.value["height"].GetInt();
			}
			else {
				std::string heightString(texture.value["height"].GetString());
				height = getIntFromPath(&json, heightString.substr(1, heightString.size() - 2));
			}

			std::string name(texture.name.GetString());
			std::string format(texture.value["format"].GetString());
			if (texture.value.HasMember("filename")) {
			}
			else {
				textures[name] = std::make_shared<TextureBuffer>(glm::vec2(width, height), textureFormats[format]);
			}
		}
	}
    return textures;
}

std::unordered_map<std::string, std::shared_ptr<Scene::Cameras::Camera>> loadCameras(rapidjson::Value& json)
{
    std::unordered_map<std::string, std::shared_ptr<Scene::Cameras::Camera>> cameras;
    for (auto& camera : json["cameras"].GetObject()) {
        if (camera.value.HasMember("type") && std::string(camera.value["type"].GetString()) == "quaternion") {
            cameras[camera.name.GetString()] = std::make_shared<Scene::Cameras::QuaternionCamera>(objectToVec3(camera.value["position"]),
                objectToVec3(camera.value["target"]),
                objectToVec3(camera.value["up"]),
                camera.value["fov"].GetFloat(),
                camera.value["aspectRatio"].GetFloat());
        } else {
            cameras[camera.name.GetString()] = std::make_shared<Scene::Cameras::Camera>(objectToVec3(camera.value["position"]),
                objectToVec3(camera.value["target"]),
                objectToVec3(camera.value["up"]),
                camera.value["fov"].GetFloat(),
                camera.value["aspectRatio"].GetFloat());
        }
    }
    return cameras;
}

std::unordered_map<std::string, std::vector<std::shared_ptr<ModelInstance>>> loadModelGroups(
    rapidjson::Value& json, std::unordered_map<std::string, std::shared_ptr<Model>> models)
{
    std::unordered_map<std::string, std::vector<std::shared_ptr<ModelInstance>>> modelGroups;
    for (auto& modelGroup : json["models"].GetObject()) {
        std::string name = modelGroup.name.GetString();
        for (auto& instance : modelGroup.value.GetArray()) {
            std::shared_ptr<ModelInstance> i(std::make_shared<ModelInstance>(models[instance["model"].GetString()]));
            if (instance.HasMember("scale")) {
                i->scale(objectToVec3(instance["scale"]));
            }
            if (instance.HasMember("rotation")) {
                i->rotate(objectToVec3(instance["rotation"]["axis"]), instance["rotation"]["degrees"].GetFloat());
            }
            if (instance.HasMember("position")) {
                i->translate(objectToVec3(instance["position"]));
            }
            modelGroups[name].push_back(i);
        }
    }
    return modelGroups;
}

std::unordered_map<std::string, std::vector<PointLight>> loadLightGroups(rapidjson::Value& json, std::unordered_map<std::string, PointLight> lights)
{
    std::unordered_map<std::string, std::vector<PointLight>> lightGroups;
    for (auto& lightGroup : json["lights"].GetObject()) {
        std::string name = lightGroup.name.GetString();
        for (auto& instance : lightGroup.value.GetArray()) {
            PointLight& baseLight = lights[instance["light"].GetString()];
            lightGroups[name].push_back(PointLight(objectToVec3(instance["position"]), baseLight.intensity(), baseLight.power()));
        }
    }
    return lightGroups;
}

std::unordered_map<std::string, std::shared_ptr<Scene::RenderPass>> loadPasses(rapidjson::Value& json,
    std::unordered_map<std::string, std::vector<std::shared_ptr<ModelInstance>>>          modelGroups,
    std::unordered_map<std::string, std::vector<PointLight>>                              lightGroups,
    std::unordered_map<std::string, std::shared_ptr<Scene::Cameras::Camera>>                              cameras,
    std::unordered_map<std::string, std::shared_ptr<TextureBuffer>>                       textures)
{
    std::unordered_map<std::string, std::shared_ptr<Scene::RenderPass>> passes;
    for (auto& pass : json["passes"].GetObject()) {
        std::string                 name = pass.name.GetString();
        std::shared_ptr<Scene::RenderPass> p    = std::make_shared<Scene::RenderPass>();

        for (auto& modelGroup : pass.value["models"].GetArray()) {
            for (auto& model : modelGroups[modelGroup.GetString()]) {
                p->addModelInstance(model);
            }
        }

        if (pass.value.HasMember("clearColour")) {
            p->clearColour(objectToColour(pass.value["clearColour"]));
        }

		if (pass.value.HasMember("clearBuffers")) {
			p->clearBuffers(pass.value["clearBuffers"].GetBool());
		}

        if (pass.value.HasMember("cull")) {
            p->cull(pass.value["cull"].GetBool());
        }

        if (pass.value.HasMember("wireframe")) {
            p->wireframe(pass.value["wireframe"].GetBool());
        }

        if (pass.value.HasMember("camera")) {
            p->camera(cameras[pass.value["camera"].GetString()]);
        }

        if (pass.value.HasMember("lights")) {
            for (auto& lightGroup : pass.value["lights"].GetArray()) {
                for (auto& light : lightGroups[lightGroup.GetString()]) {
                    p->addPointLight(light);
                }
            }
        }

        if (pass.value.HasMember("bufferOverrides")) {
            std::unordered_map<FrameBufferTarget, std::shared_ptr<TextureBuffer>> targets;
            auto& overrides = pass.value["bufferOverrides"];
            if (overrides.HasMember("colour0")) {
                targets[FrameBufferTarget::COLOUR0] = textures[overrides["colour0"].GetString()];
            }
			if (overrides.HasMember("colour1")) {
				targets[FrameBufferTarget::COLOUR1] = textures[overrides["colour1"].GetString()];
			}
			if (overrides.HasMember("colour2")) {
				targets[FrameBufferTarget::COLOUR2] = textures[overrides["colour2"].GetString()];
			}
			if (overrides.HasMember("colour3")) {
				targets[FrameBufferTarget::COLOUR3] = textures[overrides["colour3"].GetString()];
			}
			if (overrides.HasMember("colour4")) {
				targets[FrameBufferTarget::COLOUR4] = textures[overrides["colour4"].GetString()];
			}
			if (overrides.HasMember("colour5")) {
				targets[FrameBufferTarget::COLOUR5] = textures[overrides["colour5"].GetString()];
			}
            if (overrides.HasMember("depth")) {
                targets[FrameBufferTarget::DEPTH] = textures[overrides["depth"].GetString()];
            }
            p->frameBuffer(std::make_shared<FrameBuffer>(targets));
        }

        if (pass.value.HasMember("viewport")) {
            auto& viewport = pass.value["viewport"];
            p->viewport(glm::vec2(viewport["x"].GetFloat(), viewport["y"].GetFloat()), glm::vec2(viewport["width"].GetFloat(), viewport["height"].GetFloat()));
        }

        passes[name] = p;
    }
    return passes;
}

std::unordered_map<std::string, std::vector<std::string>> loadPassDepencencies(rapidjson::Value& json)
{
    std::unordered_map<std::string, std::vector<std::string>> passDependencies;
    for (auto& pass : json["passes"].GetObject()) {
        std::string name = pass.name.GetString();
        for (auto& dependency : pass.value["dependencies"].GetArray()) {
            passDependencies[name].push_back(dependency.GetString());
        }
    }
    return passDependencies;
}

std::unordered_map<std::string, std::shared_ptr<Shader>> loadShaders(
    rapidjson::Document& json, std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures)
{
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
    for (auto& shader : json["shaders"].GetObject()) {
        std::vector<std::string> vertexFiles;
        for (auto& filename : shader.value["vertexFiles"].GetArray()) {
            vertexFiles.push_back(filename.GetString());
        }

        std::vector<std::string> fragmentFiles;
        for (auto& filename : shader.value["fragmentFiles"].GetArray()) {
            fragmentFiles.push_back(filename.GetString());
        }

		std::shared_ptr<const BufferFormat> instanceDataFormat = std::make_shared<BufferFormat>();
		if (shader.value.HasMember("instanceDataFormat")) {
			instanceDataFormat = readFormat(shader.value["instanceDataFormat"]);
		}

		std::unordered_map<std::string, std::shared_ptr<const BufferFormat>> materialConstantBufferFormats;
		if (shader.value.HasMember("materialConstantBufferFormats")) {
			materialConstantBufferFormats = readFormat(shader.value["materialConstantBufferFormats"])->nestedFormats();
		}

		std::vector<std::string> systemConstantBufferNames;
		if (shader.value.HasMember("systemConstantBuffers")) {
			for (auto& name : shader.value["systemConstantBuffers"].GetArray()) {
				systemConstantBufferNames.push_back(name.GetString());
			}
		}

		shaders[shader.name.GetString()] = std::make_shared<Shader>(vertexFiles, fragmentFiles, instanceDataFormat, materialConstantBufferFormats, systemConstantBufferNames);
    }
    return shaders;
}

std::unordered_map<std::string, PointLight> loadLights(rapidjson::Document& json)
{
    std::unordered_map<std::string, PointLight> lights;
	if (json.HasMember("lights")) {
		for (auto& light : json["lights"].GetObject()) {
			glm::vec3 colour(light.value["colour"]["r"].GetFloat(), light.value["colour"]["g"].GetFloat(), light.value["colour"]["b"].GetFloat());
			float     power = light.value["power"].GetFloat();

			lights[light.name.GetString()] = PointLight(glm::vec3(), colour, power);
		}
	}
    return lights;
}
}

std::unordered_map<std::string, std::shared_ptr<Material>> loadMaterials(rapidjson::Document& json, std::unordered_map<std::string, std::shared_ptr<Shader>>& shaders) {
	std::unordered_map<std::string, std::shared_ptr<Material>> materials;
	for (auto& material : json["materials"].GetObject()) {
		materials[material.name.GetString()] = std::make_shared<Material>(shaders[material.value["shader"].GetString()]);
	}

	return materials;
}

namespace Scene {
	World SceneLoader::loadWorld(std::string filename)
	{
		std::string             file = Util::File::ReadWholeFile(filename);
		rapidjson::StringStream s(file.c_str());
		rapidjson::Document     json;
		json.Parse(file.c_str());

		if (json.HasParseError()) {
			rapidjson::ParseErrorCode errorCode = json.GetParseError();
			size_t                    errorOffset = json.GetErrorOffset();
			std::cout << "ERROR: " << errorCode << " , " << errorOffset << std::endl;
		}

		std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes(loadMeshes(json));
		std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> textures(loadTextures(json));
		std::unordered_map<std::string, std::shared_ptr<Shader>> shaders(loadShaders(json, textures));
		std::unordered_map<std::string, PointLight> lights(loadLights(json));
		std::unordered_map<std::string, std::shared_ptr<Material>> materials(loadMaterials(json, shaders));

		std::unordered_map<std::string, std::shared_ptr<Model>> models;

		for (auto& model : json["models"].GetObject()) {
			std::string name(model.name.GetString());
			std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> modelTextures;
			if (model.value.HasMember("textures")) {
				for (auto& texture : model.value["textures"].GetObject()) {
					modelTextures.emplace(texture.name.GetString(), textures[texture.value.GetString()]);
				}
			}
			models[name] = std::make_shared<Model>(meshes[model.value["mesh"].GetString()], materials[model.value["material"].GetString()], modelTextures);
		}

		rapidjson::Value scene = json["scene"].GetObject();

		std::unordered_map<std::string, std::shared_ptr<Scene::Cameras::Camera>> cameras(loadCameras(scene));
		std::unordered_map<std::string, std::vector<PointLight>> lightInstances(loadLightGroups(scene, lights));
		std::unordered_map<std::string, std::vector<std::shared_ptr<ModelInstance>>> modelInstances(loadModelGroups(scene, models));
		std::unordered_map<std::string, std::shared_ptr<RenderPass>> passes(loadPasses(scene, modelInstances, lightInstances, cameras, textures));
		std::unordered_map<std::string, std::vector<std::string>> passDependencies(loadPassDepencencies(scene));

		return World(passes, passDependencies, cameras, modelInstances);
	}
}
