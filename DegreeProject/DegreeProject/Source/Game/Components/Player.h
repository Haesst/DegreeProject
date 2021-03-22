#pragma once

#include "ECS/Component.h"

struct Player : public Component
{
	int m_HighlightedRegion = -1;
	unsigned int m_SelectedUnitID = 0;
	unsigned int m_OwnedCharacter = 18;

	Player()
	{
		m_SelectedUnitID = 0;
	}

	Player(EntityID owner)
	{
		m_SelectedUnitID = 0;
		m_OwnedCharacter = owner;
	}
};