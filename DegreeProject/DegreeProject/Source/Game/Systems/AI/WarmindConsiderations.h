#pragma once
#include "AIConsideration.h"
#include <Game\Components\Warmind.h>
#include <Game\Components\Unit.h>
#include <ECS\Components\Transform.h>
#include "Game/Components/CharacterComponent.h"
#include "Engine/UnitManager.h"
#include "Game/Data/Unit.h"


struct SiegeCapitalConsideration : public Consideration
{
	SiegeCapitalConsideration() : Consideration()
	{
	}

	SiegeCapitalConsideration(CharacterID context) { m_Context = context; }

	void setContext(EntityID context) override
	{
		m_Context = context;
	}

	float evaluate(EntityID context, EntityID target)
	{
		//Judge distance to other army
		Vector2D contextPosition = UnitManager::get().getUnitOfCharacter(context).m_Position;
		Vector2D targetPosition = UnitManager::get().getUnitOfCharacter(target).m_Position;

		float distance = (contextPosition - targetPosition).getLength();
		distance = std::clamp(distance * 0.1f, 0.0f, 1.0f);

		return distance;
	}
};

struct FightEnemyArmyConsideration : public Consideration
{
	FightEnemyArmyConsideration() : Consideration()
	{
	}

	FightEnemyArmyConsideration(EntityID context) { m_Context = context; }

	void setContext(EntityID context) override
	{
		m_Context = context;
	}

	float evaluate(EntityID context, EntityID target) override
	{
		Vector2D contextPosition = UnitManager::get().getUnitOfCharacter(context).m_Position;
		Vector2D targetPosition = UnitManager::get().getUnitOfCharacter(context).m_Position;

		float distance = (contextPosition - targetPosition).getLength();
		distance = std::clamp(distance * 0.1f, 0.0f, 1.0f);

		return std::clamp(std::pow(distance, distance), 0.0f, 1.0f);
	}
};