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
			m_Warminds[ent].m_UnitEntity = m_EntityManager->AddNewEntity();
			m_EntityManager->AddComponent<UnitComponent>(m_Warminds[ent].m_UnitEntity, 1, ent);
			AssetHandler handler;
			m_EntityManager->AddComponent<SpriteRenderer>(m_Warminds[ent].m_UnitEntity, "Assets/Graphics/soldier unit.png", 32, 32, &handler);
			auto warmindComp = m_EntityManager->GetComponent<WarmindComponent>(ent);
			SpriteRenderer& renderer = m_EntityManager->GetComponent<SpriteRenderer>(warmindComp.m_UnitEntity);
			renderer.m_ShouldRender = false;
		}
	}

	virtual void Update() override
	{
		for (auto entity : m_Entities)
		{
			if (!m_Warminds[entity].m_Active)
			{
				continue;
			}

			if (m_Warminds[entity].m_RecentlyAtWar)
			{
				OnWarStarted(entity);
				m_Warminds[entity].m_RecentlyAtWar = false;
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
			int regionIndex = m_Characters[Id].m_OwnedRegionIDs[0];
			Vector2DInt capitalPos = Map::GetRegionCapitalLocation(regionIndex);
			Vector2D pos = Map::ConvertToScreen(capitalPos);
			RaiseUnits(m_Warminds[Id].m_UnitEntity, Id, unit, renderer, pos);
		}
	}

	void GiveUnitOrders(EntityID warmindID, UnitComponent& unit)
	{
		//Todo: Set this on timer when more orders can be given.

		if (unit.m_Raised)
		{
			Vector2D unitPosition = m_EntityManager->GetComponent<Transform>(m_Warminds[warmindID].m_UnitEntity).m_Position;

			if (!m_Warminds[warmindID].m_Defending)
			{
				Vector2DInt startingPosition = Map::ConvertToMap(unitPosition);
				unit.SetPath(Pathfinding::FindPath(startingPosition, Map::GetRegionCapitalLocation(m_Warminds[warmindID].m_WargoalRegionId)), Map::ConvertToScreen(startingPosition));
			}
		}
	}

	void RaiseUnits(EntityID unitEnt, EntityID warmindEnt, UnitComponent& unit, SpriteRenderer& renderer, const Vector2D& position)
	{
		auto& transform = m_EntityManager->GetComponent<Transform>(unitEnt);
		transform.m_Position = position;
		renderer.m_Sprite.setPosition(position.x, position.y);

		Vector2DInt pos(position.x, position.y);

		Map::m_MapUnitData[pos].AddUnique(unitEnt);
		unit.m_LastPosition = position;

		unit.m_Raised = true;
		renderer.m_ShouldRender = true;
		GiveUnitOrders(warmindEnt, unit);
	}
};
