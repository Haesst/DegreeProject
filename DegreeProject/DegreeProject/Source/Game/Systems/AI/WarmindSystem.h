#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Game/Components/Warmind.h"
#include "Game/Components/CharacterComponent.h"

struct WarmindSystem : System
{
	EntityManager* m_EntityManager = nullptr;


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

		for (EntityID ent : m_Entities)
		{
			for (auto& unit : m_Warminds[ent].m_Units)
			{
				//Init positions
				int regionIndex = m_Characters[ent].m_OwnedRegionIDs[0];
				Vector2DInt capitalPos = MapInfo::GetRegionCapital(regionIndex);
				unit.m_CurrentMapPosition = capitalPos;
			}
		}
	}

	virtual void Update() override
	{
		m_TickAccu++;

		for (auto entity : m_Entities)
		{
			if (!m_Warminds[entity].m_Active)
			{
				continue;
			}

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
