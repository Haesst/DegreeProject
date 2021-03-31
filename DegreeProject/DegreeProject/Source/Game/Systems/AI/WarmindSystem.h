#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Game/Components/Warmind.h"
#include "WarmindConsiderations.h"
#include "Game/Components/CharacterComponent.h"
#include "Game/Pathfinding.h"
#include "Game/Systems/UnitSystem.h"

struct WarmindSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	WarmindComponent* m_Warminds = nullptr;
	CharacterComponent* m_Characters = nullptr;
	Transform* m_Transforms = nullptr;
	UnitComponent* m_Units = nullptr;
	SpriteRenderer* m_Renderers = nullptr;
	WarManager* m_WarManager = nullptr;

	WarmindSystem()
	{
		addComponentSignature<WarmindComponent>();
		addComponentSignature<CharacterComponent>();
		m_EntityManager = &EntityManager::get();

		m_Units = m_EntityManager->getComponentArray<UnitComponent>();
		m_Warminds = m_EntityManager->getComponentArray<WarmindComponent>();
		m_Characters = m_EntityManager->getComponentArray<CharacterComponent>();
		m_Transforms = m_EntityManager->getComponentArray<Transform>();

		m_WarManager = &WarManager::get();
	}

	virtual void update() override
	{
		for (auto entity : m_Entities)
		{
			if (!m_Warminds[entity].m_Active)
			{
				continue;
			}

			m_Warminds[entity].m_PrioritizedWarHandle = considerPrioritizedWar(entity);
			
			//if (m_Warminds[entity].m_PrioritizedWar->isWinning(entity, m_Warminds[entity].m_Opponent))
			//{
			//	//Send peace offer
			//}

			if (m_Units[m_Characters[entity].m_UnitEntity].m_Raised)
			{
				m_Warminds[entity].m_OrderAccu += Time::deltaTime();

				if (m_Warminds[entity].m_OrderAccu >= m_Warminds[entity].m_OrderTimer)
				{
					m_Warminds[entity].m_OrderAccu = 0.0f;
					considerOrders(entity, m_Units[m_Characters[entity].m_UnitEntity], m_Warminds[entity].m_Opponent);
				}
			}
		}
	}

	void orderSiegeCapital(EntityID warmindID, UnitComponent& unit)
	{
		Vector2D unitPosition = m_EntityManager->getComponent<Transform>(m_Characters[warmindID].m_UnitEntity).m_Position;
		Vector2DInt startingPosition = Map::convertToMap(unitPosition);

		Vector2DInt capitalPosition;
		War& currentWar = *m_WarManager->getWar(m_Warminds[warmindID].m_PrioritizedWarHandle);

		if (currentWar.isAttacker(warmindID))
		{
			capitalPosition = Map::get().getRegionCapitalLocation(m_Warminds[warmindID].m_WargoalRegionId);
		}

		else
		{
			CharacterComponent& enemyCharacter = currentWar.getAttacker();

			if (m_Units[m_Characters[warmindID].m_UnitEntity].m_DaysSeizing > 0)
			{
				return;
			}
			
			int randomRegionIndex = rand() % enemyCharacter.m_OwnedRegionIDs.size();
			capitalPosition = Map::get().getRegionCapitalLocation(enemyCharacter.m_OwnedRegionIDs[randomRegionIndex]);
		}

		moveUnit(unit.m_EntityID, capitalPosition);
	}

	void orderFlee(EntityID warmindID, UnitComponent&)
	{
		LOG_INFO("{0} is fleeing from the enemy army", m_Characters[warmindID].m_Name);
	}

	void orderFightEnemyArmy(EntityID warmindID, UnitComponent& unit)
	{
		if (m_Warminds[warmindID].m_PrioritizedWarHandle == -1)
		{
			return;
		}

		auto& enemyCharacter = m_Characters[m_Warminds[warmindID].m_Opponent];
		auto& enemyUnit = m_Units[enemyCharacter.m_UnitEntity];

		if (!enemyUnit.m_Raised)
		{
			return;
		}

		Vector2DInt battlefieldIntPosition;

		if (enemyUnit.m_CurrentPath.size() > 0)
		{
			LOG_INFO("{0} is chasing the enemy army", m_Characters[warmindID].m_Name);
			battlefieldIntPosition = enemyUnit.m_CurrentPath.back();
		}

		else
		{
			battlefieldIntPosition = Map::convertToMap(m_Transforms[enemyUnit.getID()].m_Position);
		}

		moveUnit(unit.m_EntityID, battlefieldIntPosition);
	}

	void considerOrders(EntityID warmind, UnitComponent& unit, EntityID target)
	{
		auto& enemyUnit = m_Units[m_Characters[target].m_UnitEntity];

		if (m_Warminds[warmind].m_PrioritizedWarHandle == -1)
		{
			return; 
		}

		if (m_WarManager->getWar(m_Warminds[warmind].m_PrioritizedWarHandle)->isDefender(warmind))
		{
			FightEnemyArmyConsideration fightConsideration;
			float fightEval = fightConsideration.evaluate(warmind, target);

			if (fightEval > 0.7)
			{
				orderFightEnemyArmy(warmind, unit);
				return;
			}

			Vector2D unitPosition = m_Transforms[unit.getID()].m_Position;
			Vector2D enemyUnitPosition = m_Transforms[enemyUnit.getID()].m_Position;

			float distance = (unitPosition - enemyUnitPosition).getLength();
			if (distance < 100.0f)
			{
				orderFlee(warmind, unit);
				return;
			}

			int regionID = m_WarManager->getWar(m_Warminds[warmind].m_PrioritizedWarHandle)->m_WargoalRegion;

			Vector2DInt regionPosition = Map::get().getRegionById(regionID).m_RegionCapital;
			moveUnit(unit.getID(), regionPosition);
			return;
		}

		else
		{
			FightEnemyArmyConsideration fightConsideration;
			float fightEval = fightConsideration.evaluate(warmind, target);

			if (fightEval > .7)
			{
				Vector2D unitPosition = m_Transforms[unit.getID()].m_Position;
				Vector2D enemyUnitPosition = m_Transforms[enemyUnit.getID()].m_Position;

				float distance = (unitPosition - enemyUnitPosition).getLength();
				if (distance < 100.0f)
				{
					//Hunt enemy army
					LOG_INFO("Warmind belonging to {0} decided to hunt the enemy army", m_Characters[warmind].m_Name);
					orderFightEnemyArmy(warmind, unit);
					return;
				}

				else
				{
					//Siege wargoal region
					LOG_INFO("Warmind belonging to {0} decided to siege the enemy capital", m_Characters[warmind].m_Name);
					orderSiegeCapital(warmind, unit);
					return;
				}
			}
		}
	}

	int considerPrioritizedWar(EntityID ent) //Will be expanded later
	{
		WarManager* warManager = &WarManager::get();

		if (!m_Characters[ent].m_CurrentWars.empty())
		{
			m_Warminds[ent].m_PrioritizedWarHandle = m_Characters[ent].m_CurrentWars.front();
			return m_Warminds[ent].m_PrioritizedWarHandle;
		}

		return -1;
	}

	void moveUnit(EntityID unitToMove, Vector2DInt targetPosition)
	{
		UnitComponent& unit = m_Units[unitToMove];
		Transform& transform = m_EntityManager->getComponent<Transform>(unitToMove);

		unit.m_Moving = false;
		transform.m_Position = unit.m_LastPosition;

		Vector2D unitPosition = transform.m_Position;
		Vector2DInt startingPosition = Map::convertToMap(unitPosition);
		std::vector<Vector2DInt> path = Pathfinding::get().findPath(startingPosition, targetPosition);

		if (path.size() > 0)
		{
			if (unit.m_CurrentPath.size() > 0)
			{
				if (path.back() == unit.m_CurrentPath.back())
				{
					unit.m_Moving = true;
					return;
				}
			}

			unit.setPath(path, Map::convertToScreen(startingPosition));
#pragma warning(push)
#pragma warning(disable: 26815)
			UnitSystem* unitSystem = (UnitSystem*)m_EntityManager->getSystem<UnitSystem>().get();
#pragma warning(pop)
			unitSystem->showPath(transform, unit);
		}
		else
		{
			unit.m_CurrentPath.clear();
		}
	}
};
