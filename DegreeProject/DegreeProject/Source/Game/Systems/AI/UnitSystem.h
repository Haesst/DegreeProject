#pragma once
#include "ECS/System.h"
#include <Game\Components\Unit.h>
#include <Engine\AssetHandler.h>

struct UnitSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	UnitComponent* m_UnitComponents = nullptr;

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

			if (!m_UnitComponents[ent].m_CurrentPath.empty())
			{
				transform.m_Position = Vector2D((float)unit.m_CurrentMapPosition.x, (float)unit.m_CurrentMapPosition.y);
				unit.m_CurrentPath.pop_front();

				unit.m_CurrentMapPosition = Vector2DInt((int)(transform.m_Position.x - 100 + 16) / 32,
					(int)(transform.m_Position.y - 100 + 16) / 32);

				//int x = (int)(transform.m_Position.x - 100 + 16) / 32;
				//int y = (int)(transform.m_Position.y - 100 + 16) / 32;
			}
		}
	}
};
