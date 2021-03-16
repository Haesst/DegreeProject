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

#include "MapRegion.h"
#include "MapData.h"

enum class FileStatus;

#pragma warning(push)
#pragma warning(disable: 26812)

struct Map
{
	static const char* m_RegionPath;
	static const char* m_MapPath;

	static const char* m_FragmentShaderPath;
	static const char* m_VertexShaderPath;
	static MapData m_Data;

	static std::mutex m_RegionMutex;

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