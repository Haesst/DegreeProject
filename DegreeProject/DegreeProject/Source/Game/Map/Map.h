#pragma once

#include "ECS/Types.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

#include "Engine/Vector2D.h"
#include "Engine/Log.h"
#include "Engine/Window.h"
#include "Engine/AssetHandler.h"

#include "MapRegion.h"
#include "MapData.h"
#include <map>

#include <list>

enum class FileStatus;
struct Building;

#pragma warning(push)
#pragma warning(disable: 26812)

struct SquareData
{
	std::list<EntityID> m_EntitiesInSquare = std::list<EntityID>();
	unsigned int m_RegionID;
	Vector2DInt m_Position;

	void addUnique(EntityID entityToAdd)
	{
		for (auto& entity : m_EntitiesInSquare)
		{
			if (entity == entityToAdd)
			{
				return;
			}
		}

		m_EntitiesInSquare.push_back(entityToAdd);
	}

	void remove(EntityID entityToRemove)
	{
		m_EntitiesInSquare.remove(entityToRemove);
	}

	SquareData(unsigned int regionID)
		: m_RegionID(regionID)
	{
		m_EntitiesInSquare.clear();
	}

	SquareData()
	{
		m_EntitiesInSquare.clear();
		m_RegionID = 0; // apparantly need a default constructor, need to see where
	}
};

struct Map
{
	static const char* m_RegionPath;
	static const char* m_MapPath;

	static const char* m_FragmentShaderPath;
	static const char* m_VertexShaderPath;
	static MapData m_Data;

	static std::mutex m_RegionMutex;
	static std::mutex m_ShaderMutex;

	static std::vector<SquareData> m_MapSquareData;

	Map()
	{}

	~Map()
	{}

	static void init();
	static void setLandTexture(sf::Texture tex);

#pragma region Hot Reloading
	static void setupHotReloading();
	static void regionsChanged(std::string path, FileStatus fileStatus);
	static void mapChanged(std::string path, FileStatus fileStatus);
	static void shadersChanged(std::string path, FileStatus fileStatus);
#pragma endregion Hot Reloading

#pragma region Map & Region Loading
	static std::ifstream loadFile(const char* path);
	static void loadAllRegions();
	static void loadMap();
	static void updateRegions();
#pragma endregion

#pragma region Rendering
	static void render();
#pragma endregion

	static void setRegionColor(int regionId, sf::Color color);

	static void clearRegions();

	static bool mapSquareDataContainsKey(const Vector2DInt& key);

	static void clearRegionMapTiles();

#pragma region Render Data Creation
	static void createVertexArrays();

	static void loadShadersAndCreateRenderStates();

#pragma endregion

#pragma region Buildings
	static void startConstructionOfBuilding(int buildingId, int buildSlot, int regionId);
#pragma endregion

	static int getRegionPositionFromMapCharacter(const char& c);
	static int getRegionPositionFromRegionId(const unsigned int id);

	// From mapinfo
	static MapRegion& getRegionById(unsigned int regionId);
	static Vector2DInt convertToMap(Vector2D position);
	static Vector2D convertToScreen(Vector2DInt position);
	static Vector2DInt getRegionCapitalLocation(unsigned int regionId);
	static std::vector<Vector2DInt> getRegionCapitals();
	static std::vector<int> getRegionIDs();
};
#pragma warning(pop)