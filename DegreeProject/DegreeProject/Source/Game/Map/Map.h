#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

#include "Engine/Vector2D.h"
#include "Engine/Log.h"
#include "Engine/Window.h"
#include "Engine/AssetHandler.h"
#include "Game/Systems/CharacterManager.h"

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
	std::list<UnitID> m_EntitiesInSquare = std::list<UnitID>();
	unsigned int m_RegionID;
	Vector2DInt m_Position;

	void addUnique(UnitID entityToAdd)
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

	void remove(UnitID entityToRemove)
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
	static const int m_XOffset;
	static const int m_YOffset;
	static const float m_TileSize;
	static const float m_HalfTileSize;

	int width;
	int height;

	static Map* m_Instance;
	
	MapData m_Data;

	std::mutex m_RegionMutex;
	std::mutex m_ShaderMutex;

	std::vector<SquareData> m_MapSquareData;

	Map()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = this;
		}

		width = 0;
		height = 0;
	}

	~Map()
	{}

	static Map& get();
	void init();
	void setLandTexture(sf::Texture tex);

	void setupHotReloading();
	void regionsChanged(std::string path, FileStatus fileStatus);
	void shadersChanged(std::string path, FileStatus fileStatus);

	std::ifstream loadFile(const char* path);
	void loadAllRegions();
	void loadMap();
	void updateRegions();

	void render();

	void setRegionColor(int regionId, sf::Color color);
	void clearRegions();
	bool mapSquareDataContainsKey(const Vector2DInt& key);
	bool isWater(const Vector2DInt& tile);
	void clearRegionMapTiles();

	void createVertexArrays();
	void loadShadersAndCreateRenderStates();

	void startConstructionOfBuilding(int buildingId, int buildSlot, int regionId);

	int getRegionPositionFromMapCharacter(const char& c);

	// From mapinfo
	MapRegion& getRegionById(unsigned int regionId);
	static Vector2DInt convertToMap(Vector2D position);
	static Vector2D convertToScreen(Vector2DInt position);
	Vector2DInt getRegionCapitalLocation(unsigned int regionId);
	std::vector<Vector2DInt> getRegionCapitals();
	std::vector<int> getRegionIDs();
	Vector2DInt getMapSize();

	bool regionOccupiedByFriendlies(Character& character, int regionID);

private:
	int getRegionPositionFromRegionId(const unsigned int id);
	void renderSquares(const sf::VertexArray& vertexArray, const sf::Color& color, const sf::Color& highlightColor, const sf::Texture& texture, const bool& highlighted);
	void createVertexArray(sf::VertexArray& vertexArray, const std::vector<Vector2DInt>& squares);

	static const float m_AspectRatio;

	// Mountains
	std::vector<Vector2DInt> m_MountainSquares;
	sf::VertexArray m_MountainVertexArray;
	sf::Color m_MountainBaseColor = sf::Color(80, 80, 80, 255);
	sf::Color m_MountainAlternateColor = sf::Color(100, 100, 100, 255);

	//Water
	std::vector<Vector2DInt> m_WaterSquares;
	sf::VertexArray m_WaterVertexArray;
	sf::Color m_WaterBaseColor = sf::Color::Blue;

	// Unreachable land
	std::vector<Vector2DInt> m_UnreachableLandSquares;
	sf::VertexArray m_UnreachableVertexArray;
	sf::Color m_UnreachableLandColor = sf::Color(20, 60, 20, 255);
};
#pragma warning(pop)