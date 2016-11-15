#include "Util/StringUtils.h"

#include <sstream>

namespace Util::String
{
	std::vector<std::string> Split(const std::string& string, char delimiter)
	{
		std::vector<std::string> elements;
		auto start = string.begin();
		auto end = string.begin();
		auto actualEnd = string.end();
		long size = string.size();

		while (end != actualEnd)
		{
			if (*end == delimiter)
			{
				elements.emplace_back(start, end);
				start = end + 1;
			}

			end = end + 1;
		}
		elements.emplace_back(start, end);

		return elements;
	}
}