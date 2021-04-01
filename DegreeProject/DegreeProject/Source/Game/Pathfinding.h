#pragma once

#include "Engine/Vector2D.h"
#include "Game/Map/Map.h"

struct Node
{
	float m_GCost = FLT_MAX;
	float m_FCost = FLT_MAX;

	int x;
	int y;
};

class Pathfinding
{
public:
	Pathfinding();

	float calculateHCost(const Vector2DInt a, const Vector2DInt b);

	static Pathfinding* m_Instance;
	static Pathfinding& get();

	bool usePerformantPath = false;

	void init();

	int indexOf(Node node) const;

	std::vector<Vector2DInt> findPath(Vector2DInt start, Vector2DInt end);
	std::vector<Vector2DInt> findDeterministicPath(Vector2DInt start, Vector2DInt end);
	std::vector<Vector2DInt> findPerformantPath(Vector2DInt start, Vector2DInt end);

	int m_MapWidth = 0;
	int m_MapHeight = 0;
};