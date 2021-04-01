#include "WarOrders.h"
#include "Game/WarManager.h"
#include "Game/Data/Unit.h"
#include "Engine/UnitManager.h"
#include "Game/Pathfinding.h"

void WarOrders::orderFightEnemyArmy(WarmindComponent& warmind, Unit& unit)
{
	if (warmind.m_PrioritizedWarHandle == -1)
	{
		return;
	}

	auto& enemyUnit = UnitManager::get().getUnitOfCharacter(CharacterManager::get()->getCharacter(warmind.m_Opponent).m_CharacterID);

	if (!enemyUnit.m_Raised)
	{
		return;
	}

	Vector2DInt battlefieldIntPosition;

	if (enemyUnit.m_CurrentPath.size() > 0)
	{
		battlefieldIntPosition = enemyUnit.m_CurrentPath.back();
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
	WarManager* warManager = &WarManager::get();

	War& currentWar = *warManager->getWar(warmind.m_PrioritizedWarHandle);

	if (currentWar.isAttacker(warmind.m_OwnerID))
	{
		capitalPosition = Map::get().getRegionCapitalLocation(warmind.m_WargoalRegionId);
	}

	else
	{
		CharacterID enemyCharacter = currentWar.getAttacker();

		if (unit.m_DaysSeizing > 0)
		{
			return;
		}

		int randomRegionIndex = rand() % CharacterManager::get()->getCharacter(enemyCharacter).m_OwnedRegionIDs.size();
		capitalPosition = Map::get().getRegionCapitalLocation(CharacterManager::get()->getCharacter(enemyCharacter).m_OwnedRegionIDs[randomRegionIndex]);
	}

	UnitManager::get().giveUnitPath(unit.m_UnitID, Pathfinding::get().findPath(startingPosition, capitalPosition));
}

void WarOrders::orderFlee(WarmindComponent&, Unit&)
{
}
