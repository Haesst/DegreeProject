#pragma once

#include "Engine/Vector2D.h"
#include "Game/Components/Map.h"

struct Node
{
	std::vector<Node*> m_Neighbours = std::vector<Node*>();

	float m_GCost = FLT_MAX;
	float m_HCost = FLT_MAX;
	float m_FCost = FLT_MAX;

	Node* m_Parent = nullptr;

	bool m_Visited = false;

	Vector2DInt m_Position;

	Node()
	{
		m_Position = { 0, 0 };
	}

	Node(Vector2DInt position)
	{
		m_Position = position;
	}

	bool operator== (const Node& other)
	{
		return other.m_Position.x == m_Position.x && other.m_Position.y == m_Position.y;
	}
};

class Pathfinding
{
public:
	~Pathfinding();
	static float CalculateHCost(const Node& a, const Node& b);
	static float CalculateGCost(const Node& a, const Node& b);

	static std::vector<Node*> m_Map;
	static bool IsInMap(const Vector2DInt& key);
	static Node* GetNodeFromKey(const Vector2DInt& key);

	static void Init(const std::vector<MapRegion>& map);

	static void CalculateNeighbours(Node* node);

	static void ClearNodeData();

	static std::list<Vector2DInt> FindPath(Vector2DInt start, Vector2DInt end);
};