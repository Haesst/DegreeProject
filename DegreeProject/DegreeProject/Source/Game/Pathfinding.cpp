#include "Game/Pathfinding.h"
#include <queue>
#include <list>

Pathfinding* Pathfinding::m_Instance = nullptr;

void Pathfinding::init()
{
	Vector2DInt mapSize = Map::get().getMapSize();
	m_MapWidth = mapSize.x;
	m_MapHeight = mapSize.y;
}

Pathfinding::Pathfinding()
{
	init();
}

Pathfinding::~Pathfinding()
{
	for (auto* node : m_Map)
	{
		delete node;
	}
}

float Pathfinding::calculateHCost(const Vector2DInt a, const Vector2DInt b)
{
	return (float)std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

Pathfinding& Pathfinding::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new Pathfinding();
	}

	return *m_Instance;
}

void Pathfinding::calculateNeighbours(Node* node)
{
	//std::vector<Vector2DInt> neighbourPos = {
	//	{-1, -1},
	//	{0, -1},
	//	{1, -1},
	//	{-1, 0},
	//	{1, 0},
	//	{-1, 1},
	//	{0, 1},
	//	{1, 1}
	//};
	//
	//for (auto& n : neighbourPos)
	//{
	//	Vector2DInt nPos = node->m_Position + n;
	//
	//	Node* neigbourNode = getNodeFromKey(nPos);
	//
	//	if (neigbourNode != nullptr)
	//	{
	//		node->m_Neighbours.push_back(neigbourNode);
	//	}
	//}
}

int Pathfinding::indexOf(Node node) const
{
	return node.x + node.y * m_MapWidth;
}

bool Pathfinding::isInMap(const Vector2DInt& key)
{
	//for (auto* n : m_Map)
	//{
	//	if (n->m_Position == key)
	//	{
	//		return true;
	//	}
	//}

	return false;
}

Node* Pathfinding::getNodeFromKey(const Vector2DInt& key)
{
	//for (auto* n : m_Map)
	//{
	//	if (n->m_Position == key)
	//	{
	//		return n;
	//	}
	//}

	return nullptr;
}

void Pathfinding::clearNodeData()
{
	//for (auto* node : m_Map)
	//{
	//	node->m_FCost = FLT_MAX;
	//	node->m_GCost = FLT_MAX;
	//	node->m_FCost = FLT_MAX;
	//	node->m_Visited = false;
	//	node->m_Parent = nullptr;
	//}
}

void Pathfinding::findThreadedPath(std::list<Node*> openList, Node* endNode)
{
	//while (!openList.empty())
	//{
	//	while (!openList.empty() && openList.front()->m_Visited)
	//	{
	//		openList.pop_front();
	//	}
	//
	//	if (openList.empty())
	//	{
	//		break;
	//	}
	//
	//	openList.sort([](const Node* lhs, const Node* rhs) { return lhs->m_GCost < rhs->m_GCost; });
	//	Node* node = openList.front();
	//
	//	node->m_Visited = true;
	//
	//	for (auto& neighbour : node->m_Neighbours)
	//	{
	//		if (!neighbour->m_Visited)
	//		{
	//			float possibleLowerCost = node->m_FCost + calculateHCost(*node, *neighbour);
	//
	//			if (possibleLowerCost < neighbour->m_FCost)
	//			{
	//				neighbour->m_Parent = node;
	//				neighbour->m_FCost = possibleLowerCost;
	//				neighbour->m_HCost = calculateHCost(*node, *endNode);
	//				neighbour->m_GCost = neighbour->m_FCost + neighbour->m_HCost;
	//			}
	//
	//			openList.push_back(neighbour);
	//		}
	//	}
	//}
}

std::vector<Vector2DInt> Pathfinding::findPath(Vector2DInt start, Vector2DInt end)
{
	if (start == end)
	{
		return std::vector<Vector2DInt>();
	}

	if (!Map::get().mapSquareDataContainsKey(start) || !Map::get().mapSquareDataContainsKey(end))
	{
		return std::vector<Vector2DInt>();
	}

	//f = start -> current node
	//h = current node -> goal
	//g = Sum of f and h

	auto cmp = [](const Node& left, const Node& right) {return (left.m_GCost) >= (right.m_GCost); };

	std::vector<int> visited = std::vector<int>(m_MapWidth * m_MapHeight);

	for (int& node : visited)
	{
		node = 0;
	}

	std::vector<Node> currentNodes = std::vector<Node>(m_MapWidth * m_MapHeight);

	std::vector<Node> parents = std::vector<Node>(m_MapWidth * m_MapHeight);
	std::vector<Vector2DInt> path = std::vector<Vector2DInt>();
	std::vector<float> fCosts = std::vector<float>(m_MapWidth * m_MapHeight);
	std::priority_queue<Node, std::vector<Node>, decltype(cmp)> openList(cmp);

	for (float& cost : fCosts)
	{
		cost = FLT_MAX; 
	}

	//Calculating from end to start so we dont have to reverse the path

	currentNodes[end.x + end.y * m_MapWidth].m_GCost = calculateHCost(end, start);

	currentNodes[end.x + end.y * m_MapWidth].m_FCost = 0;
	currentNodes[end.x + end.y * m_MapWidth].x = end.x;
	currentNodes[end.x + end.y * m_MapWidth].y = end.y;
	openList.push(currentNodes[end.x + end.y * m_MapWidth]);

	//Node startNode;
	//startNode.x = end.x;
	//startNode.y = end.y;
	//startNode.m_FCost = 0;
	//openList.push(startNode);

	Node goalNode;
	goalNode.x = start.x;
	goalNode.y = start.y;

	
	while (!openList.empty())
	{
		while (!openList.empty() && visited[indexOf(openList.top())] == 1)
		{
			openList.pop();
		}

		if (openList.empty())
		{
			break;
		}

	    const Node current = openList.top();
		visited[indexOf(current)] = 1;

		for (int x = -1; x < 2; x++)
		{
			for (int y = -1; y < 2; y++)
			{
				if (x == 0 && y == 0)
				{
					continue;
				}

				int nodeX = current.x + x;
				int nodeY = current.y + y;

				Node neighbour;
				neighbour.x = nodeX;
				neighbour.y = nodeY;

				currentNodes[indexOf(neighbour)].x = nodeX;
				currentNodes[indexOf(neighbour)].y = nodeY;

				float possibleLowerCost = current.m_FCost + calculateHCost({ current.x, current.y }, { neighbour.x, neighbour.y });

				if (possibleLowerCost < currentNodes[indexOf(neighbour)].m_FCost)
				{
					currentNodes[indexOf(neighbour)].m_FCost = possibleLowerCost;
					currentNodes[indexOf(neighbour)].m_GCost = possibleLowerCost + calculateHCost({ neighbour.x, neighbour.y }, { goalNode.x, goalNode.y });
					parents[indexOf(neighbour)] = current;
				}

				if (visited[indexOf(neighbour)] != 1 && Map::get().mapSquareDataContainsKey({ neighbour.x, neighbour.y }))
				{
					openList.push(currentNodes[indexOf(neighbour)]);
				}
			}
		}
	}

	Node currentNode = parents[start.x + start.y * m_MapWidth];
	std::vector<int> parentIndexes = std::vector<int>();

	while (true)
	{
		if (currentNode.x == end.x)
		{
			if (currentNode.y == end.y)
			{
				break;
			}
		}

		path.push_back({ currentNode.x, currentNode.y });
		parentIndexes.push_back(indexOf(currentNode));
		currentNode = parents[indexOf(currentNode)];
	}

	path.push_back(end);
	return path;
}

bool Pathfinding::endMet(Node current, Node end)
{
	if (current.x == end.x && current.y == end.y)
	{
		return true;
	}

	return false;
}
