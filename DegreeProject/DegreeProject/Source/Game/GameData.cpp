#include "GameData.h"

#include "Engine/FileWatcher.h"
#include "Engine/Log.h"
#include "Game/HotReloader.h"

char* GameData::m_BuildingPath = "Assets\\Data\\Buildings.json";

std::map<int, Building> GameData::m_Buildings;

void GameData::initialize()
{
	clearBuildings();
	loadBuildings();
	HotReloader::get()->subscribeToFileChange(m_BuildingPath, std::bind(&GameData::buildingsChanged, std::placeholders::_1, std::placeholders::_2));
}

std::ifstream GameData::loadFile(char* path)
{
	std::ifstream file(path);

	return file;
}

json GameData::loadJsonObject(char* path)
{
	std::ifstream file = loadFile(path);

	json j;
	file >> j;

	return j;
}

void GameData::buildingsChanged(std::string path, FileStatus fileStatus)
{
	if (fileStatus != FileStatus::Modified)
	{
		return;
	}

	clearBuildings();
	loadBuildings();
}

void GameData::loadBuildings()
{
	json j = loadJsonObject(m_BuildingPath);

	ASSERT(m_Buildings.size() <= 0, "Buildings not cleared before loading");

	for (auto element : j)
	{
		Building building;

		building.m_Id = element["id"].get<int>();
		building.m_Name = element["name"].get<std::string>();
		building.m_Cost = element["cost"].get<int>();
		building.m_HoldingModifier = element["regionStrength"].get<int>();
		building.m_IncomeModifier = element["regionIncome"].get<int>();
		building.m_ArmyModifier = element["regionArmy"].get<int>();
		building.m_DaysToConstruct = element["daysToConstruct"].get<int>();

		m_Buildings.insert(std::pair(building.m_Id, building));
	}

	LOG_INFO("{0} Buildings Loaded.", m_Buildings.size());
}

void GameData::clearBuildings()
{
	m_Buildings.clear();
}
