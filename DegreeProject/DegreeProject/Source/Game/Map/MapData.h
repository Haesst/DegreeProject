#pragma once

struct MapData
{
	std::vector<MapRegion> m_Regions;

	sf::Texture m_LandTexture;
	sf::RenderStates m_RenderStates;
	sf::Shader m_Shader;

	const int m_XOffset = -300;
	const int m_YOffset = -100;
	const float m_TileSize = 32;
	const float m_HalfTileSize = m_TileSize * 0.5f;
};