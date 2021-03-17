#pragma once
#include "ECS/Component.h"
#include "Game/Components/CharacterComponent.h"
#include <string>
#include <Engine\AssetHandler.h>

struct UnitComponent : public Component
{
	int m_RepresentedForce = 0;
	bool m_Raised = false;
	std::list<Vector2DInt> m_CurrentPath = std::list<Vector2DInt>();
	Vector2D m_Target;
	Vector2D m_Direction;
	bool m_Moving = false;
	float m_Speed = 150.0f;

	EntityManager* m_EntityManager;
	Transform* transform;

	UnitComponent()
	{
		m_EntityManager = &EntityManager::Get();
		transform = &m_EntityManager->GetComponent<Transform>(m_EntityID);
	}

	UnitComponent(int representedForce)
	{
		m_RepresentedForce = representedForce;
	}

	void SetPath(std::list<Vector2DInt> path, const Vector2D& startingPosition)
	{
		m_CurrentPath = path;
		m_Moving = true;

		Vector2DInt firstPositionMap = m_CurrentPath.front();
		m_Target = Map::ConvertToScreen(firstPositionMap);

		m_Direction = (m_Target - startingPosition).Normalized();
		m_CurrentPath.pop_front();
	}
}; 
