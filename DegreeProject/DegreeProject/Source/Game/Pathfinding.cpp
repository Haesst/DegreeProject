#include "Game/Pathfinding.h"
#include <list>

std::vector<Node*> Pathfinding::m_Map;
std::thread Pathfinding::m_PathThread;
std::mutex Pathfinding::m_PathMutex;

void Pathfinding::init(const std::vector<MapRegion>& map)
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
		calculateNeighbours(node);
	}
}

Pathfinding::~Pathfinding()
{
	for (auto* node : m_Map)
	{
		delete node;
	}
}

float Pathfinding::calculateHCost(const Node& a, const Node& b)
{
	return (float)std::abs(a.m_Position.x - b.m_Position.x) + std::abs(a.m_Position.y - b.m_Position.y);
}

void Pathfinding::calculateNeighbours(Node* node)
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

		Node* neigbourNode = getNodeFromKey(nPos);

		if (neigbourNode != nullptr)
		{
			node->m_Neighbours.push_back(neigbourNode);
		}
	}
}

bool Pathfinding::isInMap(const Vector2DInt& key)
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

Node* Pathfinding::getNodeFromKey(const Vector2DInt& key)
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

void Pathfinding::clearNodeData()
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

void Pathfinding::findThreadedPath(std::list<Node*> openList, Node* endNode)
{
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
				float possibleLowerCost = node->m_FCost + calculateHCost(*node, *neighbour);

				if (possibleLowerCost < neighbour->m_FCost)
				{
					neighbour->m_Parent = node;
					neighbour->m_FCost = possibleLowerCost;
					neighbour->m_HCost = calculateHCost(*node, *endNode);
					neighbour->m_GCost = neighbour->m_FCost + neighbour->m_HCost;
				}

				openList.push_back(neighbour);
			}
		}
	}
}

std::list<Vector2DInt> Pathfinding::findPath(Vector2DInt start, Vector2DInt end)
{
	//f = start -> current node
	//h = current node -> goal
	//g = Sum of f and h

	if (start == end)
	{
		return std::list<Vector2DInt>();
	}

	Node* startNode = getNodeFromKey(start);
	Node* endNode = getNodeFromKey(end);

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
	startNode->m_FCost = 0;

	m_PathThread = std::thread(&Pathfinding::findThreadedPath, openList, endNode);
	m_PathThread.join();
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

	clearNodeData();
	std::reverse(finalPath.begin(), finalPath.end());
	return finalPath;
}
