#pragma once
#include <vector>
#include <string>

class MapInfo
{
public:
	static void Initialization(unsigned int numberOfIDs);
	static unsigned int GetRegionTax(unsigned int index);
	static void SetRegionTax(unsigned int regionTax, unsigned int index);
	static std::string GetRegionName(unsigned int index);
	static void SetRegionName(std::string regionName, unsigned int index);
private:
	static unsigned int m_NumberOfIDs;
	static std::vector<unsigned int> m_RegionTax;
	static std::vector<std::string> m_RegionNames;
};