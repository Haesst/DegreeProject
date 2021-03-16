#pragma once
#include <vector>
#include <string>
#include "Engine/Vector2D.h"

struct MapRegion;

class MapInfo
{
public:
	static void Initialization(unsigned int numberOfIDs);

	static unsigned int GetRegionTax(unsigned int index);
	static std::string GetRegionName(unsigned int index);
	static std::string GetOwnerName(unsigned int index);
	static unsigned int GetIndexOfCapital(Vector2DInt regionCapital);
	//static Vector2DInt GetRegionCapital(unsigned int index);
	static std::vector<int> GetRegionIDs();
	static std::vector<unsigned int> GetCharacterIDs();

	static std::vector<MapRegion> GetMapRegions();
	static std::vector<Vector2DInt> GetRegionPositions(unsigned int index);
	static std::vector<Vector2DInt> GetRegionCapitals();
	static unsigned int GetRegionIndex(Vector2DInt position);

	static void SetMapOffset(int mapOffsetX, int mapOffsetY);
	static void SetRegionTax(unsigned int regionTax, unsigned int index);
	static void SetRegionName(std::string regionName, unsigned int index);
	static void SetOwnerName(std::string ownerName, unsigned int index);
	static void SetCharacterID(unsigned int entity, unsigned int characterIndex);
	static void SetRegionCapital(Vector2DInt regionCapital, unsigned int index);
	static void SetMapRegions(std::vector<MapRegion> regions);

	// static Vector2DInt ConvertToMap(Vector2D position);
	// static Vector2D ConvertToScreen(Vector2DInt position);
};