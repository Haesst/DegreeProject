#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Game/Components/Warmind.h"
#include "Game/Components/CharacterComponent.h"

struct WarmindSystem : System
{
	EntityManager* m_EntityManager = nullptr;

	bool m_Active = false;

	WarmindComponent* warminds = nullptr;
	CharacterComponent* characters = nullptr;

	float m_AtWarTickRate = 2.0f;
	float m_TickAccu = 0.0f;

	WarmindSystem()
	{
		AddComponentSignature<WarmindComponent>();
		AddComponentSignature<CharacterComponent>();
		m_EntityManager = &EntityManager::Get();

		warminds = m_EntityManager->GetComponentArray<WarmindComponent>();
		characters = m_EntityManager->GetComponentArray<CharacterComponent>();
	}

	virtual void Update() override
	{
		m_TickAccu++;

		//if (!m_Active)
		//{
		//	return;
		//}


		for (auto entity : m_Entities)
		{
			if (m_TickAccu <= m_AtWarTickRate)
			{
				if (characters[entity].m_AtWar)
				{
					OnWarStarted(entity);
				}
				
				m_TickAccu = 0.0f;
			}
		}
	}

	void OnWarStarted(EntityID Id)
	{
		for (unsigned int i = 0; i < warminds[Id].m_Units.size(); i++)
		{
			if (!warminds[Id].m_Units[i].m_Raised)
			{
				warminds[Id].m_Units[i].m_Raised = true;
				//Set positions here
			}
		}
	}

	void GiveUnitOrders(EntityID Id)
	{
		for (unsigned int i = 0; i < warminds[Id].m_Units.size(); i++)
		{
			if (warminds[Id].m_Units[i].m_Raised)
			{
				
			}
		}
	}
};
