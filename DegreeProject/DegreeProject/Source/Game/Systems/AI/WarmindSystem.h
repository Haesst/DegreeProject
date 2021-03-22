#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Game/Components/Warmind.h"
#include "WarmindConsiderations.h"
#include "Game/Components/CharacterComponent.h"
#include "Game/Pathfinding.h"

struct WarmindSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	WarmindComponent* m_Warminds = nullptr;
	CharacterComponent* m_Characters = nullptr;
	Transform* m_Transforms = nullptr;
	UnitComponent* m_Units = nullptr;
	SpriteRenderer* m_Renderers = nullptr;

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
		m_Transforms = m_EntityManager->GetComponentArray<Transform>();
	}

	virtual void Start() override
	{
		for (EntityID ent : m_Entities)
		{
			m_Warminds[ent].m_UnitEntity = m_EntityManager->AddNewEntity();
			m_EntityManager->AddComponent<UnitComponent>(m_Warminds[ent].m_UnitEntity, m_Characters[ent].m_MaxArmySize, ent);
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

			if (m_Units[m_Warminds[entity].m_UnitEntity].m_Raised)
			{
				m_Warminds[entity].m_OrderAccu += Time::DeltaTime();

				if (m_Warminds[entity].m_OrderAccu >= m_Warminds[entity].m_OrderTimer)
				{
					m_Warminds[entity].m_OrderAccu = 0.0f;
					ConsiderOrders(entity, m_Units[m_Warminds[entity].m_UnitEntity], m_Warminds[entity].m_Opponent);
				}
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
#pragma warning(push)
#pragma warning(disable: 26815)
			CharacterSystem* characterSystem = (CharacterSystem*)m_EntityManager->GetSystem<CharacterSystem>().get();
#pragma warning(pop)
			characterSystem->RaiseUnit(Id, m_Warminds[Id].m_UnitEntity, unit, renderer, capitalPos);
			LOG_INFO("Warmind belonging to {0} is considering new orders", m_Characters[Id].m_Name);
		}
	}

	void OrderSiegeCapital(EntityID warmindID, UnitComponent& unit)
	{
		Vector2D unitPosition = m_EntityManager->GetComponent<Transform>(m_Warminds[warmindID].m_UnitEntity).m_Position;
		Vector2DInt startingPosition = Map::ConvertToMap(unitPosition);

		unit.SetPath(Pathfinding::FindPath(startingPosition, Map::GetRegionCapitalLocation(m_Warminds[warmindID].m_WargoalRegionId)), Map::ConvertToScreen(startingPosition));
	}

	void OrderFightEnemyArmy(EntityID warmindID, UnitComponent& unit)
	{
		auto& enemyWarmind = m_Warminds[m_Warminds[warmindID].m_Opponent];
		auto& enemyUnit = m_Units[enemyWarmind.m_UnitEntity];

		if (!enemyUnit.m_Raised)
		{
			return;
		}

		Vector2DInt battlefieldIntPosition;

		if (enemyUnit.m_CurrentPath.size() > 0 && !m_Warminds[warmindID].m_Defending)
		{
			LOG_INFO("{0} is chasing the enemy army", m_Characters[warmindID].m_Name);
			battlefieldIntPosition = enemyUnit.m_CurrentPath.back();
		}

		else
		{
			LOG_INFO("{0} is using a random square", m_Characters[warmindID].m_Name);
			//int randomSquareIndex = rand() % Map::GetRegionById(m_Warminds[warmindID].m_WargoalRegionId).m_MapSquares.size() + 1;
			battlefieldIntPosition = Map::GetRegionById(m_Warminds[warmindID].m_WargoalRegionId).m_MapSquares[2];
		}

		Vector2DInt startingPosition = Map::ConvertToMap(m_Transforms[unit.GetID()].m_Position);
		LOG_INFO("Starting position: {0}", startingPosition);

		std::list<Vector2DInt> path = Pathfinding::FindPath(startingPosition, battlefieldIntPosition);

		if (path.size() > 0)
		{
			unit.SetPath(path, Map::ConvertToScreen(startingPosition));
		}
			
	}

	void RaiseUnits(EntityID unitEnt, UnitComponent& unit, SpriteRenderer& renderer, const Vector2D& position)
	{
		auto& transform = m_EntityManager->GetComponent<Transform>(unitEnt);
		transform.m_Position = position;
		renderer.m_Sprite.setPosition(position.x, position.y);

		Vector2DInt pos((int)position.x, (int)position.y);

		Map::m_MapUnitData[pos].AddUnique(unitEnt);
		unit.m_LastPosition = position;

		unit.m_Raised = true;
		renderer.m_ShouldRender = true;
	}

	void KillUnit(EntityID ID)
	{
		m_Units[ID].m_RepresentedForce = 0;
		m_Units[ID].m_Raised = false;
		m_Renderers[ID].m_ShouldRender = false;
	}

	void ConsiderOrders(EntityID warmind, UnitComponent& unit, EntityID targetWarmind)
	{
		SiegeCapitalConsideration siegeConsideration;
		float siegeEval = siegeConsideration.Evaluate(warmind, targetWarmind);
		FightEnemyArmyConsideration fightConsideration;
		float fightEval = fightConsideration.Evaluate(warmind, targetWarmind);
		
		if (siegeEval > fightEval)
		{
			if (!m_Warminds[warmind].m_Defending)
			{
				LOG_INFO("Warmind belonging to {0} decided to siege the enemy capital", m_Characters[warmind].m_Name);
			}

			OrderSiegeCapital(warmind, m_Units[m_Warminds[warmind].m_UnitEntity]);
		}

		else
		{
			LOG_INFO("Warmind belonging to {0} decided to fight the enemy army", m_Characters[warmind].m_Name);
			OrderFightEnemyArmy(warmind, unit);
		}
	}
};
