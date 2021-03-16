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

#include "Game/MapInfo.h"

#include "MapRegion.h"

#pragma warning(push)
#pragma warning(disable: 26812)

struct MapData
{
	std::vector<MapRegion> m_Regions;

	sf::Texture m_LandTexture;
	sf::RenderStates m_RenderStates;
	sf::Shader m_Shader;

	int m_XOffset = -300;
	int m_YOffset = -100;
	float m_TileSize = 32;
	float m_HalfTileSize;

	MapData()
	{
		m_HalfTileSize = m_TileSize * 0.5f;
	}
};

struct Map
{
	static const char* m_RegionPath;
	static const char* m_MapPath;

	static const char* m_FragmentShaderPath;
	static const char* m_VertexShaderPath;
	static MapData m_Data;

	Map()
	{}

	~Map()
	{}

	static void Init();
	static void SetLandTexture(sf::Texture tex);
	static void UpdateMapInfo(size_t regionIndex);

#pragma region Map & Region Loading
	static std::ifstream LoadFile(const char* path);
	static void LoadAllRegions();
	static void LoadMap();
#pragma endregion

#pragma region Rendering
	static void Render();
#pragma endregion

	static void SetRegionColor(int regionId, sf::Color color);

	static void ClearRegions();

	void ColorRegionAfterOwner()
	{
	}

	static void ClearRegionMapTiles();

#pragma region Render Data Creation
	static void CreateVertexArrays();

	static void LoadShadersAndCreateRenderStates();

#pragma endregion

	static int GetRegionPositionFromMapCharacter(const char& c);
	static int GetRegionPositionFromRegionId(const unsigned int id);

	// From mapinfo
	static Vector2DInt ConvertToMap(Vector2D position);
	static Vector2D ConvertToScreen(Vector2DInt position);
	static Vector2DInt GetRegionCapitalLocation(unsigned int regionId);
	static std::vector<Vector2DInt> GetRegionCapitals();
	static std::vector<int> GetRegionIDs();
};
#pragma warning(pop)