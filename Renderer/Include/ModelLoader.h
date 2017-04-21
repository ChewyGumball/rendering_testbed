#pragma once
#include <memory>
#include <string>

#include "Resources/Mesh.h"

namespace ModelLoader
{
	std::shared_ptr<Mesh> loadOBJFile(std::string filename,VertexFormat loadedFormat = VertexFormats::Unknown);
	std::shared_ptr<Mesh> loadBinFile(std::string filename);
	std::shared_ptr<Mesh> loadFBXFile(std::string filename, VertexFormat fileFormat = VertexFormats::Unknown, VertexFormat loadedFormat = VertexFormats::Unknown);
}
