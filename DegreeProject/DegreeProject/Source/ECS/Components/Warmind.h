#pragma once
#include "../Component.h"
#include <ECS\Components\CharacterComponent.h>

struct WarmindComponent : Component
{
	int m_WargoalRegionId;

	//If the gold diff between opponent and this is smaller than this it can go to war
	int m_AcceptableGoldDiff = 200;
	int m_AcceptableArmyDiff = 50;

	CharacterComponent& Opponent;
};
