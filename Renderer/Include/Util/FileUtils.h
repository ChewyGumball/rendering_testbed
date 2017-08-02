#pragma once
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace Util::File
{
	std::string ReadWholeFile(const std::string& filename);

	std::vector<std::string_view> ReadLines(const std::string& fileContents);

	std::vector<uint8_t> ReadBinary(const std::string& filename);

	void ProcessLines(const std::string& filename, std::function<void(const std::string_view&)> processor);

	void WatchForChanges(const std::string& filename, std::function<void()> observer);
	void MonitorFiles();
}
