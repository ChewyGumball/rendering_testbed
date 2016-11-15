#include "Util/FileUtils.h"
#include "Util/StringUtils.h"

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
			std::string fullPath = directory + "\\" + filename;
			if (action == FW::Action::Modified && watchers.count(fullPath) > 0)
			{
				for (auto observer : watchers[fullPath])
				{
					observer();
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
	void ProcessLines(std::string filename, std::function<void(const std::string&)> processor)
	{
		std::string file = ReadWholeFile(filename);
		auto start = file.begin();
		auto end = file.begin();
		auto actualEnd = file.end();
		long size = file.size();

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
