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
	sf::Color m_HighlightColor = sf::Color::White;
	char m_MapChar = '1';
	unsigned int m_RegionId = INT_MAX;
	unsigned int m_RegionTax = 0;
	std::string m_RegionName = "";
	sf::VertexArray m_VertexArray;
	MapRegion() {};
	~MapRegion() { m_MapSquares.clear(); }
};

struct Map : public Component
{
	std::vector<MapRegion> m_Regions;
	sf::Texture m_LandTexture;
	sf::Sprite m_LandSprite;

	int m_XOffset = 100;
	int m_YOffset = 100;
	float m_MapScale = 0.6f;

	bool m_ChangeFlag = false;
	bool m_UpdateMapInfo = false;

	sf::RenderStates m_RenderStates;
	float m_TileSize = 32;
	float m_HalfTileSize;

	sf::Shader m_LandShader;

	Map()
	{}

	Map(sf::Texture landTexture)
	{
		m_HalfTileSize = m_TileSize * 0.5f;
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
		LoadShadersAndCreateRenderStates();
		CreateVertexArrays();
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
					}

					x++;
				}
				y++;
			}
		}
		inData.close();
	}

	void CreateVertexArrays()
	{
		for (auto& region : m_Regions)
		{
			region.m_VertexArray.setPrimitiveType(sf::Triangles);

			for (auto& square : region.m_MapSquares)
			{
				sf::Vertex v1;
				sf::Vertex v2;
				sf::Vertex v3;
				sf::Vertex v4;

				v1.position = { (square.x * m_TileSize) - m_HalfTileSize + m_XOffset, (square.y * m_TileSize) - m_HalfTileSize + m_YOffset }; // Top left
				v1.color = sf::Color::Green;
				v2.position = { (square.x * m_TileSize) + m_HalfTileSize + m_XOffset, (square.y * m_TileSize) - m_HalfTileSize + m_YOffset }; // Top right
				v2.color = sf::Color::Green;
				v3.position = { (square.x * m_TileSize) + m_HalfTileSize + m_XOffset, (square.y * m_TileSize) + m_HalfTileSize + m_YOffset }; // Bottom right
				v3.color = sf::Color::Green;
				v4.position = { (square.x * m_TileSize) - m_HalfTileSize + m_XOffset, (square.y * m_TileSize) + m_HalfTileSize + m_YOffset }; // Bottom Left
				v4.color = sf::Color::Green;

				region.m_VertexArray.append(v1);
				region.m_VertexArray.append(v2);
				region.m_VertexArray.append(v3);
				region.m_VertexArray.append(v1);
				region.m_VertexArray.append(v3);
				region.m_VertexArray.append(v4);
			}
		}
	}

	void LoadShadersAndCreateRenderStates()
	{
		m_LandShader.loadFromFile("Assets/Shaders/LandShader.vert", "Assets/Shaders/LandShader.frag");

		m_RenderStates.shader = &m_LandShader;
	}

	Map& Map::operator =(const Map& other)
	{
		m_Regions = other.m_Regions;
		m_LandTexture = other.m_LandTexture;
		m_LandSprite = other.m_LandSprite;

		m_XOffset = other.m_XOffset;
		m_YOffset = other.m_YOffset;
		m_MapScale = other.m_MapScale;

		m_ChangeFlag = other.m_ChangeFlag;
		m_UpdateMapInfo = other.m_UpdateMapInfo;

		m_RenderStates = other.m_RenderStates;
		m_TileSize = other.m_TileSize;
		m_HalfTileSize = other.m_HalfTileSize;

		LoadShadersAndCreateRenderStates();

		return *this;
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