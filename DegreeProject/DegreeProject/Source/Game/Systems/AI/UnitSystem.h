#pragma once
#include "ECS/System.h"
#include <Game\Components\Unit.h>
#include <Engine\AssetHandler.h>

struct UnitSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	UnitComponent* m_UnitComponents = nullptr;

	WarmindComponent* m_Warminds = nullptr;

	float m_MoveTolerance = 0.3f;

	UnitSystem()
	{
		AddComponentSignature<UnitComponent>();
		m_EntityManager = &EntityManager::Get();
		m_UnitComponents = m_EntityManager->GetComponentArray<UnitComponent>();
		m_Warminds = m_EntityManager->GetComponentArray<WarmindComponent>();
	}

	virtual void Start() override
	{
	}

	virtual void Update() override 
	{
		for (EntityID ent : m_Entities)
		{
			UnitComponent& unit = m_UnitComponents[ent];
			Transform& transform = m_EntityManager->GetComponent<Transform>(ent);

			if (unit.m_Raised)
			{
				if (unit.m_Moving)
				{
					if (!transform.m_Position.NearlyEqual(unit.m_Target, m_MoveTolerance))
					{
						Vector2D movement = unit.m_Direction * unit.m_Speed * Time::DeltaTime();
						transform.Translate(movement);
					}
					else
					{
						transform.m_Position = unit.m_Target;

						if (unit.m_CurrentPath.size() > 0)
						{
							Vector2D nextPosition = Map::ConvertToScreen(unit.m_CurrentPath.front());
							unit.m_Target = nextPosition;
							unit.m_Direction = (nextPosition - transform.m_Position).Normalized();

							unit.m_CurrentPath.pop_front();
						}

						else
						{
							unit.m_Moving = false;
						}
					}
				}
			}
		}
	}

	bool EnemyAtSquare(Vector2D square, EntityID opponent)
	{
		if (m_UnitComponents[m_Warminds[opponent].m_UnitEntity].transform->m_Position.NearlyEqual(square, m_MoveTolerance))
		{
			return true;
		}

		return false;
	}

	void EnterCombat(EntityID unit, EntityID enemyUnit)
	{
		//Very WIP early combat prototype
		//Todo: Needs more weights and values affecting the outcome than army size

		if (m_UnitComponents[unit].m_RepresentedForce > m_UnitComponents[enemyUnit].m_RepresentedForce)
		{
			m_UnitComponents[enemyUnit].m_RepresentedForce = 0;
			m_UnitComponents[enemyUnit].m_Raised = false;
		}

		else
		{
			m_UnitComponents[unit].m_RepresentedForce = 0;
			m_UnitComponents[unit].m_Raised = false;
		}
	}
};
