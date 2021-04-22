#include "WarOrders.h"
#include "Game/WarManager.h"
#include "Game/Data/Unit.h"
#include "Game/Systems/UnitManager.h"
#include "Game/Pathfinding.h"

void WarOrders::orderFightEnemyArmy(WarmindComponent& warmind, Unit& unit)
{
	WarManager& warManager = WarManager::get();

	if (warmind.m_PrioritizedWarHandle == -1)
	{
		return;
	}

	auto& enemyUnit = UnitManager::get().getUnitOfCharacter(CharacterManager::get().getCharacter(warmind.m_Opponent).m_CharacterID);

	Vector2DInt battlefieldIntPosition;

	if (enemyUnit.m_CurrentPath.size() > 0)
	{
		battlefieldIntPosition = enemyUnit.m_CurrentPath.back();
	}

	else if(!enemyUnit.m_Raised)
	{
		War* war = WarManager::get().getWar(warmind.m_PrioritizedWarHandle);
		
		if(Map::get().getRegionById(war->m_WargoalRegion).m_OccupiedBy == INVALID_CHARACTER_ID)
		{
			battlefieldIntPosition = Map::get().getRegionById(war->m_WargoalRegion).m_RegionCapital;
		}

		else
		{
			if (CharacterManager::get().getCharacter(warManager.getOpposingForce(war->getHandle(), warmind.m_OwnerID)).m_OwnedRegionIDs.size() > 0)
			{
				int randomRegion = rand() % CharacterManager::get().getCharacter(warManager.getOpposingForce(war->getHandle(), warmind.m_OwnerID)).m_OwnedRegionIDs.size();
				battlefieldIntPosition = Map::get().getRegionById(randomRegion).m_RegionCapital;
			}
		}
	}

	else
	{
		battlefieldIntPosition = Map::convertToMap(enemyUnit.m_LastPosition);
	}

	Vector2DInt unitPos{ (int)unit.m_Position.x, (int)unit.m_Position.y };
	UnitManager::get().giveUnitPath(unit.m_UnitID, Pathfinding::get().findPath(unitPos, battlefieldIntPosition));
}

void WarOrders::orderSiegeCapital(WarmindComponent& warmind, Unit& unit)
{
	Vector2D unitPosition = unit.m_Position;
	Vector2DInt startingPosition = Map::convertToMap(unitPosition);

	Vector2DInt capitalPosition;
	WarManager& warManager = WarManager::get();

	War* currentWar = warManager.getWar(warmind.m_PrioritizedWarHandle);

	if (warManager.isAttacker(currentWar->getHandle(), warmind.m_OwnerID))
	{
		capitalPosition = Map::get().getRegionCapitalLocation(warmind.m_WargoalRegionId);
	}

	else
	{
		CharacterID enemyCharacter = warManager.getAttacker(currentWar->getHandle());

		if (unit.m_DaysSeizing > 0)
		{
			return;
		}

		int randomRegionIndex = rand() % CharacterManager::get().getCharacter(enemyCharacter).m_OwnedRegionIDs.size();
		capitalPosition = Map::get().getRegionCapitalLocation(CharacterManager::get().getCharacter(enemyCharacter).m_OwnedRegionIDs[randomRegionIndex]);
	}

	UnitManager::get().giveUnitPath(unit.m_UnitID, Pathfinding::get().findPath(startingPosition, capitalPosition));
}

void WarOrders::orderDefendWargoal(WarmindComponent& warmind, Unit& unit, Unit& enemyUnit)
{
	Vector2D unitPosition = unit.m_Position;
	Vector2D enemyUnitPosition = enemyUnit.m_Position;

	int regionID = WarManager::get().getWar(warmind.m_PrioritizedWarHandle)->m_WargoalRegion;
	Vector2DInt regionPosition = Map::get().getRegionById(regionID).m_RegionCapital;

	//Order unit to move
	UnitManager::get().giveUnitPath(UnitManager::get().getUnitOfCharacter(warmind.m_OwnerID).m_UnitID, Pathfinding::get().findPath(Map::get().convertToMap(unitPosition), regionPosition));
}

void WarOrders::orderAttackArmy(Unit& unit, Unit& enemyUnit)
{
	Vector2DInt enemyPosition;

	if (enemyUnit.m_CurrentPath.size() > 0)
	{
		enemyPosition = enemyUnit.m_CurrentPath.front();
	}

	else
	{
		enemyPosition = Map::get().convertToMap(enemyUnit.m_Position);
	}

	UnitManager::get().giveUnitPath(unit.m_UnitID, Pathfinding::get().findPath(Map::get().convertToMap(unit.m_Position), enemyPosition));

}

void WarOrders::orderAttackEnemyRegion(Unit& unit, Unit& enemyUnit)
{
	CharacterManager& characterManager = CharacterManager::get();
	Character& enemyCharacter = characterManager.getCharacter(enemyUnit.m_Owner);
	Character& character = characterManager.getCharacter(unit.m_Owner);

	Vector2DInt bestRegion;
	float shortestDistance = FLT_MAX;

	for (auto region : enemyCharacter.m_OwnedRegionIDs)
	{
		Vector2DInt regionPos = Map::get().getRegionById(region).m_RegionCapital;

		for (auto ownedRegion : character.m_OwnedRegionIDs)
		{
			float dist = (regionPos - Map::get().getRegionById(ownedRegion).m_RegionCapital).getLength();

			if (dist < shortestDistance)
			{
				if (Map::get().getRegionById(ownedRegion).m_OccupiedBy != INVALID_CHARACTER_ID)
				{
					continue;
				}

				bestRegion = regionPos;
				shortestDistance = dist;
			}
		}
	}

	ASSERT(shortestDistance != FLT_MAX, "This should not happen :(");
	
	UnitManager::get().giveUnitPath(unit.m_UnitID, Pathfinding::get().findPath(Map::get().convertToMap(unit.m_Position), bestRegion));
}
