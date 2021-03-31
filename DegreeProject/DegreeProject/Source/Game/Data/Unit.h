#pragma once

#include <vector>

#include "Engine/Vector2D.h"
#include "Game/Data/Types.h"

class RectangleShape;

struct Unit
{
	CharacterID m_Owner = INVALID_CHARACTER_ID;
	bool m_Raised = false;
	int m_RepresentedForce = 0;

	Vector2D m_Position;
	bool m_Moving = false;
	std::vector<Vector2DInt> m_CurrentPath = std::vector<Vector2DInt>();
	Vector2D m_Target;
	Vector2D m_Direction;

	int m_SeizingRegionID = -1;
	int m_DaysSeizing = 0;

	bool m_InCombat = false;
	float m_CombatTimer = 3.0f;
	float m_CombatTimerAccu = 0.0f;
};