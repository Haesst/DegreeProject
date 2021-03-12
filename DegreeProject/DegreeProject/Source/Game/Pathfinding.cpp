#include "Game/Pathfinding.h"
#include <list>

std::vector<Node*> Pathfinding::m_Map;

void Pathfinding::Init(const std::vector<MapRegion>& map)
{
	for (const auto& region : map)
	{
		for (const auto& square : region.m_MapSquares)
		{
			Vector2DInt pos = { square.x, square.y };
			m_Map.push_back(new Node{ pos });
		}
	}
	for (auto* node : m_Map)
	{
		CalculateNeighbours(node);
	}
}

Pathfinding::~Pathfinding()
{
	for (auto* node : m_Map)
	{
		delete node;
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

void Pathfinding::CalculateNeighbours(Node* node)
{
	std::vector<Vector2DInt> neighbourPos = {
		{-1, -1},
		{0, -1},
		{1, -1},
		{-1, 0},
		{1, 0},
		{-1, 1},
		{0, 1},
		{1, 1}
	};

	for (auto& n : neighbourPos)
	{
		Vector2DInt nPos = node->m_Position + n;

		Node* neigbourNode = GetNodeFromKey(nPos);

		if (neigbourNode != nullptr)
		{
			node->m_Neighbours.push_back(neigbourNode);
		}
	}
}

bool Pathfinding::IsInMap(const Vector2DInt& key)
{
	for (auto* n : m_Map)
	{
		if (n->m_Position == key)
		{
			return true;
		}
	}

	return false;
}

Node* Pathfinding::GetNodeFromKey(const Vector2DInt& key)
{
	for (auto* n : m_Map)
	{
		if (n->m_Position == key)
		{
			return n;
		}
	}

	return nullptr;
}

void Pathfinding::ClearNodeData()
{
	for (auto* node : m_Map)
	{
		node->m_FCost = FLT_MAX;
		node->m_GCost = FLT_MAX;
		node->m_FCost = FLT_MAX;
		node->m_Visited = false;
		node->m_Parent = nullptr;
	}
}

std::list<Vector2DInt> Pathfinding::FindPath(Vector2DInt start, Vector2DInt end)
{
	//f = start -> current node
	//h = current node -> goal
	//g = Sum of f and h

	Node* startNode = GetNodeFromKey(start);
	Node* endNode = GetNodeFromKey(end);

	if (startNode == nullptr)
	{
		LOG_WARN("Startnode is a nullptr");
		return std::list<Vector2DInt>();
	}
	if (endNode == nullptr)
	{
		LOG_WARN("Endnode is a nullptr");
		return std::list<Vector2DInt>();
	}

	std::list<Node*> openList;
	openList.push_back(startNode);
	startNode->m_Visited = false;

	while (!openList.empty())
	{
		while (!openList.empty() && openList.front()->m_Visited)
		{
			openList.pop_front();
		}

		if (openList.empty())
		{
			break;
		}

		openList.sort([](const Node* lhs, const Node* rhs) { return lhs->m_GCost < rhs->m_GCost; });
		Node* node = openList.front();

		node->m_Visited = true;

		for (auto& neighbour : node->m_Neighbours)
		{
			if (!neighbour->m_Visited)
			{
				if (neighbour->m_Position == end)
				{
					endNode->m_Parent = node;
					break;
				}

				float possibleLowerCost = node->m_FCost + CalculateHCost(*node, *neighbour);

				if (possibleLowerCost < neighbour->m_FCost)
				{
					neighbour->m_Parent = node;
					neighbour->m_FCost = possibleLowerCost;
					neighbour->m_HCost = CalculateHCost(*node, *endNode);
					neighbour->m_GCost = neighbour->m_FCost + neighbour->m_HCost;
				}

				openList.push_back(neighbour);
			}
		}
	}

	std::list<Vector2DInt> finalPath;

	Node* currentNode = endNode;

	while (currentNode != nullptr)
	{
		if (currentNode->m_Position != start)
		{
			finalPath.push_back(currentNode->m_Position);
		}
		currentNode = currentNode->m_Parent;
	}

	ClearNodeData();
	std::reverse(finalPath.begin(), finalPath.end());
	return finalPath;
}