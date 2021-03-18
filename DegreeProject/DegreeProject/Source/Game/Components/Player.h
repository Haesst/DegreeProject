#pragma once

#include "ECS/Component.h"

struct Player : public Component
{
	int m_HighlightedRegion = -1;
};