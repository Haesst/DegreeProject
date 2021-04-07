#pragma once
#include <Game\Data\Character.h>

enum class Action
{
	War,
	Upgrade_Settlement,
	NONE
};

struct AIData
{
	Action m_CurrentAction = Action::NONE;
	CharacterID m_OwnerID = INVALID_CHARACTER_ID;
	Action m_LastAction = Action::NONE;

	float m_ConsiderationAccu = 0.0f;
	float m_ConsiderationTimer = 10.0f;
};