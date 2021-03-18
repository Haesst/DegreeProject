#pragma once
#include "AIConsideration.h"
#include <Game\Components\Warmind.h>
#include <Game\Components\Unit.h>
#include <ECS\Components\Transform.h>

struct SiegeCapitalConsideration : public Consideration
{
	WarmindComponent* m_Warminds = nullptr;
	UnitComponent* m_Units = nullptr;
	Transform* m_Transforms = nullptr;

	SiegeCapitalConsideration() : Consideration()
	{
	}

	SiegeCapitalConsideration(EntityID context) { m_Context = context; }

	void SetContext(EntityID context) override
	{
		m_Context = context;
	}

	float Evaluate(EntityID context, EntityID target)
	{
		////Judge distance to other army
		//Vector2D contextPosition = m_Transforms[context].m_Position;
		//Vector2D targetPosition = m_Transforms[target].m_Position;
		//
		//auto distance = (contextPosition - targetPosition).GetLength();
		//distance = std::clamp(distance * 0.1f, 0.0f, 1.0f);
		//
		//return distance;

		return .5f;
	}
};