#pragma once
#include <vector>
#include "Engine/Vector2D.h"
#include "RegionBuilding.h"
#include "Game/Data/HeraldicShield.h"

struct MapRegion
{
	char m_MapChar = '1';
	unsigned int m_RegionId = INT_MAX;
	bool m_Occupied = false;
	unsigned int m_RegionTax = 0;
	bool m_Highlighted = false;
	std::vector<Vector2DInt> m_MapSquares = {};
	Vector2DInt m_RegionCapital = Vector2DInt();
	sf::Color m_HighlightColor = sf::Color::White;
	std::string m_RegionName = "";
	unsigned int m_OwnerID = 0;
	unsigned int m_DaysToSeize = 60;
	sf::VertexArray m_VertexArray;
	RegionBuilding m_BuildingSlots[4];
	HeraldicShield m_HeraldicShield;
};