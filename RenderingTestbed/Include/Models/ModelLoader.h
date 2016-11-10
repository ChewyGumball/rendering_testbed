#pragma once
#include <string>

#include "Models/Mesh.h"

namespace ModelLoader
{
	Mesh loadOBJFile(std::string& filename);
}
