#include "Util/StringUtils.h"

#include <sstream>

namespace Util::String
{
	std::vector<std::string> Split(std::string& string, char delimiter)
	{
		std::vector<std::string> elements;
		std::stringstream stream;
		stream.str(string);
		std::string item;
		while (std::getline(stream, item, delimiter)) {
			elements.push_back(item);
		}
		return elements;
	}
}