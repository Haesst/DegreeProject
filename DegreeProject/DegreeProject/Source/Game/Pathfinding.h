#pragma once

#include "Engine/Vector2D.h"
#include "Game/Map/Map.h"

struct Node
{
	float m_GCost = FLT_MAX;
	float m_FCost = FLT_MAX;

	bool m_Blocker = false;

	int x;
	int y;
};

class Pathfinding
{
public:
	Pathfinding();
	~Pathfinding();
	float calculateHCost(const Vector2DInt a, const Vector2DInt b);

	static Pathfinding* m_Instance;
	static Pathfinding& get();

	std::vector<Node*> m_Map;
	bool isInMap(const Vector2DInt& key);
	Node* getNodeFromKey(const Vector2DInt& key);

	void init();

	bool endMet(Node current, Node end);

	void calculateNeighbours(Node* node);

	int indexOf(Node node) const;

	void clearNodeData();

	std::vector<Vector2DInt> findPath(Vector2DInt start, Vector2DInt end);
	void findThreadedPath(std::list<Node*> openList, Node* endNode);

	std::mutex m_PathMutex;
	std::thread m_PathThread;


	int m_MapWidth = 0;
	int m_MapHeight = 0;
};