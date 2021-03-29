#pragma once

struct MapData
{
	std::vector<MapRegion> m_Regions;

	sf::Texture m_LandTexture;
	sf::RenderStates m_RenderStates;
	sf::Shader m_Shader;

	const char* m_RegionPath = "Assets/Data/Regions.json";
	const char* m_MapPath = "Assets/Map/RegionMap.txt";
	const char* m_FragmentShaderPath = "Assets/Shaders/LandShader.frag";
	const char* m_VertexShaderPath = "Assets/Shaders/LandShader.vert";
};