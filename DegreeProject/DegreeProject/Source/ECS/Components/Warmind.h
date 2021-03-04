#pragma once
#include "../Component.h"
#include <ECS\Components\CharacterComponent.h>

struct WarmindComponent : Component
{
	int m_WargoalRegionId = 0;

	CharacterComponent* m_Opponent;

	WarmindComponent(int warGoalRegionId, CharacterComponent opponent)
	{
		m_WargoalRegionId = warGoalRegionId;
		m_Opponent = &opponent;
	};
};
