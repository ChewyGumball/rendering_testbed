#include "Util/FileUtils.h"
#include "Util/StringUtils.h"

#include <fstream>
#include <sstream>

namespace Util::File
{
	std::string ReadWholeFile(std::string filename)
	{
		std::ifstream file(filename, std::ios::in | std::ios::binary);
		if (file)
		{
			std::ostringstream fileContents;
			fileContents << file.rdbuf();
			file.close();
			return fileContents.str();
		}

		return std::string();
	}

	std::vector<std::string> ReadLines(std::string filename)
	{
		return Util::String::Split(ReadWholeFile(filename), '\n');
	}
}
