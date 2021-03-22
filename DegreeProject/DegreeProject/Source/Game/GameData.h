#pragma once
#include <map>
#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

enum class FileStatus;

struct Building
{
	int m_Id = -1;
	std::string m_Name = "";
	int m_Cost = 0;
	int m_HoldingModifier = 0;
	int m_IncomeModifier = 0;
	int m_ArmyModifier = 0;
	int m_DaysToConstruct = 0;
};

struct GameData
{
	static char* m_BuildingPath;
	static std::map<int, Building> m_Buildings;

	static void Initialize();
	static std::ifstream LoadFile(char* path);
	static json LoadJsonObject(char* path);

	static void LoadBuildings();
	static void ClearBuildings();
	static void BuildingsChanged(std::string path, FileStatus fileStatus);
};