#include "Game/Pathfinding.h"

void Pathfinding::Init(const std::vector<MapRegion>& map)
{
	for (const auto& region : map)
	{
		for (const auto& square : region.m_MapSquares)
		{
			Vector2DInt key = Vector2DInt(square.x, square.y);
			Node newNode(key);
			m_Map.insert(std::make_pair(key, newNode));
		}
	}
	for (auto& mapPair : m_Map)
	{
		CalculateNeighbours(mapPair.second);
	}
}

void Pathfinding::CalculateNeighbours(Node& node)
{
	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			if (x == 0 && y == 0)
			{
				continue;
			}
			Vector2DInt neighbourPosition = node.m_Position + Vector2DInt(x, y);
			if (m_Map.find(neighbourPosition) != m_Map.end())
			{
				node.m_Neighbours.push_back(m_Map[neighbourPosition]);
			}
		}
	}
}

std::vector<Vector2DInt> Pathfinding::FindPath(Vector2DInt start, Vector2DInt end)
{
	return std::vector<Vector2DInt>();
}
