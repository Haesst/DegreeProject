#pragma once
#include "ECS\Component.h"
#include "Game\Components\CharacterComponent.h"
#include <Game\Components\Unit.h>

struct WarmindComponent : Component
{
	int m_WargoalRegionId = 0;

	EntityID m_Opponent;

	bool m_Defending = false;
	bool m_Active = false;
	bool m_AtWar = false;
	bool m_RecentlyAtWar = false; //Flag for starting war

	EntityID m_UnitEntity;

	WarmindComponent()
	{
		m_Opponent = (size_t)INT_MAX;
	};

	WarmindComponent(int warGoalRegionId,  EntityID opponent)
	{
		m_WargoalRegionId = warGoalRegionId;
		m_Opponent = opponent;
	};
};
