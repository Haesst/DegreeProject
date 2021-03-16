#pragma once
#include <vector>
#include "Engine/Vector2D.h"

struct MapRegion
{
public:
	char m_MapChar = '1';
	unsigned int m_RegionId = INT_MAX;
	unsigned int m_RegionTax = 0;
	std::vector<Vector2DInt> m_MapSquares = {};
	Vector2DInt m_RegionCapital = Vector2DInt();
	sf::Color m_HighlightColor = sf::Color::White;
	std::string m_RegionName = "";
	sf::VertexArray m_VertexArray;
	MapRegion() {};
	~MapRegion() { m_MapSquares.clear(); }
};