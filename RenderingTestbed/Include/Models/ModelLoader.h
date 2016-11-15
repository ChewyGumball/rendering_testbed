#pragma once
#include <memory>
#include <string>

#include "Models/Mesh.h"

namespace ModelLoader
{
	std::shared_ptr<Mesh> loadOBJFile(std::string filename);
}
