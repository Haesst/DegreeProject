#pragma once
#include "ECS/Components/CharacterComponent.h"

struct Consideration
{
	Consideration() = default;

	Consideration(const CharacterComponent& context)
	{
		m_Context = context;
	}

	CharacterComponent& m_Context;

	float Evaluate(const CharacterComponent& context)
	{
		return 0.0f;
	}
};

struct WarConsideration : public Consideration
{
	WarConsideration(const CharacterComponent& context)
	{
		m_Context = context;
	}

	float Evaluate(const CharacterComponent& context)
	{
		if (context.m_CurrentGold <= 0 || context.m_CurrentArmySize == 0)
		{
			return 0.0;
		}
		
		else
		{
			return 1.0;
		}
	}
};
