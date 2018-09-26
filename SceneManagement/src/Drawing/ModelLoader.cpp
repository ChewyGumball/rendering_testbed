#include "Drawing/ModelLoader.h"

#include <utility>

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/hash.hpp>
#include <iostream>
#include <vector>

//#include <fbxsdk.h>

#include "Util/FileUtils.h"
#include "Util/StringUtils.h"

using namespace Renderer;

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(const Vertex& x) const {
			size_t h = 0;

			std::hash<glm::vec2> v2hasher;
			std::hash<glm::vec3> v3hasher;
			std::hash<glm::vec4> v4hasher;

			h ^= v3hasher(x.position) + 0x9e3779b9 + (h << 6) + (h >> 2);
			h ^= v2hasher(x.textureCoodinate) + 0x9e3779b9 + (h << 6) + (h >> 2);
			h ^= v3hasher(x.normal) + 0x9e3779b9 + (h << 6) + (h >> 2);
			h ^= v4hasher(x.colour) + 0x9e3779b9 + (h << 6) + (h >> 2);

			return h;
		}
	};
}

namespace {
// OBJ Indexes that are negative are offsets from the end of the list
template <typename T> const T& objIndexFind(std::vector<T>& objs, int index)
{
    if (index < 0) {
        return objs[objs.size() + index];
    } else {
		//indicies start at 1
        return objs[index - 1];
    }
}

struct MeshPart {
	std::vector<uint32_t> indicies;
	std::vector<Vertex> vertices;
	VertexFormat format;
};

std::pair<std::vector<Vertex>, std::vector<uint32_t>> dedupeVertices(std::vector<Vertex>& vertexData, std::vector<uint32_t>& indexData) {
	std::unordered_map<Vertex, uint32_t> deduppedVertexIndices;

	std::vector<Vertex> deduppedVertices;
	std::vector<uint32_t> deduppedIndices;

	for (Vertex& v : vertexData) {
		if (deduppedVertexIndices.count(v) == 0) {
			deduppedVertexIndices[v] = deduppedVertices.size();
			deduppedVertices.push_back(v);
		}
	}

	for (uint32_t index : indexData) {
		deduppedIndices.push_back(deduppedVertexIndices[vertexData[index]]);
	}

	return std::make_pair(deduppedVertices, deduppedIndices);
}

/*

FbxManager*    lSdkManager = FbxManager::Create();
FbxIOSettings* ios         = FbxIOSettings::Create(lSdkManager, IOSROOT);

Vertex makeFBXVertex(int polygonIndex, int vertexIndex, FbxVector4* controlPoints, FbxMesh* mesh, glm::mat4& transform)
{
    FbxVector4 vertex = controlPoints[mesh->GetPolygonVertex(polygonIndex, vertexIndex)];
    FbxVector4 normal;
    mesh->GetPolygonVertexNormal(polygonIndex, vertexIndex, normal);

    return Vertex(glm::vec3(transform * glm::vec4(vertex[0], vertex[1], vertex[2], 1.0)), glm::vec3(normal[0], normal[1], normal[2]));
}

glm::mat4 FBXMatrixToGLM(FbxAMatrix& matrix)
{
    glm::dvec4 c0 = glm::make_vec4((double*)matrix.GetRow(0).Buffer());
    glm::dvec4 c1 = glm::make_vec4((double*)matrix.GetRow(1).Buffer());
    glm::dvec4 c2 = glm::make_vec4((double*)matrix.GetRow(2).Buffer());
    glm::dvec4 c3 = glm::make_vec4((double*)matrix.GetRow(3).Buffer());
    return glm::mat4(c0, c1, c2, c3);
}

void loadFBXMeshFromNode(FbxNode* node, std::vector<int>& indices, std::vector<Vertex>& vertices)
{
    FbxNodeAttribute* attribute   = node->GetNodeAttribute();
    int               vertexCount = static_cast<int>(vertices.size());
    if (attribute && attribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
        glm::mat4   transform     = FBXMatrixToGLM(node->EvaluateGlobalTransform(0));
        FbxMesh*    mesh          = node->GetMesh();
        FbxVector4* controlPoints = mesh->GetControlPoints();

        int polycount = mesh->GetPolygonCount();

        for (int i = 0; i < polycount; i++) {
            switch (mesh->GetPolygonSize(i)) {
            case 4:
                indices.push_back(vertexCount++);
                vertices.push_back(makeFBXVertex(i, 0, controlPoints, mesh, transform));
                indices.push_back(vertexCount++);
                vertices.push_back(makeFBXVertex(i, 2, controlPoints, mesh, transform));
                indices.push_back(vertexCount++);
                vertices.push_back(makeFBXVertex(i, 3, controlPoints, mesh, transform));
            case 3:
                indices.push_back(vertexCount++);
                vertices.push_back(makeFBXVertex(i, 0, controlPoints, mesh, transform));
                indices.push_back(vertexCount++);
                vertices.push_back(makeFBXVertex(i, 1, controlPoints, mesh, transform));
                indices.push_back(vertexCount++);
                vertices.push_back(makeFBXVertex(i, 2, controlPoints, mesh, transform));
            }
        }
    }

    int childrenCount = node->GetChildCount();
    for (int i = 0; i < childrenCount; ++i) {
        loadFBXMeshFromNode(node->GetChild(i), indices, vertices);
    }
}
*/
}

namespace ModelLoader {
	std::unordered_map<Renderer::RenderResourceID, std::pair<std::string, std::shared_ptr<Renderer::Mesh>>> loadMultiPartOBJFile(std::string filename)
	{
		MeshPart defaultMeshPart;
		std::unordered_map<std::string, MeshPart> meshParts;

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> textureCoordinates;

		MeshPart* currentMeshPart = &defaultMeshPart;

		std::string materialName = "unknown";

		bool first = true;

		Util::File::ProcessLines(filename, [&](const std::string_view& line) {
			//There may be more than one space between elements, this will break in such a case :(
			std::vector<std::string_view> elements = Util::String::Split(line, ' ');
			if (elements.size() == 0) return;

			if (elements[0] == "v") {
				positions.emplace_back(Util::String::svtof(elements[1]), Util::String::svtof(elements[2]), Util::String::svtof(elements[3]));
			}
			else if (elements[0] == "vt") {
				textureCoordinates.emplace_back(Util::String::svtof(elements[1]), Util::String::svtof(elements[2]));
			}
			else if (elements[0] == "vn") {
				normals.emplace_back(Util::String::svtof(elements[1]), Util::String::svtof(elements[2]), Util::String::svtof(elements[3]));
			}
			else if (elements[0] == "usemtl") {
				materialName = elements[1];
				currentMeshPart = &meshParts[materialName];
			}
			else if (elements[0] == "f") {
				uint32_t vertexCount = currentMeshPart->vertices.size();

				currentMeshPart->indicies.push_back(vertexCount);
				currentMeshPart->indicies.push_back(vertexCount + 1);
				currentMeshPart->indicies.push_back(vertexCount + 2);

				for (int i = 1; i < 4; ++i) {
					std::vector<std::string_view> face = Util::String::Split(elements[i], '/');

					int  vertexIndex = Util::String::svtoi(face[0]);
					bool hasTextureCoordinates = false;
					bool hasNormals = false;
					int  textureIndex = -1;
					int  normalIndex = -1;

					if (face.size() > 1 && face[1] != "") {
						hasTextureCoordinates = true;
						textureIndex = Util::String::svtoi(face[1]);
					}

					if (face.size() > 2 && face[2] != "") {
						hasNormals = true;
						normalIndex = Util::String::svtoi(face[2]);
					}

					if (hasTextureCoordinates) {
						if (hasNormals) {
							currentMeshPart->format = VertexFormats::Position_Normal_Texture;
							currentMeshPart->vertices.emplace_back(
								objIndexFind(positions, vertexIndex), objIndexFind(normals, normalIndex), objIndexFind(textureCoordinates, textureIndex));
						}
						else {
							currentMeshPart->format = VertexFormats::Position_Texture;
							currentMeshPart->vertices.emplace_back(objIndexFind(positions, vertexIndex), objIndexFind(textureCoordinates, textureIndex));
						}
					}
					else if (hasNormals) {
						currentMeshPart->format = VertexFormats::Position_Normal;
						const glm::vec3& p = objIndexFind(positions, vertexIndex);
						const glm::vec3& n = objIndexFind(normals, normalIndex);
						currentMeshPart->vertices.emplace_back(p, n);
					}
					else {
						currentMeshPart->format = VertexFormats::Position;
						currentMeshPart->vertices.emplace_back(objIndexFind(positions, vertexIndex));
					}
				}
			}
		});

		std::unordered_map<Renderer::RenderResourceID, std::pair<std::string, std::shared_ptr<Renderer::Mesh>>> meshes;

		for (auto& meshPart : meshParts) {
			std::pair<std::vector<Vertex>, std::vector<uint32_t>> deduppedData = dedupeVertices(meshPart.second.vertices, meshPart.second.indicies);
			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(meshPart.second.format, Vertex::flatten(meshPart.second.format, deduppedData.first), std::move(deduppedData.second));
			meshes[mesh->id()] = std::make_pair(meshPart.first, mesh);
		}

		if (defaultMeshPart.indicies.size() > 0) {
			std::pair<std::vector<Vertex>, std::vector<uint32_t>> deduppedData = dedupeVertices(defaultMeshPart.vertices, defaultMeshPart.indicies);
			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(defaultMeshPart.format, Vertex::flatten(defaultMeshPart.format, deduppedData.first), std::move(deduppedData.second));
			meshes[mesh->id()] = std::make_pair("DEFAULT", mesh);
		}

		return meshes;
	}
	std::shared_ptr<Renderer::Mesh> loadOBJFile(std::string filename, VertexFormat loadedFormat)
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> textureCoordinates;

		std::vector<uint32_t>    indices;
		std::vector<Vertex> vertices;
		int                 vertexCount = 0;
		VertexFormat        format(0);
		Util::File::ProcessLines(filename, [&](const std::string_view& line) {
			std::vector<std::string_view> elements = Util::String::Split(line, ' ');
			if (elements[0] == "v") {
				positions.emplace_back(Util::String::svtof(elements[1]), Util::String::svtof(elements[2]), Util::String::svtof(elements[3]));
			} else if (elements[0] == "vt") {
				textureCoordinates.emplace_back(Util::String::svtof(elements[1]), Util::String::svtof(elements[2]));
			} else if (elements[0] == "vn") {
				normals.emplace_back(Util::String::svtof(elements[1]), Util::String::svtof(elements[2]), Util::String::svtof(elements[3]));
			} else if (elements[0] == "f") {
				indices.push_back(vertexCount++);
				indices.push_back(vertexCount++);
				indices.push_back(vertexCount++);

				for (int i = 1; i < 4; ++i) {
					std::vector<std::string_view> face = Util::String::Split(elements[i], '/');

					int  vertexIndex = Util::String::svtoi(face[0]);
					bool hasTextureCoordinates = false;
					bool hasNormals = false;
					int  textureIndex = -1;
					int  normalIndex = -1;

					if (face.size() > 1 && face[1] != "") {
						hasTextureCoordinates = true;
						textureIndex = Util::String::svtoi(face[1]);
					}

					if (face.size() > 2 && face[2] != "") {
						hasNormals = true;
						normalIndex = Util::String::svtoi(face[2]);
					}

					if (hasTextureCoordinates) {
						if (hasNormals) {
							format = VertexFormats::Position_Normal_Texture;
							vertices.emplace_back(
								objIndexFind(positions, vertexIndex), objIndexFind(normals, normalIndex), objIndexFind(textureCoordinates, textureIndex));
						} else {
							format = VertexFormats::Position_Texture;
							vertices.emplace_back(objIndexFind(positions, vertexIndex), objIndexFind(textureCoordinates, textureIndex));
						}
					} else if (hasNormals) {
						format             = VertexFormats::Position_Normal;
						const glm::vec3& p = objIndexFind(positions, vertexIndex);
						const glm::vec3& n = objIndexFind(normals, normalIndex);
						vertices.emplace_back(p, n);
					} else {
						format = VertexFormats::Position;
						vertices.emplace_back(objIndexFind(positions, vertexIndex));
					}
				}
			}
		});

		if (loadedFormat != VertexFormats::Unknown) {
			format = loadedFormat;
		}

		std::pair<std::vector<Vertex>, std::vector<uint32_t>> deduppedData = dedupeVertices(vertices, indices);

		return std::make_shared<Mesh>(format, Vertex::flatten(format, deduppedData.first), std::move(deduppedData.second));
	}
std::shared_ptr<Renderer::Mesh> loadBinFile(std::string filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);

	VertexAttribute format;
    file.read(&format, sizeof(VertexAttribute));

    int64_t vertexCount;
    file.read(reinterpret_cast<char*>(&vertexCount), sizeof(int64_t));
    std::vector<float> vertices(vertexCount);

    file.read(reinterpret_cast<char*>(vertices.data()), vertexCount * sizeof(float));

    int64_t indexCount;
    file.read(reinterpret_cast<char*>(&indexCount), sizeof(int64_t));
    std::vector<uint32_t> indices(indexCount);

    file.read(reinterpret_cast<char*>(indices.data()), indexCount * sizeof(uint32_t));
    file.close();
    return std::make_shared<Mesh>(format, std::move(vertices), std::move(indices));
}

/*
std::shared_ptr<Mesh> loadFBXFile(std::string filename, VertexFormat fileFormat, VertexFormat loadedFormat)
{
    lSdkManager->SetIOSettings(ios);
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
    // Use the first argument as the filename for the importer.
    if (!lImporter->Initialize(filename.c_str(), -1, lSdkManager->GetIOSettings())) {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        return nullptr;
    }

    // Create a new scene so that it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);

    // The file is imported, so get rid of the importer.
    lImporter->Destroy();

    FbxNode* root = lScene->GetRootNode();

    std::vector<int>    indices;
    std::vector<Vertex> vertices;
    VertexFormat        format(VertexFormats::Position_Normal);

    loadFBXMeshFromNode(root, indices, vertices);

    return std::make_shared<Mesh>(format, Vertex::flatten(format, vertices), indices);
}
*/
}
