#include "Models/ModelLoader.h"

#include <vector>
#include <glm/glm.hpp>

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
							vertices.emplace_back(objIndexFind(positions, vertexIndex), objIndexFind(normals, normalIndex), objIndexFind(textureCoordinates, textureIndex));
						}
						else
						{
							vertices.emplace_back(objIndexFind(positions, vertexIndex), objIndexFind(textureCoordinates, textureIndex));
						}
					}
					else if (hasNormals)
					{
						const glm::vec3& p = objIndexFind(positions, vertexIndex);
						const glm::vec3& n = objIndexFind(normals, normalIndex);
						vertices.emplace_back(p, n);
					}
					else
					{
						vertices.emplace_back(objIndexFind(positions, vertexIndex));
					}
				}
			}
		});

		return std::make_shared<Mesh>(vertices, indices);
	}
}
