#pragma once
#include "ECS/System.h"
#include <Game\Components\Unit.h>
#include <Engine\AssetHandler.h>

struct UnitSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	UnitComponent* m_UnitComponents = nullptr;

	float m_MoveTolerance = 0.3f;

	UnitSystem()
	{
		AddComponentSignature<UnitComponent>();
		m_EntityManager = &EntityManager::Get();
		m_UnitComponents = m_EntityManager->GetComponentArray<UnitComponent>();
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
};
