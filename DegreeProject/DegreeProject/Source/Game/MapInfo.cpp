#include "MapInfo.h"

std::vector<unsigned int> m_RegionTax;
std::vector<std::string> m_RegionNames;
std::vector<std::string> m_OwnerNames;
std::vector<MapRegion> m_MapRegions;
std::vector<int> m_RegionIDs;
unsigned int m_NumberOfIDs = 0;

void MapInfo::Initialization(unsigned int numberOfIDs)
{
	m_NumberOfIDs = numberOfIDs;
	for (unsigned int index = 0; index < numberOfIDs; index++)
	{
		m_RegionTax.push_back(0);
		m_RegionNames.push_back("");
		m_OwnerNames.push_back("");
		m_RegionIDs.push_back(index);
	}
}

unsigned int MapInfo::GetRegionTax(unsigned int index)
{
	if (index >= m_NumberOfIDs)
	{
		return 0;
	}
	return m_RegionTax[index];
}

std::string MapInfo::GetRegionName(unsigned int index)
{
	if (index >= m_NumberOfIDs)
	{
		return "";
	}
	return m_RegionNames[index];
}

std::string MapInfo::GetOwnerName(unsigned int index)
{
	if (index >= m_NumberOfIDs)
	{
		return "";
	}
	return m_OwnerNames[index];
}

std::vector<int> MapInfo::GetRegionIDs()
{
	return m_RegionIDs;
}

std::vector<MapRegion> MapInfo::GetMapRegions()
{
	return m_MapRegions;
}

std::vector<Vector2DInt> MapInfo::GetRegionPositions(unsigned int index)
{
	if (index >= m_NumberOfIDs)
	{
		return std::vector<Vector2DInt>();
	}
	return m_MapRegions[index].m_MapSquares;
}

unsigned int MapInfo::GetRegionIndex(Vector2DInt position)
{
	unsigned int index = 0;
	for each (MapRegion region in m_MapRegions)
	{
		for each (Vector2DInt regionPosition in region.m_MapSquares)
		{
			if (regionPosition == position)
			{
				return index;
			}
		}
		index++;
	}
	return 0;
}

void MapInfo::SetRegionTax(unsigned int tax, unsigned int index)
{
	if (index < m_NumberOfIDs)
	{
		m_RegionTax[index] = tax;
	}
}

void MapInfo::SetRegionName(std::string regionName, unsigned int index)
{
	if (index < m_NumberOfIDs)
	{
		m_RegionNames[index] = regionName;
	}
}

void MapInfo::SetOwnerName(std::string ownerName, unsigned int index)
{
	if (index < m_NumberOfIDs)
	{
		m_OwnerNames[index] = ownerName;
	}
}

void MapInfo::SetMapRegions(std::vector<MapRegion> regions)
{
	m_MapRegions = regions;
}

Vector2DInt MapInfo::ConvertToMap(Vector2D position)
{
	return Vector2DInt((position.x - 100 + 16) / 32, (position.y - 100 + 16) / 32);
}

Vector2D MapInfo::ConvertToScreen(Vector2DInt position)
{
	return Vector2D((float)position.x * 32 + 100 - 16, (float)position.y * 32 + 100 - 16);
}