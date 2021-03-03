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
	const char* m_Path;
	std::chrono::duration<int, std::milli> delay; // Time interval at which we check the base folder for changes

	// Keep a record of files from the base directory and their last modification time
	FileWatcher(const char* path, std::chrono::duration<int, std::milli> delay)
		: m_Path(path), delay{ delay } {
		for (auto& file : std::filesystem::recursive_directory_iterator(path)) {
			m_Paths[file.path().string()] = std::filesystem::last_write_time(file);
		}
	};

	void start(const std::function<void(std::string, FileStatus)>& action)
	{
		m_RunThread = std::thread(&FileWatcher::RunThread, this, action);
	}

	~FileWatcher()
	{
		m_Running = false;

		if (m_RunThread.joinable())
		{
			m_RunThread.join();
		}
	}

private:
	std::unordered_map<std::string, std::filesystem::file_time_type> m_Paths;
	bool m_Running = true;
	std::thread m_RunThread;

	// Check if "paths_" contains a given key
	bool contains(const std::string& key)
	{
		auto el = m_Paths.find(key);
		return el != m_Paths.end();
	}

	void RunThread(const std::function<void(std::string, FileStatus)>& action)
	{
		while (m_Running)
		{
			std::this_thread::sleep_for(delay);

			auto it = m_Paths.begin();
			while (it != m_Paths.end())
			{
				if (!std::filesystem::exists(it->first))
				{
					action(it->first, FileStatus::Erased);

					it = m_Paths.erase(it);
				}
				else
				{
					it++;
				}
			}

			// Check if a file was created or modified
			for (auto& file : std::filesystem::recursive_directory_iterator(m_Path))
			{
				auto currentFileLastWriteTime = std::filesystem::last_write_time(file);

				// File creation
				if (!contains(file.path().string()))
				{
					m_Paths[file.path().string()] = currentFileLastWriteTime;

					action(file.path().string(), FileStatus::Created);
				}
				else
				{
					if (m_Paths[file.path().string()] != currentFileLastWriteTime)
					{
						m_Paths[file.path().string()] = currentFileLastWriteTime;

						action(file.path().string(), FileStatus::Modified);
					}
				}
			}
		}
	}
};