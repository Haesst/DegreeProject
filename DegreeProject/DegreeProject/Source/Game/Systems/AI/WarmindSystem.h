#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Game/Components/Warmind.h"
#include "WarmindConsiderations.h"
#include "Game/Components/CharacterComponent.h"
#include "Game/Pathfinding.h"
#include "UnitSystem.h"

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

	virtual void Update() override
	{
		for (auto entity : m_Entities)
		{
			if (!m_Warminds[entity].m_Active)
			{
				continue;
			}

			if (m_Units[m_Characters[entity].m_UnitEntity].m_Raised)
			{
				m_Warminds[entity].m_OrderAccu += Time::DeltaTime();

				if (m_Warminds[entity].m_OrderAccu >= m_Warminds[entity].m_OrderTimer)
				{
					m_Warminds[entity].m_PrioritizedWar = ConsiderPrioritizedWar(entity);

					m_Warminds[entity].m_OrderAccu = 0.0f;
					ConsiderOrders(entity, m_Units[m_Characters[entity].m_UnitEntity], m_Warminds[entity].m_Opponent);
				}
			}
		}
	}

	void OrderSiegeCapital(EntityID warmindID, UnitComponent& unit)
	{
		Vector2D unitPosition = m_EntityManager->GetComponent<Transform>(m_Characters[warmindID].m_UnitEntity).m_Position;
		Vector2DInt startingPosition = Map::ConvertToMap(unitPosition);

		Vector2DInt capitalPosition;
		War& currentWar = *m_Warminds[warmindID].m_PrioritizedWar;

		if (currentWar.IsAttacker(warmindID))
		{
			capitalPosition = Map::GetRegionCapitalLocation(m_Warminds[warmindID].m_WargoalRegionId);
		}

		else
		{
			CharacterComponent& enemyCharacter = m_Warminds[warmindID].m_PrioritizedWar->GetAttacker();

			if (m_Units[m_Characters[warmindID].m_UnitEntity].m_DaysSeizing > 0)
			{
				return;
			}
			
			int randomRegionIndex = rand() % enemyCharacter.m_OwnedRegionIDs.size();
			capitalPosition = Map::GetRegionCapitalLocation(enemyCharacter.m_OwnedRegionIDs[randomRegionIndex]);
		}

		MoveUnit(unit.m_EntityID, capitalPosition);
	}

	void OrderFightEnemyArmy(EntityID warmindID, UnitComponent& unit)
	{
		if (m_Warminds[warmindID].m_PrioritizedWar == nullptr)
		{
			return;
		}

		auto& enemyChar = m_Characters[m_Warminds[warmindID].m_Opponent];
		auto& enemyUnit = m_Units[enemyChar.m_UnitEntity];

		if (!enemyUnit.m_Raised)
		{
			return;
		}

		Vector2DInt battlefieldIntPosition;

		if (enemyUnit.m_CurrentPath.size() > 0) /*&& m_Warminds[warmindID].m_PrioritizedWar->IsAttacker(warmindID))*/
		{
			LOG_INFO("{0} is chasing the enemy army", m_Characters[warmindID].m_Name);
			battlefieldIntPosition = enemyUnit.m_CurrentPath.back();
		}

		else
		{
			battlefieldIntPosition = Map::ConvertToMap(m_Transforms[enemyUnit.GetID()].m_Position);
		}

		MoveUnit(unit.m_EntityID, battlefieldIntPosition);
	}

	void ConsiderOrders(EntityID warmind, UnitComponent& unit, EntityID target)
	{
		if (m_Warminds[warmind].m_PrioritizedWar == nullptr)
		{
			ConsiderPrioritizedWar(warmind);
		}

		m_Units[m_Characters[warmind].m_UnitEntity].m_Moving = false;
		auto& enemyUnit = m_Units[m_Characters[target].m_UnitEntity]; //change to character

		SiegeCapitalConsideration siegeConsideration;
		float siegeEval = siegeConsideration.Evaluate(warmind, target);
		FightEnemyArmyConsideration fightConsideration;
		float fightEval = fightConsideration.Evaluate(warmind, target);

		if (m_Warminds[warmind].m_PrioritizedWar->GetDefender().GetID() == warmind)
		{
			fightEval += .2f;
		}

		if (siegeEval > fightEval || !enemyUnit.m_Raised)
		{
			if (!m_Warminds[warmind].m_PrioritizedWar->IsDefender(target))
			{
				LOG_INFO("Warmind belonging to {0} decided to siege the enemy capital", m_Characters[warmind].m_Name);
			}

			OrderSiegeCapital(warmind, m_Units[m_Characters[warmind].m_UnitEntity]);
		}

		else
		{
			LOG_INFO("Warmind belonging to {0} decided to fight the enemy army", m_Characters[warmind].m_Name);
			OrderFightEnemyArmy(warmind, unit);
		}
	}

	War* ConsiderPrioritizedWar(EntityID ent) //Will be expanded later
	{
		if (m_Characters[ent].m_CurrentWars.front().IsAttacker(ent))
		{
			m_Warminds[ent].m_Opponent = m_Characters[ent].m_CurrentWars.front().m_Defender->GetID();
		}

		else
		{
			m_Warminds[ent].m_Opponent = m_Characters[ent].m_CurrentWars.front().m_Attacker->GetID();
		}

		return &m_Characters[ent].m_CurrentWars.front();
	}

	void MoveUnit(EntityID unitToMove, Vector2DInt targetPosition)
	{
		UnitComponent& unit = m_Units[unitToMove];
		Transform& transform = m_EntityManager->GetComponent<Transform>(unitToMove);

		unit.m_Moving = false;
		transform.m_Position = unit.m_LastPosition;

		Vector2D unitPosition = transform.m_Position;
		Vector2DInt startingPosition = Map::ConvertToMap(unitPosition);
		std::list<Vector2DInt> path = Pathfinding::FindPath(startingPosition, targetPosition);

		if (path.size() > 0)
		{
			if (path.back() == unit.m_CurrentPath.back())
			{
				unit.m_Moving = true;
				return;
			}

			unit.SetPath(path, Map::ConvertToScreen(startingPosition));
#pragma warning(push)
#pragma warning(disable: 26815)
			UnitSystem* unitSystem = (UnitSystem*)m_EntityManager->GetSystem<UnitSystem>().get();
#pragma warning(pop)
			unitSystem->ShowPath(transform, unit);
		}
		else
		{
			unit.m_CurrentPath.clear();
		}
	}
};
