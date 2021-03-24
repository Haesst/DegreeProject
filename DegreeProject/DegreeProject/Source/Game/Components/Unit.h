#pragma once
#include "ECS/Component.h"
#include "Game/Components/CharacterComponent.h"
#include <string>
#include <Engine\AssetHandler.h>
#include "Game/GameDate.h"

struct UnitComponent : public Component
{
	int m_RepresentedForce = 0;
	bool m_Raised = false;
	std::list<Vector2DInt> m_CurrentPath = std::list<Vector2DInt>();
	std::list<sf::RectangleShape> m_TargetPath = std::list<sf::RectangleShape>();
	sf::RectangleShape m_EndPosition;
	Vector2D m_Target;
	Vector2D m_Direction;
	bool m_Moving = false;
	int m_SeizingRegionID = -1;
	int m_DaysSeizing = 0;

	Vector2D m_LastPosition;
	sf::RectangleShape m_HighlightShape;
	sf::Color m_FillColor = sf::Color::Transparent;
	sf::Color m_OutlineColor = sf::Color::White;
	sf::Vector2<float> m_HighlightShapeSize = sf::Vector2(32.0f, 32.0f);
	float m_OutlineThickness = 0.0f;
	bool m_PlayerSelected = false;
	bool m_PlayerControlled = false;

	sf::RectangleShape m_InnerSeizeMeter;
	sf::RectangleShape m_OuterSeizeMeter;

	Date m_LastSeizeDate = Date(0,0,0);
	float m_Speed = 150.0f;
	EntityID m_Owner = INT_MAX;
	EntityManager* m_EntityManager;

	bool m_InCombat = false;
	float m_CombatTimer = 3.0f;
	float m_CombatTimerAccu = 0.0f;
	UnitComponent* EnemyArmy = nullptr;

	UnitComponent()
	{
		m_EntityManager = &EntityManager::Get();
	}

	UnitComponent(int representedForce, EntityID owner)
		: UnitComponent()
	{
		m_RepresentedForce = representedForce;
		m_Owner = owner;
	}

	void SetPath(std::list<Vector2DInt> path, const Vector2D& startingPosition)
	{
		if (m_InCombat)
		{
			return;
		}

		if (path.size() <= 0)
		{
			return;
		}

		if (m_SeizingRegionID >= 0)
		{
			m_SeizingRegionID = -1;
			m_DaysSeizing = 0;
		}

		if (Map::m_MapUnitData.find(path.front()) == Map::m_MapUnitData.end())
		{
			return;
		}

		m_CurrentPath = path;
		m_Moving = true;

		Vector2DInt firstPositionMap = m_CurrentPath.front();
		m_Target = Map::ConvertToScreen(firstPositionMap);

		m_Direction = (m_Target - startingPosition).Normalized();
		m_CurrentPath.pop_front();
	}
}; 
