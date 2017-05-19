#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace Util::File
{
	std::string ReadWholeFile(const std::string& filename);

	std::vector<std::string> ReadLines(const std::string& filename);

	std::vector<uint8_t> ReadBinary(const std::string& filename);

	void ProcessLines(const std::string& filename, std::function<void(const std::string&)> processor);

	void WatchForChanges(const std::string& filename, std::function<void()> observer);
	void MonitorFiles();
}
