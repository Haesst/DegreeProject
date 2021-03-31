#pragma once

#include <vector>

#include "Engine/Vector2D.h"
#include "Game/Data/Types.h"

class RectangleShape;

struct Unit
{
	UnitID m_UnitID = INVALID_UNIT_ID;
	CharacterID m_Owner = INVALID_CHARACTER_ID;
	bool m_Raised = false;
	int m_RepresentedForce = 0;

	bool m_Selected = false;
	sf::Sprite m_Sprite;

	sf::RectangleShape m_HighlightShape;
	sf::Color m_FillColor = sf::Color::Transparent;
	sf::Color m_OutlineColor = sf::Color::White;
	sf::Vector2<float> m_HighlightShapeSize = sf::Vector2(32.0f, 32.0f);

	sf::RectangleShape m_InnerSeizeMeter;
	sf::RectangleShape m_OuterSeizeMeter;

	Vector2D m_Position;
	Vector2D m_LastPosition;
	bool m_Moving = false;
	std::vector<Vector2DInt> m_CurrentPath = std::vector<Vector2DInt>();
	Vector2D m_Target;
	Vector2D m_Direction;
	float m_Speed = 150.0f;

	int m_SeizingRegionID = -1;
	Date m_LastSeizeDate = Date(0, 0, 0);
	int m_DaysSeizing = 0;

	bool m_InCombat = false;
	float m_CombatTimer = 3.0f;
	float m_CombatTimerAccu = 0.0f;
};