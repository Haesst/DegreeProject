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
	bool m_Finished = false;
};

struct GameData
{
	static char* m_BuildingPath;
	static std::map<int, Building> m_Buildings;

	static void initialize();
	static std::ifstream loadFile(char* path);
	static json loadJsonObject(char* path);

	static void loadBuildings();
	static void clearBuildings();
	static void buildingsChanged(std::string path, FileStatus fileStatus);
};