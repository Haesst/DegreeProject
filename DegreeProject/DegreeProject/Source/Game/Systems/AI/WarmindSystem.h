#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Game/Components/Warmind.h"
#include "Game/Components/CharacterComponent.h"
#include <Game\MapInfo.cpp>

struct WarmindSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	WarmindComponent* m_Warminds = nullptr;
	CharacterComponent* m_Characters = nullptr;
	UnitComponent* m_Units = nullptr;

	float m_AtWarTickRate = 2.0f;
	float m_TickAccu = 0.0f;

	WarmindSystem()
	{
		AddComponentSignature<WarmindComponent>();
		AddComponentSignature<CharacterComponent>();
		m_EntityManager = &EntityManager::Get();
		m_Units = m_EntityManager->GetComponentArray<UnitComponent>();
		m_Warminds = m_EntityManager->GetComponentArray<WarmindComponent>();
		m_Characters = m_EntityManager->GetComponentArray<CharacterComponent>();
	}

	virtual void Start() override
	{
		for (EntityID ent : m_Entities)
		{
			//Init positions
			int regionIndex = m_Characters[ent].m_OwnedRegionIDs[0];
			Vector2DInt capitalPos = Map::GetRegionCapitalLocation(regionIndex);
			Vector2D pos = Map::ConvertToScreen(capitalPos);
			Vector2DInt intpos(pos.x, pos.y);

			m_Warminds[ent].m_UnitEntity = m_EntityManager->AddNewEntity();
			m_EntityManager->AddComponent<UnitComponent>(m_Warminds[ent].m_UnitEntity, 1, intpos);
			AssetHandler handler;
			m_EntityManager->AddComponent<SpriteRenderer>(m_Warminds[ent].m_UnitEntity, "Assets/Graphics/soldier unit.png", 32, 32, &handler);
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

			if (m_TickAccu >= m_AtWarTickRate)
			{
				if (!m_Warminds[entity].m_AtWar)
				{
					if (m_Characters[entity].m_AtWar)
					{
						OnWarStarted(entity);
					}
				}

				m_TickAccu = 0.0f;
			}
		}
	}

	void OnWarStarted(EntityID Id)
	{
		auto warmindComp = m_EntityManager->GetComponent<WarmindComponent>(Id);

		UnitComponent& unit = m_EntityManager->GetComponent<UnitComponent>(warmindComp.m_UnitEntity);
		SpriteRenderer& renderer = m_EntityManager->GetComponent<SpriteRenderer>(warmindComp.m_UnitEntity);

		if (!unit.m_Raised)
		{
			RaiseUnits(unit, renderer);
		}
	}

	void GiveUnitOrders(EntityID Id)
	{
		auto warmindComp = m_EntityManager->GetComponent<WarmindComponent>(Id);

		UnitComponent& unit = m_EntityManager->GetComponent<UnitComponent>(warmindComp.m_UnitEntity);

		if (unit.m_Raised)
		{
			if (!m_Warminds[Id].m_Defending)
			{
				std::vector<Vector2DInt> squares = MapInfo::GetRegionPositions(m_Warminds[Id].m_WargoalRegionId);
				int randomIndex = rand() % squares.size();
				unit.m_CurrentPath = Pathfinding::FindPath(unit.m_CurrentMapPosition, squares[randomIndex]);
			}
		}
	}

	void RaiseUnits(UnitComponent& unit, SpriteRenderer& renderer)
	{
		unit.m_Raised = true;
		renderer.m_ShouldRender = true;
	}
};
