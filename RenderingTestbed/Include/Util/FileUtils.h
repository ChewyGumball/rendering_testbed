#pragma once
#include <string>
#include <vector>

namespace Util::File
{
	std::string ReadWholeFile(std::string filename);

	std::vector<std::string> ReadLines(std::string filename);
}
