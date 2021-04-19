#include "Game/Pathfinding.h"
#include <queue>
#include <list>

#include <chrono>

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

float Pathfinding::calculateHCost(const Vector2DInt current, const Vector2DInt next)
{
	return (float)std::abs(current.x - next.x) + std::abs(current.y - next.y);
}

Pathfinding& Pathfinding::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new Pathfinding();
	}

	return *m_Instance;
}

int Pathfinding::indexOf(Node node) const
{
	return node.x + node.y * m_MapWidth;
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

	std::vector<Vector2DInt> path;

	auto t1 = std::chrono::high_resolution_clock::now();
	if (usePerformantPath)
	{
		path = findPerformantPath(start, end);
	}
	else
	{
		path = findDeterministicPath(start, end);
	}
	auto t2 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> ms_double = t2 - t1;

	LOG_INFO("Pathfinder elapsed time: {0}", ms_double.count());

	return path;
}

std::vector<Vector2DInt> Pathfinding::findDeterministicPath(Vector2DInt start, Vector2DInt end)
{
	//f = start -> current node
	//h = current node -> goal
	//g = Sum of f and h

	auto cmp = [](const Node& left, const Node& right) {return (left.m_GCost) >= (right.m_GCost); };

	size_t mapSize = m_MapWidth * m_MapHeight;

	std::vector<int> visited = std::vector<int>(mapSize);

	std::vector<Node> currentNodes = std::vector<Node>(mapSize);
	std::vector<Node> parents = std::vector<Node>(mapSize);

	std::priority_queue<Node, std::vector<Node>, decltype(cmp)> openList(cmp);

	std::vector<Vector2DInt> path = std::vector<Vector2DInt>();
	path.reserve((size_t)mapSize);

	//Calculating from end to start so we don't have to reverse the path

	currentNodes[end.x + end.y * m_MapWidth].m_GCost = calculateHCost(end, start);
	currentNodes[end.x + end.y * m_MapWidth].m_FCost = 0;
	currentNodes[end.x + end.y * m_MapWidth].x = end.x;
	currentNodes[end.x + end.y * m_MapWidth].y = end.y;
	openList.push(currentNodes[end.x + end.y * m_MapWidth]);

	Node startNode;

	startNode.x = end.x;
	startNode.y = end.y;

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

		const int dirx[] = { 0, 0, -1, 1, -1, 1, -1, 1 };
		const int diry[] = { -1, 1, 0, 0, 1, -1, -1, 1 };

		for (int i = 0; i < 8; ++i)
		{
			const int x = current.x + dirx[i];
			const int y = current.y + diry[i];

			if (!Map::get().mapSquareDataContainsKey({ x,  y }))
			{
				continue;
			}

			Node neighbour;
			neighbour.x = x;
			neighbour.y = y;


			const int neighbourIndex = indexOf(neighbour);

			currentNodes[neighbourIndex].x = x;
			currentNodes[neighbourIndex].y = y;

			float possibleLowerCost = current.m_FCost + calculateHCost({ current.x, current.y }, { neighbour.x, neighbour.y });

			if (possibleLowerCost < currentNodes[indexOf(neighbour)].m_FCost)
			{
				currentNodes[indexOf(neighbour)].m_FCost = possibleLowerCost;
				currentNodes[indexOf(neighbour)].m_GCost = possibleLowerCost + calculateHCost({ neighbour.x, neighbour.y }, { goalNode.x, goalNode.y });
				parents[indexOf(neighbour)] = current;
			}

			if (visited[indexOf(neighbour)] != 1)
			{
				openList.push(currentNodes[indexOf(neighbour)]);
			}
		}
	}

	Node currentNode = parents[indexOf(goalNode)];

	while (currentNode.x != end.x || currentNode.y != end.y)
	{
		path.push_back({ currentNode.x, currentNode.y });
		currentNode = parents[indexOf(currentNode)];
	}

	path.push_back(end);
	return path;
}

std::vector<Vector2DInt> Pathfinding::findPerformantPath(Vector2DInt start, Vector2DInt end)
{
	//f = start -> current node
	//h = current node -> goal
	//g = Sum of f and h

	auto cmp = [](const Node& left, const Node& right) {return (left.m_GCost) >= (right.m_GCost); };

	size_t mapSize = m_MapWidth * m_MapHeight;

	std::vector<int> visited = std::vector<int>(mapSize);
	std::vector<Node> parents = std::vector<Node>(mapSize);

	std::priority_queue<Node, std::vector<Node>, decltype(cmp)> openList(cmp);

	std::vector<Vector2DInt> path = std::vector<Vector2DInt>();
	path.reserve(mapSize);

	Node startNode;

	startNode.x = end.x;
	startNode.y = end.y;
	startNode.m_FCost = 0;
	startNode.m_GCost = calculateHCost(end, start);
	openList.push(startNode);

	Node goalNode;
	goalNode.x = start.x;
	goalNode.y = start.y;

	const int dirx[] = { 0, 0, -1, 1, -1, 1, -1, 1 };
	const int diry[] = { -1, 1, 0, 0, 1, -1, -1, 1 };

	while (!openList.empty())
	{
		if (openList.empty())
		{
			break;
		}

		const Node current = openList.top();
		openList.pop();

		if (current.x == goalNode.x && current.y == goalNode.y)
		{
			break;
		}

		for (int i = 0; i < 8; ++i)
		{
			const int x = current.x + dirx[i];
			const int y = current.y + diry[i];

			if (!Map::get().mapSquareDataContainsKey({ x,  y }))
			{
				if (!Map::get().isWater({ x, y }))
				{
					continue;
				}
			}

			Node neighbour;
			neighbour.x = x;
			neighbour.y = y;

			const int neighbourIndex = indexOf(neighbour);

			if (visited[neighbourIndex])
			{
				continue;
			}

			neighbour.m_FCost = current.m_FCost + calculateHCost({ current.x, current.y }, { neighbour.x, neighbour.y });
			neighbour.m_GCost = current.m_FCost + calculateHCost({ neighbour.x, neighbour.y }, { goalNode.x, goalNode.y });

			if (Map::get().isWater({neighbour.x, neighbour.y}))
			{
				neighbour.m_GCost += 10.0f;
			}

			visited[neighbourIndex] = 1;
			parents[neighbourIndex] = current;
			openList.push(neighbour);
		}
	}

	Node currentNode = parents[indexOf(goalNode)];

	while (currentNode.x != end.x || currentNode.y != end.y)
	{
		path.push_back({ currentNode.x, currentNode.y });
		currentNode = parents[indexOf(currentNode)];
	}

	path.push_back(end);
	return path;
}
