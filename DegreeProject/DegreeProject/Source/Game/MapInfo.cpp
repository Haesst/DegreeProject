#include "MapInfo.h"

std::vector<unsigned int> MapInfo::m_RegionTax;
std::vector<std::string> MapInfo::m_RegionNames;
unsigned int MapInfo::m_NumberOfIDs = 0;

void MapInfo::Initialization(unsigned int numberOfIDs)
{
	m_NumberOfIDs = numberOfIDs;
	for (unsigned int index = 0; index < numberOfIDs; index++)
	{
		m_RegionTax.push_back(0);
		m_RegionNames.push_back("");
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