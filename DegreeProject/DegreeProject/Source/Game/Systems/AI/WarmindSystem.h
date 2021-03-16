#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Game/Components/Warmind.h"
#include "Game/Components/CharacterComponent.h"

struct WarmindSystem : System
{
	EntityManager* m_EntityManager = nullptr;

	bool m_Active = false;

	WarmindComponent* m_Warminds = nullptr;
	CharacterComponent* m_Characters = nullptr;

	float m_AtWarTickRate = 2.0f;
	float m_TickAccu = 0.0f;

	WarmindSystem()
	{
		AddComponentSignature<WarmindComponent>();
		AddComponentSignature<CharacterComponent>();
		m_EntityManager = &EntityManager::Get();

		m_Warminds = m_EntityManager->GetComponentArray<WarmindComponent>();
		m_Characters = m_EntityManager->GetComponentArray<CharacterComponent>();
	}

	virtual void Update() override
	{
		m_TickAccu++;

		if (!m_Active)
		{
			return;
		}


		for (auto entity : m_Entities)
		{
			if (m_TickAccu <= m_AtWarTickRate)
			{
				if (m_Characters[entity].m_AtWar)
				{
					OnWarStarted(entity);
				}
				
				m_TickAccu = 0.0f;
			}
		}
	}

	void OnWarStarted(EntityID Id)
	{
		for (unsigned int i = 0; i < m_Warminds[Id].m_Units.size(); i++)
		{
			if (!m_Warminds[Id].m_Units[i].m_Raised)
			{
				m_Warminds[Id].m_Units[i].m_Raised = true;
				//Set positions here
			}
		}
	}

	void GiveUnitOrders(EntityID Id)
	{
		for (unsigned int i = 0; i < m_Warminds[Id].m_Units.size(); i++)
		{
			if (m_Warminds[Id].m_Units[i].m_Raised)
			{
				if (!m_Warminds[Id].m_Defending)
				{
					std::vector<Vector2DInt> squares = MapInfo::GetRegionPositions(m_Warminds[i].m_WargoalRegionId);
					int randomIndex = rand() % squares.size();
					m_Warminds[Id].m_Units[i].m_CurrentPath = Pathfinding::FindPath(m_Warminds[Id].m_Units[i].m_Position, squares[randomIndex]);
				}
			}
		}
	}
};
