#pragma once
#include <vector>
#include <string>
#include "Game/Components/Map.h"
#include "Engine/Vector2D.h"

class MapInfo
{
public:
	static void Initialization(unsigned int numberOfIDs);

	static unsigned int GetRegionTax(unsigned int index);
	static std::string GetRegionName(unsigned int index);
	static std::string GetOwnerName(unsigned int index);
	static Vector2DInt GetRegionCapital(unsigned int index);
	static std::vector<int> GetRegionIDs();

	static std::vector<MapRegion> GetMapRegions();
	static std::vector<Vector2DInt> GetRegionPositions(unsigned int index);
	static unsigned int GetRegionIndex(Vector2DInt position);

	static void SetRegionTax(unsigned int regionTax, unsigned int index);
	static void SetRegionName(std::string regionName, unsigned int index);
	static void SetOwnerName(std::string ownerName, unsigned int index);
	static void SetRegionCapital(Vector2DInt regionCapital, unsigned int index);
	static void SetMapRegions(std::vector<MapRegion> regions);

	static Vector2DInt ConvertToMap(Vector2D position);
	static Vector2D ConvertToScreen(Vector2DInt position);
};