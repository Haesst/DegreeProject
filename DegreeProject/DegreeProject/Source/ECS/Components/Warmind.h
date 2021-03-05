#pragma once
#include "../Component.h"
#include <ECS\Components\CharacterComponent.h>

struct WarmindComponent : Component
{
	int m_WargoalRegionId = 0;

	EntityID m_Opponent;

	WarmindComponent()
	{

	};

	WarmindComponent(int warGoalRegionId,  EntityID opponent)
	{
		m_WargoalRegionId = warGoalRegionId;
		m_Opponent = opponent;
	};
};
