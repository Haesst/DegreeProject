#include "MapInfo.h"
#include "Engine/Window.h"

std::vector<unsigned int> MapInfo::m_RegionTax;
std::vector<std::string> MapInfo::m_RegionNames;
std::vector<std::string> MapInfo::m_OwnerNames;
std::vector<std::vector<Vector2DInt> > MapInfo::m_RegionPositions;
unsigned int MapInfo::m_NumberOfIDs = 0;

void MapInfo::Initialization(unsigned int numberOfIDs)
{
	m_NumberOfIDs = numberOfIDs;
	for (unsigned int index = 0; index < numberOfIDs; index++)
	{
		m_RegionTax.push_back(0);
		m_RegionNames.push_back("");
		m_OwnerNames.push_back("");
		m_RegionPositions.push_back(std::vector<Vector2DInt>());
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

void MapInfo::SetRegionTax(unsigned int tax, unsigned int index)
{
	m_RegionTax[index] = tax;
}

std::string MapInfo::GetRegionName(unsigned int index)
{
	if (index >= m_NumberOfIDs)
	{
		return "";
	}
	return m_RegionNames[index];
}

void MapInfo::SetRegionName(std::string regionName, unsigned int index)
{
	m_RegionNames[index] = regionName;
}

std::string MapInfo::GetOwnerName(unsigned int index)
{
	if (index >= m_NumberOfIDs)
	{
		return "";
	}
	return m_OwnerNames[index];
}

void MapInfo::SetOwnerName(std::string ownerName, unsigned int index)
{
	m_OwnerNames[index] = ownerName;
}

std::vector<Vector2DInt> MapInfo::GetRegionPositions(unsigned int index)
{
	if (index >= m_NumberOfIDs)
	{
		return std::vector<Vector2DInt>();
	}
	return m_RegionPositions[index];
}

std::vector<std::vector<Vector2DInt> >MapInfo::GetRegions()
{
	return m_RegionPositions;
}

void MapInfo::SetRegionPositions(std::vector<Vector2DInt> regionPositions, unsigned int index)
{
	m_RegionPositions[index] = regionPositions;
}

unsigned int MapInfo::GetRegionIndex(Vector2DInt position)
{
	unsigned int index = 0;
	for each (std::vector<Vector2DInt> region in m_RegionPositions)
	{
		for each (Vector2DInt regionPosition in region)
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
