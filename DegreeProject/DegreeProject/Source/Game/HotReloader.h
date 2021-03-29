#pragma once

#include <vector>
#include <map>
#include <string>
#include <functional>

class FileWatcher;
enum class FileStatus;

class HotReloader
{
public:
	HotReloader(const char* path);
	~HotReloader();

	static HotReloader* get();
	static void close();

	void onFileChange(std::string path, FileStatus fileStatus);

	void subscribeToFileChange(std::string path, const std::function<void(std::string, FileStatus)> action);

private:
	FileWatcher* m_FileWatcher;
	std::map<std::string, std::vector<std::function<void(std::string, FileStatus)>>> m_Actions;
	static HotReloader* m_Instance;
	static const char* ASSET_DIRECTORY;
};