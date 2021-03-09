#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

#include "ECS/Component.h"
#include "Engine/Vector2D.h"
#include "Engine/Log.h"

struct MapRegion
{
public:
	std::vector<Vector2DInt> m_MapSquares = {};
	std::vector<Vector2DInt> m_RegionPositions = {};
	sf::Color m_HighlightColor = sf::Color::White;
	char m_MapChar = '1';
	unsigned int m_RegionId = INT_MAX;
	unsigned int m_RegionTax = 0;
	std::string m_RegionName = "";
	MapRegion() {};
	~MapRegion() { m_MapSquares.clear(); }
};

struct Map : public Component
{
	std::vector<MapRegion> m_Regions;
	sf::Texture m_LandTexture;
	sf::Sprite m_LandSprite;

	int m_XOffset = -10;
	int m_YOffset = 7;
	float m_MapScale = 0.6f;

	bool m_ChangeFlag = false;

	Map()
	{}

	Map(sf::Texture landTexture)
	{
		Init();
		m_LandTexture = landTexture;
		m_LandSprite.setTexture(landTexture);
	}

	~Map()
	{}

	void Init()
	{
		LoadAllRegions();
		LoadMap();
	}

	sf::Color GetColor(std::string color)
	{
		if (color == "Red")
		{
			return sf::Color::Red;
		}
		if (color == "Yellow")
		{
			return sf::Color::Yellow;
		}
		if (color == "Purple")
		{
			return sf::Color::Magenta;
		}
		if (color == "Cyan")
		{
			return sf::Color::Cyan;
		}
		if (color == "Blue")
		{
			return sf::Color::Blue;
		}
		if (color == "Brown")
		{
			return sf::Color::Color(40,26,13);
		}
		
		return sf::Color::White;
	}

	void SetRegionColor(int regionId, sf::Color color)
	{
		m_Regions[regionId].m_HighlightColor = color;
	}

	void LoadAllRegions()
	{
		std::ifstream file("Assets/Data/Regions.json");
		json j;
		file >> j;

		m_Regions.clear();

		for (auto& element : j)
		{
			MapRegion region;

			region.m_HighlightColor = GetColor(element["color"].get<std::string>());
			std::string mapCharString = element["mapchar"].get<std::string>();
			region.m_RegionId = element["id"].get<unsigned int>();
			region.m_RegionTax = element["tax"].get<unsigned int>();
			region.m_RegionName = element["name"].get<std::string>();

			if (mapCharString.size() > 0)
			{
				region.m_MapChar = mapCharString[0];
			}

			m_Regions.push_back(region);
		}
	}

	void LoadMap()
	{
		std::string tempString;
		std::ifstream inData;
		inData.open("Assets/Map/RegionMap.txt");
		
		for (auto region : m_Regions)
		{
			region.m_MapSquares.clear();
			region.m_RegionPositions.clear();
		}

		if (inData.is_open())
		{
			int y = 0;
			while (!inData.eof())
			{
				getline(inData, tempString);
				int x = 0;
				for (char character : tempString)
				{
					int regionPosition = GetRegionPosition(character);

					if (regionPosition >= 0)
					{
						m_Regions[regionPosition].m_MapSquares.push_back({ x, y });
						m_Regions[regionPosition].m_RegionPositions.push_back({ x, y });
					}

					x++;
				}
				y++;
			}
		}
		inData.close();
	}

	size_t GetRegionPosition(const char& c)
	{
		for (size_t i = 0; i < m_Regions.size(); ++i)
		{
			if (m_Regions[i].m_MapChar == c)
			{
				return i;
			}
		}

		return -1;
	}
};