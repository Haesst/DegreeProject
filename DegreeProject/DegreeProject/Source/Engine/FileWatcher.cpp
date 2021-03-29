#include "FileWatcher.h"

void FileWatcher::runThread(const std::function<void(std::string, FileStatus)>& action)
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
