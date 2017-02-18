#include "Util/FileUtils.h"
#include "Util/StringUtils.h"

//#include <assert.h>

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
	std::string ReadWholeFile(std::string filename)
	{
		std::ifstream file(filename, std::ios::in | std::ios::binary);
		if (!file) {
			//assert(false); // Couldn't read file
		}

		std::ostringstream fileContents;
		fileContents << file.rdbuf();
		file.close();
		return fileContents.str();
	}

	std::vector<std::string> ReadLines(std::string filename)
	{
		return Util::String::Split(ReadWholeFile(filename), '\n');
	}

	std::vector<uint8_t> ReadBinary(std::string filename)
	{
		std::ifstream file(filename, std::ios::in | std::ios::binary);
		if (!file) {
			//assert(false); // Couldn't read file
		}

		file.seekg(0, std::ios::end);
		size_t length = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint8_t> data(length);
		file.read(reinterpret_cast<char*>(data.data()), length);

		return data;
	}

	void ProcessLines(std::string filename, std::function<void(const std::string&)> processor)
	{
		std::string file = ReadWholeFile(filename);
		auto start = file.begin();
		auto end = file.begin();
		auto actualEnd = file.end();

		while (end != actualEnd)
		{
			if (*end == '\n')
			{
				std::string line(start, end);
				processor(line);
				start = end + 1;
			}
			end = end + 1;
		}
	}

	void WatchForChanges(std::string filename, std::function<void()> observer)
	{
		FileUpdateListener.add(filename, observer);
	}
	void MonitorFiles()
	{
		fileWatcher->update();
	}
}
