#pragma once

#include <Game\War.h>

struct WarmindComponent
{
	int m_WargoalRegionId = 0;

	CharacterID m_Opponent;

	bool m_Active = false;
	bool m_AtWar = false;
	bool m_RecentlyAtWar = false; //Flag for starting war

	float m_OrderAccu = 0.0f;
	float m_OrderTimer = 3.0f;  //timer which the AI will evaluate new orders

	float m_AtWarTickRate = 2.0f;
	float m_TickAccu = 0.0f;

	int m_PrioritizedWarHandle = -1;

	CharacterID m_OwnerID = INVALID_CHARACTER_ID;

	WarmindComponent()
	{
		m_Opponent = (size_t)INT_MAX;
	};

	WarmindComponent(int warGoalRegionId, CharacterID opponent)
	{
		m_WargoalRegionId = warGoalRegionId;
		m_Opponent = opponent;
	};
};
