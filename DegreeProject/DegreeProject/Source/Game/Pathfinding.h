#pragma once

#include "Engine/Vector2D.h"
#include "Game/Components/Map.h"

struct Node
{
	std::vector<Node> m_Neighbours = std::vector<Node>();
	std::vector<Node> m_Slowdowns = std::vector<Node>();
	std::vector<Node> m_Obstacles = std::vector<Node>();

	float m_GCost = FLT_MAX;
	float m_HCost = FLT_MAX;
	float m_FCost = FLT_MAX;

	Node* m_Parent;

	bool m_Visited = false;

	Vector2DInt m_Position;

	Node()
	{
		m_Position = { 0,0 };
	}

	Node(Vector2DInt position)
	{
		m_Position = position;
	}

	int Cost(Vector2DInt next)
	{
		for each (Node node in m_Slowdowns)
		{
			if(node.m_Position == next)
			{
				return 3;
			}
		}
		return 1;
	}

	bool Passable(Vector2DInt id)
	{
		for each (Node node in m_Obstacles)
		{
			if (node.m_Position == id)
			{
				return true;
			}
		}
		return false;
	}

	int Heuristic(Vector2DInt start, Vector2DInt goal)
	{
		return abs(start.x - goal.x) + abs(start.y - goal.y);
	}
};

class Pathfinding
{
public:
	static float CalculateHCost(const Node& a, const Node& b);
	static float CalculateGCost(const Node& a, const Node& b);
	static std::map<Vector2DInt, Node> m_Map;

	static void Init(const std::vector<MapRegion>& map);

	static void CalculateNeighbours(Node& node);

	static void ClearNodeData();

	static std::list<Vector2DInt> FindPath(Vector2DInt start, Vector2DInt end);
};