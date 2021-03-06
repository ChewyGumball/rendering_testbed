#include "Util/FileUtils.h"
#include "Util/StringUtils.h"

#include <assert.h>

#include <algorithm>
#include <fstream>
#include <sstream>

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <lib/SimpleFileWatcher/FileWatcher/FileWatcher.h>

namespace {
	FW::FileWatcher* fileWatcher = new FW::FileWatcher(); 
	
	class UpdateListener : public FW::FileWatchListener
	{
	private:
		std::unordered_map<std::string, std::vector<std::function<void()>>> watchers;
		std::unordered_set<std::string> watchedDirectories;
	public:
		UpdateListener()
		{}
		void handleFileAction(FW::WatchID watchID, const FW::String& directory, const FW::String& filename, FW::Action action)
		{
			if (action == FW::Action::Modified)
			{
				std::string fullPath = directory + "\\" + filename;
				if (watchers.count(fullPath) > 0)
				{
					for (auto observer : watchers[fullPath])
					{
						observer();
					}
				}
				fullPath = directory + "/" + filename;
				if (watchers.count(fullPath) > 0)
				{
					for (auto observer : watchers[fullPath])
					{
						observer();
					}
				}
			}
		}

		void add(std::string filename, std::function<void()> observer)
		{
			watchers[filename].push_back(observer);
			std::string filepath = filename.substr(0, filename.find_last_of("/\\"));
			if (watchedDirectories.count(filepath) == 0)
			{
				fileWatcher->addWatch(filepath, this);
				watchedDirectories.insert(filepath);
			}
		}
	} FileUpdateListener;
}

namespace Util::File
{
	std::string ReadWholeFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::in | std::ios::binary);
		if (!file) {
			assert(false); // Couldn't read file
		}

		std::ostringstream fileContents;
		fileContents << file.rdbuf();
		file.close();
		return fileContents.str();
	}

	std::vector<std::string> ReadLines(const std::string& filename)
	{
		return Util::String::Split(ReadWholeFile(filename), '\n');
	}

	std::vector<uint8_t> ReadBinary(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::in | std::ios::binary);
		if (!file) {
			assert(false); // Couldn't read file
		}

		file.seekg(0, file.end);
		size_t length = file.tellg();
		file.seekg(0, file.beg);

		std::vector<uint8_t> data(length);
		file.read(reinterpret_cast<char*>(data.data()), length);

		return data;
	}

	void ProcessLines(const std::string& filename, std::function<void(const std::string&)> processor)
	{
		std::string file = ReadWholeFile(filename);
		auto start = file.begin();
		auto end = file.begin();
		auto actualEnd = file.end();

		while (end != actualEnd)
		{
			if (*end == '\r' && end + 1 != actualEnd && *(end + 1) == '\n')
			{
				std::string line(start, end);
				processor(line);
				start = end + 2;
				end += 1;
			}
			else if (*end == '\n')
			{
				std::string line(start, end);
				processor(line);
				start = end + 1;
			}
			end = end + 1;
		}
	}

	std::vector<uint8_t> ReadPNG(const std::string & filename)
	{
		return std::vector<uint8_t>();
	}

	void WatchForChanges(const std::string& filename, std::function<void()> observer)
	{
		FileUpdateListener.add(filename, observer);
	}
	void MonitorFiles()
	{
		fileWatcher->update();
	}
}
