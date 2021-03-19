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

#pragma warning(push)
#pragma warning(disable: 26812)

struct SquareData
{
	std::list<EntityID> m_EntitiesInSquare;
	unsigned int m_RegionID;

	void AddUnique(EntityID entityToAdd)
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

	SquareData(unsigned int regionID)
		: m_RegionID(regionID)
	{}

	SquareData()
	{
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

	static std::map<Vector2DInt, SquareData, Vector2DInt::VectorCompare> m_MapUnitData;

	Map()
	{}

	~Map()
	{}

	static void Init();
	static void SetLandTexture(sf::Texture tex);

#pragma region Hot Reloading
	static void SetupHotReloading();
	static void RegionsChanged(std::string path, FileStatus fileStatus);
	static void MapChanged(std::string path, FileStatus fileStatus);
	static void ShadersChanged(std::string path, FileStatus fileStatus);
#pragma endregion Hot Reloading

#pragma region Map & Region Loading
	static std::ifstream LoadFile(const char* path);
	static void LoadAllRegions();
	static void LoadMap();
	static void UpdateRegions();
#pragma endregion

#pragma region Rendering
	static void Render();
#pragma endregion

	static void SetRegionColor(int regionId, sf::Color color);

	static void ClearRegions();

	void ColorRegionAfterOwner()
	{
	}

	static SquareData& GetMapSquareData(const Vector2DInt key);
	static bool MapSquareDataContainsKey(const Vector2DInt& key);

	static void ClearRegionMapTiles();

#pragma region Render Data Creation
	static void CreateVertexArrays();

	static void LoadShadersAndCreateRenderStates();

#pragma endregion

	static int GetRegionPositionFromMapCharacter(const char& c);
	static int GetRegionPositionFromRegionId(const unsigned int id);

	// From mapinfo
	static MapRegion& GetRegionById(unsigned int regionId);
	static Vector2DInt ConvertToMap(Vector2D position);
	static Vector2D ConvertToScreen(Vector2DInt position);
	static Vector2DInt GetRegionCapitalLocation(unsigned int regionId);
	static std::vector<Vector2DInt> GetRegionCapitals();
	static std::vector<int> GetRegionIDs();
};
#pragma warning(pop)