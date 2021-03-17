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
			m_EntityManager->AddComponent<UnitComponent>(m_Warminds[ent].m_UnitEntity, 1);
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
			LOG_INFO("{0} Wants to raise units", m_Characters[Id].m_Name);
			LOG_INFO("{0} owns {1} regions", m_Characters[Id].m_Name, m_Characters[Id].m_OwnedRegionIDs.size());

			for (int i : m_Characters[Id].m_OwnedRegionIDs)
			{
				LOG_INFO("{0}'s current region: {1}", m_Characters[Id].m_Name, i);
			}

			int regionIndex = m_Characters[Id].m_OwnedRegionIDs[0];
			Vector2DInt capitalPos = Map::GetRegionCapitalLocation(regionIndex);
			Vector2D pos = Map::ConvertToScreen(capitalPos);
			LOG_INFO("{0} Is raising units", m_Characters[Id].m_Name) ;

			RaiseUnits(m_Warminds[Id].m_UnitEntity, Id, unit, renderer, pos);
		}
	}

	void GiveUnitOrders(EntityID unitID, EntityID warmindID, WarmindComponent& warmind, UnitComponent& unit)
	{
		//Todo: Set this on timer when more orders can be given.
		LOG_INFO("ID: {0}", warmindID);
		auto& character = m_EntityManager->GetComponent<CharacterComponent>(warmindID);
		LOG_INFO("name of ID: {0}", m_Characters[warmindID].m_Name);

		if (unit.m_Raised)
		{
			if (!m_Warminds[warmindID].m_Defending)
			{
				LOG_INFO("WargoalRegionID: {0}", m_Warminds[warmindID].m_WargoalRegionId);
				Vector2D unitPosition = m_EntityManager->GetComponent<Transform>(m_Warminds[warmindID].m_UnitEntity).m_Position;
				Vector2DInt startingPosition = Map::ConvertToMap(unitPosition);
				unit.m_CurrentPath = Pathfinding::FindPath(startingPosition, Map::GetRegionCapitalLocation(m_Warminds[warmindID].m_WargoalRegionId));
				LOG_INFO("Current path size: {0}", unit.m_CurrentPath.size()); 
			}
		}
	}

	void RaiseUnits(EntityID unitEnt, EntityID warmindEnt, UnitComponent& unit, SpriteRenderer& renderer, const Vector2D& position)
	{
		LOG_INFO("RAISE UNITS ID: {0}", unitEnt);
		auto& transform = m_EntityManager->GetComponent<Transform>(unitEnt);
		transform.m_Position = position;
		renderer.m_Sprite.setPosition(position.x, position.y);
		unit.m_Raised = true;
		renderer.m_ShouldRender = true;
		GiveUnitOrders(unitEnt, warmindEnt, m_Warminds[unitEnt], unit);
	}
};
