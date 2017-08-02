#include "Util/StringUtils.h"

#include <sstream>
#include <string_view>

namespace {
	std::string badStringViewBuffer;
}

namespace Util::String
{
	std::vector<std::string_view> Split(const std::string& string, char delimiter)
	{
		return Split(std::string_view(string.data(), string.size()), delimiter);
	}

	std::vector<std::string_view> Split(const std::string_view & string, char delimiter)
	{
		std::vector<std::string_view> elements;

		if (string.size() > 0) {
			size_t start = 0;
			size_t end = 0;
			size_t actualEnd = string.size();

			while (end != actualEnd)
			{
				if (string[end] == delimiter)
				{
					elements.emplace_back(&string[start], end - start);
					++end;

					while (end != actualEnd && string[end] == delimiter) {
						elements.emplace_back(&string[start], 0);
						++end;
					}
					start = end;
				}
				else {
					++end;
				}
			}

			elements.emplace_back(&string[start], end - start);
		}

		return elements;
	}

	float svtof(std::string_view string)
	{
		badStringViewBuffer.reserve(string.size());
		badStringViewBuffer.clear();
		for (size_t i = 0; i < string.size(); i++) {
			badStringViewBuffer.push_back(string[i]);
		}

		return std::stof(badStringViewBuffer);
	}

	int svtoi(std::string_view string)
	{
		badStringViewBuffer.reserve(string.size());
		badStringViewBuffer.clear();
		for (size_t i = 0; i < string.size(); i++) {
			badStringViewBuffer.push_back(string[i]);
		}

		return std::stoi(badStringViewBuffer);
	}
}