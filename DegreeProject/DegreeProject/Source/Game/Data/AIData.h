#pragma once
#include <Game\Data\Character.h>

enum class Action
{
	War,
	Upgrade_Settlement,
	Marriage,
	Seek_Alliance,
	NONE
};

struct AIData
{
	Action m_CurrentAction = Action::NONE;
	CharacterID m_OwnerID = INVALID_CHARACTER_ID;
	Action m_LastAction = Action::NONE;

	std::vector<std::pair<float, Action>> m_Evaluations;

	int m_TickPrio = -1;
	int m_SettlementToUpgrade = -1;
	CharacterID m_PotentialSpouseID = INVALID_CHARACTER_ID;
	CharacterID m_PotentialAllyID = INVALID_CHARACTER_ID;
};