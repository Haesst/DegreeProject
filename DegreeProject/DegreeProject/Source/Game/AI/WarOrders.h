#pragma once
#include <Game\Components\Warmind.h>

struct Unit;

struct WarOrders
{
	void orderFightEnemyArmy(WarmindComponent& warmind, Unit& unit);
	void orderSiegeCapital(WarmindComponent& warmind, Unit& unit);
	void orderFlee(WarmindComponent& warmind, Unit& unit);
};
