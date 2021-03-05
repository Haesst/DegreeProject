#include "HotReloader.h"

#include "Engine/FileWatcher.h"
#include "Engine/Log.h"

HotReloader* HotReloader::m_Instance = nullptr;
const char* HotReloader::ASSET_DIRECTORY = "Assets";

HotReloader::HotReloader(const char* path)
{
	m_FileWatcher = new FileWatcher(path, std::chrono::milliseconds(1000));
	m_FileWatcher->start(std::bind(&HotReloader::OnFileChange, this, std::placeholders::_1, std::placeholders::_2));
}

HotReloader::~HotReloader()
{
	delete m_FileWatcher;
}

void HotReloader::OnFileChange(std::string path, FileStatus fileStatus)
{
	if (!std::filesystem::is_regular_file(std::filesystem::path(path)) && fileStatus != FileStatus::Erased) {
		return;
	}

	if (m_Actions.find(path) != m_Actions.end())
	{
		m_Actions[path](path, fileStatus);
	}
	else
	{
		LOG_INFO("No subscribers to " + path + " found");
	}
}

void HotReloader::SubscribeToFileChange(std::string path, const std::function<void(std::string, FileStatus)>& action)
{
	m_Actions.insert(std::pair<std::string, const std::function<void(std::string, FileStatus)>>(path, action));
}

HotReloader* HotReloader::Get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new HotReloader(ASSET_DIRECTORY);
	}

	return m_Instance;
}

void HotReloader::Close()
{
	delete m_Instance;
}
