#pragma once
#include <Game\Components\Warmind.h>

struct Unit;

struct WarOrders
{
	//Ally orders
	void orderFollowMainAlly(WarmindComponent& warmind, Unit& unit, CharacterID mainAttacker);

	//Offensive orders
	void orderFightEnemyArmy(WarmindComponent& warmind, Unit& unit);
	void orderSiegeCapital(WarmindComponent& warmind, Unit& unit);

	//Defensive orders
	void orderDefendWargoal(WarmindComponent& warmind, Unit& unit, Unit& enemyUnit);
	void orderAttackArmy(Unit& unit, Unit& enemyUnit);
	void orderAttackEnemyRegion(Unit& unit, Unit& enemyUnit);
};
