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
	static void SetRegionTax(unsigned int regionTax, unsigned int index);
	static std::string GetRegionName(unsigned int index);
	static void SetRegionName(std::string regionName, unsigned int index);
	static std::string GetOwnerName(unsigned int index);
	static void SetOwnerName(std::string ownerName, unsigned int index);
	static std::vector<Vector2DInt> GetRegionPositions(unsigned int index);
	static void SetRegionPositions(std::vector<Vector2DInt> regionPositions, unsigned int index);
	static unsigned int GetRegionIndex(Vector2DInt position);
	static std::vector<std::vector<Vector2DInt> > GetRegions();
	static std::vector<MapRegion> GetMapRegions();
	static void SetMapRegions(std::vector<MapRegion> regions);
	static std::vector<int> GetRegionIds();
private:
	static std::vector<MapRegion> m_MapRegions;
	static unsigned int m_NumberOfIDs;
	static std::vector<unsigned int> m_RegionTax;
	static std::vector<std::string> m_RegionNames;
	static std::vector<std::string> m_OwnerNames;
	static std::vector<std::vector<Vector2DInt> > m_RegionPositions;
};