#include "GameData.h"

#include "Engine/FileWatcher.h"
#include "Engine/Log.h"
#include "Game/HotReloader.h"

char* GameData::m_BuildingPath = "Assets\\Data\\Buildings.json";

std::map<int, Building> GameData::m_Buildings;

void GameData::Initialize()
{
	ClearBuildings();
	LoadBuildings();
	HotReloader::Get()->SubscribeToFileChange(m_BuildingPath, std::bind(&GameData::BuildingsChanged, std::placeholders::_1, std::placeholders::_2));
}

std::ifstream GameData::LoadFile(char* path)
{
	std::ifstream file(path);

	return file;
}

json GameData::LoadJsonObject(char* path)
{
	std::ifstream file = LoadFile(path);

	json j;
	file >> j;

	return j;
}

void GameData::BuildingsChanged(std::string path, FileStatus fileStatus)
{
	if (fileStatus != FileStatus::Modified)
	{
		return;
	}

	ClearBuildings();
	LoadBuildings();
}

void GameData::LoadBuildings()
{
	json j = LoadJsonObject(m_BuildingPath);

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
	}

	LOG_INFO("Buildings Loaded.");
}

void GameData::ClearBuildings()
{
	m_Buildings.clear();
}
