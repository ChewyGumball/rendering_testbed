#pragma once
#include <memory>
#include <string>

#include "Models/Mesh.h"

namespace ModelLoader
{
	std::shared_ptr<Mesh> loadOBJFile(std::string filename);
	std::shared_ptr<Mesh> loadBinFile(std::string filename);
	std::shared_ptr<Mesh> loadFBXFile(std::string filename);
}
