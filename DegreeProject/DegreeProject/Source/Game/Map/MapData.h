#pragma once

struct MapData
{
	std::vector<MapRegion> m_Regions;

	sf::Texture m_LandTexture;
	sf::RenderStates m_RenderStates;
	sf::Shader m_Shader;

	int m_XOffset = -300;
	int m_YOffset = -100;
	float m_TileSize = 32;
	float m_HalfTileSize;

	MapData()
	{
		m_HalfTileSize = m_TileSize * 0.5f;
	}
};