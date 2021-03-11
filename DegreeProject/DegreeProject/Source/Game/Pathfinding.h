#pragma once

#include "Engine/Vector2D.h"
#include "Game/Components/Map.h"

struct Node
{
	std::vector<Node> m_Neighbours = std::vector<Node>(); 
	Vector2DInt m_Position;

	Node(Vector2DInt position)
	{
		m_Position = position;
	}
};

class Pathfinding
{
	std::vector<Node> m_Map = std::vector<Node>();

	void Init(const std::vector<MapRegion>& map);

	std::vector<Node> GetNeighbours(const Node& node, const std::vector<MapRegion>& map);

	std::vector<Vector2DInt> FindPath(Vector2DInt start, Vector2DInt end);
};
