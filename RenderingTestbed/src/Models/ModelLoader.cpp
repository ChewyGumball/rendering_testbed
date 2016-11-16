#include "Models/ModelLoader.h"

#include <vector>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>

#include "Util/FileUtils.h"
#include "Util/StringUtils.h"

namespace {
	//OBJ Indexes that are negative are offsets from the end of the list
	template<typename T>
	const T& objIndexFind(std::vector<T>& objs, int index)
	{
		if (index < 0)
		{
			return objs[objs.size() + index];
		}
		else
		{
			return objs[index];
		}
	}
}


namespace ModelLoader
{
	std::shared_ptr<Mesh> loadOBJFile(std::string filename)
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> textureCoordinates;


		std::vector<int> indices;
		std::vector<Vertex> vertices;
		VertexFormat format(0);
		Util::File::ProcessLines(filename, [&](const std::string& line)
		{
			std::vector<std::string> elements = Util::String::Split(line, ' ');
			if (elements[0] == "v")
			{
				positions.emplace_back(std::stof(elements[1]), std::stof(elements[2]), std::stof(elements[3]));
			}
			else if (elements[0] == "vt")
			{
				textureCoordinates.emplace_back(std::stof(elements[1]), std::stof(elements[2]));
			}
			else if (elements[0] == "vn")
			{
				normals.emplace_back(std::stof(elements[1]), std::stof(elements[2]), std::stof(elements[3]));
			}
			else if (elements[0] == "f")
			{
				indices.push_back(vertices.size());
				indices.push_back(vertices.size() + 1);
				indices.push_back(vertices.size() + 2);

				for (int i = 1; i < 4; ++i)
				{
					std::vector<std::string> face = Util::String::Split(elements[i], '/');

					int vertexIndex = std::stoi(face[0]);
					bool hasTextureCoordinates = false;
					bool hasNormals = false;
					int textureIndex = -1;
					int normalIndex = -1;


					if (face.size() > 1 && face[1] != "")
					{
						hasTextureCoordinates = true;
						textureIndex = std::stoi(face[1]);
					}

					if (face.size() > 2 && face[2] != "")
					{
						hasNormals = true;
						normalIndex = std::stoi(face[2]);
					}

					if (hasTextureCoordinates)
					{
						if (hasNormals)
						{
							format = VertexFormats::Position_Normal_Texture;
							vertices.emplace_back(objIndexFind(positions, vertexIndex), objIndexFind(normals, normalIndex), objIndexFind(textureCoordinates, textureIndex));
						}
						else
						{
							format = VertexFormats::Position_Texture;
							vertices.emplace_back(objIndexFind(positions, vertexIndex), objIndexFind(textureCoordinates, textureIndex));
						}
					}
					else if (hasNormals)
					{
						format = VertexFormats::Position_Normal;
						const glm::vec3& p = objIndexFind(positions, vertexIndex);
						const glm::vec3& n = objIndexFind(normals, normalIndex);
						vertices.emplace_back(p, n);
					}
					else
					{
						format = VertexFormats::Position;
						vertices.emplace_back(objIndexFind(positions, vertexIndex));
					}
				}
			}
		});

		return std::make_shared<Mesh>(format, Vertex::flatten(format,vertices), indices);
	}
	std::shared_ptr<Mesh> loadBinFile(std::string filename)
	{
		std::ifstream file(filename, std::ios::in | std::ios::binary);

		char format;
		file.read(&format, sizeof(char));

		int64_t vertexCount;
		file.read(reinterpret_cast<char*>(&vertexCount), sizeof(int64_t));
		std::vector<float> vertices(vertexCount);

		file.read(reinterpret_cast<char*>(vertices.data()), vertexCount * sizeof(float));

		int64_t indexCount;
		file.read(reinterpret_cast<char*>(&indexCount), sizeof(int64_t));
		std::vector<int32_t> indices(indexCount);


		file.read(reinterpret_cast<char*>(indices.data()), indexCount * sizeof(int32_t));
		file.close();
		return std::make_shared<Mesh>(VertexFormat(format),vertices, indices);
	}
}
