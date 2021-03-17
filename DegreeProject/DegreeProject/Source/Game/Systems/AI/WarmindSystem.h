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

			LOG_INFO("{0}", m_Characters[entity].m_Name);

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
			int regionIndex = m_Characters[Id].m_OwnedRegionIDs[1];
			Vector2DInt capitalPos = Map::GetRegionCapitalLocation(regionIndex);
			Vector2D pos = Map::ConvertToScreen(capitalPos);
			LOG_INFO("{0} Is raising units", m_Characters[Id].m_Name);

			RaiseUnits(m_Warminds[Id].m_UnitEntity, unit, renderer, pos);
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
				//std::vector<Vector2DInt> squares = Map::GetRegionPositions(m_Warminds[Id].m_WargoalRegionId);
				//int randomIndex = rand() % squares.size();
				//unit.m_CurrentPath = Pathfinding::FindPath(unit.m_CurrentMapPosition, squares[randomIndex]);
			}
		}
	}

	void RaiseUnits(EntityID ent, UnitComponent& unit, SpriteRenderer& renderer, const Vector2D& position)
	{
		auto& transform = m_EntityManager->GetComponent<Transform>(ent);
		transform.m_Position = position;
		renderer.m_Sprite.setPosition(position.x, position.y);
		unit.m_Raised = true;
		renderer.m_ShouldRender = true;
	}
};
