#pragma once
#include "ECS\Component.h"
#include "Game\Components\CharacterComponent.h"
#include <Game\Components\Unit.h>

struct WarmindComponent : Component
{
	int m_WargoalRegionId = 0;

	EntityID m_Opponent;

	bool m_Defending = false;

	std::vector<UnitComponent> m_Units;

	WarmindComponent()
	{
		m_Opponent = (size_t)INT_MAX;
	};

	WarmindComponent(int warGoalRegionId,  EntityID opponent, std::vector<UnitComponent> currentUnits = std::vector<UnitComponent>())
	{
		m_WargoalRegionId = warGoalRegionId;
		m_Opponent = opponent;
		m_Units = currentUnits;
	};
};
