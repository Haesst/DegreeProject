#include "War.h"

War::War(CharacterID attacker, CharacterID defender, int warGoalRegion, int handle)
{
	m_Attackers.push_back(attacker);
	m_Defenders.push_back(defender);
	m_WargoalRegion = warGoalRegion;
	m_Handle = handle;
	m_StartDate = Time::m_GameDate.m_Date;
}

int War::getHandle()
{
 	return m_Handle;
}

Date War::getStartDate()
{
	return m_StartDate;
}
