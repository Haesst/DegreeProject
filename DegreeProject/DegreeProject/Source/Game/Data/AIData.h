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
	Action m_CurrentAction;
	CharacterID m_OwnerID;
	Action m_LastAction;

	float m_ConsiderationAccu = 0.0f;
	float m_ConsiderationTimer = 10.0f;
};