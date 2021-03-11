#include "Game/Pathfinding.h"

void Pathfinding::Init(const std::vector<MapRegion>& map)
{
	for (const auto& region : map)
	{
		for (const auto& square : region.m_MapSquares)
		{
			Node newNode(Vector2DInt(square.x, square.y));


			m_Map.push_back(newNode);
		}
	}
}

std::vector<Node> Pathfinding::GetNeighbours(const Node& node, const std::vector<MapRegion>& map)
{
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			if (x == 0 && y == 0)
			{
				continue;
			}


		}
	}
	return std::vector<Node>();
}

std::vector<Vector2DInt> Pathfinding::FindPath(Vector2DInt start, Vector2DInt end)
{
	return std::vector<Vector2DInt>();
}
