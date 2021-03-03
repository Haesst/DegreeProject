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
	std::vector<Vector2DInt> m_MapSquares;
	sf::Color m_HighlightColor;
	char m_MapChar;

	MapRegion() {};
};

struct Map : public Component
{
	std::vector<MapRegion> m_Regions;
	sf::Texture m_LandTexture;
	sf::Sprite m_LandSprite;

	int m_XOffset = -10;
	int m_YOffset = 7;
	float m_MapScale = 0.6f;

	Map(sf::Texture landTexture)
	{
		Init();
		m_LandTexture = landTexture;
		m_LandSprite.setTexture(landTexture);
	}

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

	void LoadAllRegions()
	{
		std::ifstream file("Assets/Data/Regions.json");
		json j;
		file >> j;

		for (auto& element : j)
		{
			MapRegion region;

			region.m_HighlightColor = GetColor(element["color"].get<std::string>());
			std::string mapCharString = element["mapchar"].get<std::string>();

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
						m_Regions[regionPosition].m_MapSquares.push_back({ x,y });
					}

					x++;
				}
				y++;
			}
		}
		inData.close();
	}

	int GetRegionPosition(const char& c)
	{
		for (int i = 0; i < m_Regions.size(); i++)
		{
			if (m_Regions[i].m_MapChar == c)
			{
				return i;
			}
		}

		return -1;
	}
};