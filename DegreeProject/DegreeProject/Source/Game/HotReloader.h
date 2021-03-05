#pragma once

#include <vector>
#include <map>
#include <string>
#include <functional>

class FileWatcher;
enum class FileStatus;

class HotReloader
{
private:
	FileWatcher* m_FileWatcher;
	std::map<std::string, const std::function<void(std::string, FileStatus)>> m_Actions;
	static HotReloader* m_Instance;
	static const char* ASSET_DIRECTORY;

public:
	HotReloader(const char* path);
	~HotReloader();

	static HotReloader* Get();
	static void Close();

	void OnFileChange(std::string path, FileStatus fileStatus);

	void SubscribeToFileChange(std::string path, const std::function<void(std::string, FileStatus)>& action);
};