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

		while (end != actualEnd)
		{
			if (*end == delimiter)
			{
				elements.emplace_back(start, end);
				while (end != actualEnd && *end == delimiter) {
					++end;
				}
				start = end;
			}
			else {
				++end;
			}
		}
		elements.emplace_back(start, end);

		return elements;
	}
}