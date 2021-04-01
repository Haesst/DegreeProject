#include "WarOrders.h"
#include "Game/WarManager.h"
#include "Game/Data/Unit.h"

void WarOrders::orderFightEnemyArmy(WarmindComponent& warmind, Unit& unit)
{
	if (warmind.m_PrioritizedWarHandle == -1)
	{
		return;
	}

	auto& enemyCharacter = CharacterManager::get()->getCharacter(warmind.m_Opponent);
	auto& enemyUnit = UnitComponent();

	if (!enemyUnit.m_Raised)
	{
		return;
	}

	Vector2DInt battlefieldIntPosition;

	if (enemyUnit.m_CurrentPath.size() > 0)
	{
		//LOG_INFO("{0} is chasing the enemy army", m_Characters[warmind].m_Name);
		battlefieldIntPosition = enemyUnit.m_CurrentPath.back();
	}

	else
	{
		battlefieldIntPosition = Map::convertToMap(enemyUnit.m_LastPosition); //Todo: This needs to change when transforms are changed!!!!!!
	}

	//Order unit to move
}

void WarOrders::orderSiegeCapital(WarmindComponent& warmind, Unit& unit)
{
	Vector2D unitPosition = unit.m_Position; //NEEDS TO CHANGE
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

	//Order unit to move
}

void WarOrders::orderFlee(WarmindComponent& warmind, Unit& unit)
{
}
