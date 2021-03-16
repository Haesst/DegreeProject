#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

#include "Engine/Vector2D.h"
#include "Engine/Log.h"
#include "Engine/Window.h"
#include "Engine/AssetHandler.h"

#include "Game/MapInfo.h"

#include "MapRegion.h"

#pragma warning(push)
#pragma warning(disable: 26812)
struct Map
{
	std::vector<MapRegion> m_Regions;
	sf::Texture m_LandTexture;
	sf::Sprite m_LandSprite;

	int m_XOffset = -300;
	int m_YOffset = -100;
	float m_MapScale = 0.6f;

	bool m_ChangeFlag = false;
	bool m_UpdateMapInfo = false;

	sf::RenderStates m_RenderStates;
	float m_TileSize = 32;
	float m_HalfTileSize;

	sf::Shader m_LandShader;

	Map()
	{
		m_HalfTileSize = m_TileSize * 0.5f;
	}

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
		ClearRegions();
		LoadAllRegions();
		ClearRegionMapTiles();
		LoadMap();
		LoadShadersAndCreateRenderStates();
		CreateVertexArrays();

		for (size_t i = 0; i < m_Regions.size(); ++i)
		{
			UpdateMapInfo(i);
		}
	}

	void SetLandTexture(sf::Texture tex)
	{
		m_LandTexture = tex;
	}

	void UpdateMapInfo(size_t regionIndex)
	{
		MapInfo::SetMapOffset(m_XOffset, m_YOffset);
		MapInfo::SetMapRegions(m_Regions);
		MapInfo::SetRegionName(m_Regions[regionIndex].m_RegionName, (unsigned int)regionIndex);
		MapInfo::SetRegionTax(m_Regions[regionIndex].m_RegionTax, (unsigned int)regionIndex);
		MapInfo::SetRegionCapital(m_Regions[regionIndex].m_RegionCapital, (unsigned int)regionIndex);
		MapInfo::SetMapRegions(m_Regions);
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
			return sf::Color::Color(40, 26, 13);
		}

		return sf::Color::White;
	}

	void SetRegionColor(int regionId, sf::Color color)
	{
		m_Regions[regionId].m_HighlightColor = color;
	}

	void ClearRegions()
	{
		m_Regions.clear();
	}

	void LoadAllRegions()
	{
		std::ifstream file("Assets/Data/Regions.json");
		json j;
		file >> j;

		ASSERT(m_Regions.size() <= 0, "Regions not cleared");

		for (auto& element : j)
		{
			MapRegion region;

			// region.m_HighlightColor = GetColor(element["color"].get<std::string>());
			std::string mapCharString = element["mapchar"].get<std::string>();
			region.m_RegionId = element["id"].get<unsigned int>();
			region.m_RegionTax = element["tax"].get<unsigned int>();
			region.m_RegionName = element["name"].get<std::string>();
			region.m_RegionCapital.x = element["capitalx"].get<unsigned int>();
			region.m_RegionCapital.y = element["capitaly"].get<unsigned int>();

			if (mapCharString.size() > 0)
			{
				region.m_MapChar = mapCharString[0];
			}

			m_Regions.push_back(region);
		}
	}

	void ColorRegionAfterOwner()
	{
	}

	void ClearRegionMapTiles()
	{
		for (auto& region : m_Regions)
		{
			region.m_MapSquares.clear();
		}
	}

	void LoadMap()
	{
		ASSERT(m_Regions[0].m_MapSquares.size() <= 0);

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
					int regionPosition = GetRegionPositionFromMapCharacter(character);

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

			// Calculate texture coord formula
			// Get region x size (width * 32)
			// Get region y size (height * 32)


			for (auto& square : region.m_MapSquares)
			{
				sf::Vertex v1;
				sf::Vertex v2;
				sf::Vertex v3;
				sf::Vertex v4;

				v1.position = { (square.x * m_TileSize) - m_HalfTileSize + m_XOffset, (square.y * m_TileSize) - m_HalfTileSize + m_YOffset }; // Top left
				v2.position = { (square.x * m_TileSize) + m_HalfTileSize + m_XOffset, (square.y * m_TileSize) - m_HalfTileSize + m_YOffset }; // Top right
				v3.position = { (square.x * m_TileSize) + m_HalfTileSize + m_XOffset, (square.y * m_TileSize) + m_HalfTileSize + m_YOffset }; // Bottom right
				v4.position = { (square.x * m_TileSize) - m_HalfTileSize + m_XOffset, (square.y * m_TileSize) + m_HalfTileSize + m_YOffset }; // Bottom Left

				v1.texCoords = { (square.x * m_TileSize), (square.y * m_TileSize) };
				v2.texCoords = { (square.x * m_TileSize) + m_TileSize, (square.y * m_TileSize) };
				v3.texCoords = { (square.x * m_TileSize) + m_TileSize, (square.y * m_TileSize) + m_TileSize };
				v4.texCoords = { (square.x * m_TileSize), (square.y * m_TileSize) + m_TileSize };

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
		m_RenderStates.texture = &m_LandTexture;
	}

	int GetRegionPositionFromMapCharacter(const char& c)
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

#pragma region Rendering
	void Render()
	{
		for (auto& region : m_Regions)
		{
			m_LandShader.setUniform("u_Color", sf::Glsl::Vec4(region.m_HighlightColor));
			m_LandShader.setUniform("u_Texture", m_LandTexture);
			Window::GetWindow()->draw(region.m_VertexArray, m_RenderStates);
		}
	}
#pragma endregion Rendering
};
#pragma warning(pop)