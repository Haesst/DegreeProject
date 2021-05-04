#pragma once

struct MapData
{
	std::vector<MapRegion> m_Regions;
	std::unordered_map<char, unsigned int> m_CharToRegionIndex;

	sf::Texture m_LandTexture;
	sf::RenderStates m_RenderStates;
	sf::Shader m_Shader;
	sf::RenderStates m_WaterRenderStates;
	sf::Shader m_WaterShader;

	const char* m_RegionPath = "Assets/Data/NewRegions.json";
	const char* m_MapPath = "Assets/Map/NewMap.txt";
	const char* m_FragmentShaderPath = "Assets/Shaders/LandShader.frag";
	const char* m_VertexShaderPath = "Assets/Shaders/LandShader.vert";
	const char* m_WaterFragShaderPath = "Assets/Shaders/WaterShader.frag";
};