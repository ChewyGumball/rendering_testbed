#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace Util::File
{
	std::string ReadWholeFile(std::string filename);

	std::vector<std::string> ReadLines(std::string filename);

	std::vector<uint8_t> ReadBinary(std::string filename);

	void ProcessLines(std::string filename, std::function<void(const std::string&)> processor);

	void WatchForChanges(std::string filename, std::function<void()> observer);
	void MonitorFiles();
}
