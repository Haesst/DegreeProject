#include "Game/Pathfinding.h"
#include <list>

std::map<Vector2DInt, Node> Pathfinding::m_Map;

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

float Pathfinding::CalculateHCost(const Node& a, const Node& b)
{
	return std::abs(a.m_Position.x - b.m_Position.x) + std::abs(a.m_Position.y - b.m_Position.y);
}

float Pathfinding::CalculateGCost(const Node& a, const Node& b)
{
	return 0.0f;
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

void Pathfinding::ClearNodeData()
{
	for (auto& pair : m_Map)
	{
		Node& node = pair.second;

		node.m_FCost = FLT_MAX;
		node.m_GCost = FLT_MAX;
		node.m_FCost = FLT_MAX;
		node.m_Visited = false;
		node.m_Parent = nullptr;
	}
}

std::list<Vector2DInt> Pathfinding::FindPath(Vector2DInt start, Vector2DInt end)
{
	//f = start -> current node
	//h = current node -> goal
	//g = Sum of f and h

	std::list<Node> openList;
	std::list<Vector2DInt> closedList;
	openList.push_back(start);


	while (!openList.empty())
	{
		openList.sort([](const Node& lhs, const Node& rhs) { return lhs.m_GCost < rhs.m_GCost; });
		Node node = openList.front();
		openList.pop_front();
		node.m_Visited = true;

		for (auto& neighbour : node.m_Neighbours)
		{
			if (!neighbour.m_Visited)
			{
				if (neighbour.m_Position == end)
				{
					closedList.push_back(node.m_Position);
					return closedList;
				}

				float possibleLowerCost = node.m_FCost + CalculateHCost(node, neighbour);

				if (possibleLowerCost < neighbour.m_FCost)
				{
					neighbour.m_Parent = &node;
					neighbour.m_FCost = possibleLowerCost;
					neighbour.m_HCost = CalculateHCost(node, m_Map[end]);
					neighbour.m_GCost = neighbour.m_FCost + neighbour.m_HCost;
				}

				openList.push_back(neighbour);
			}
		}
	}

	std::list<Vector2DInt> finalPath;

	Node* currentNode = &m_Map[end];

	while (currentNode != nullptr)
	{
		finalPath.push_back(currentNode->m_Position);
		currentNode = currentNode->m_Parent;
	}

	ClearNodeData();
	std::reverse(finalPath.begin(), finalPath.end());
	return finalPath;
}