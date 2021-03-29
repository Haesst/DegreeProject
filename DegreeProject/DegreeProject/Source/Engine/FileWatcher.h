#pragma once

#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>

// Define available file changes
enum class FileStatus
{
	Created,
	Modified,
	Erased
};

class FileWatcher
{
public:
	// Keep a record of files from the base directory and their last modification time
	FileWatcher(const char* path, std::chrono::duration<int, std::milli> delay)
		: m_Path(path), delay{ delay } {
		for (auto& file : std::filesystem::recursive_directory_iterator(path)) {
			m_Paths[file.path().string()] = std::filesystem::last_write_time(file);
		}
	};

	~FileWatcher()
	{
		m_Running = false;

		if (m_RunThread.joinable())
		{
			m_RunThread.join();
		}
	}

	void start(const std::function<void(std::string, FileStatus)>& action)
	{
		m_RunThread = std::thread(&FileWatcher::runThread, this, action);
	}

private:
	bool contains(const std::string& key) const
	{
		auto el = m_Paths.find(key);
		return el != m_Paths.end();
	}

	void runThread(const std::function<void(std::string, FileStatus)>& action);

private:
	std::unordered_map<std::string, std::filesystem::file_time_type> m_Paths;
	bool m_Running = true;
	std::thread m_RunThread;
	const char* m_Path;
	std::chrono::duration<int, std::milli> delay;

};