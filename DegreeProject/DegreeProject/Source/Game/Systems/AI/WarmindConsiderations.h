#pragma once
#include "AIConsideration.h"
#include <Game\Components\Warmind.h>
#include <Game\Components\Unit.h>
#include <ECS\Components\Transform.h>

EntityManager* m_EntityManager = &EntityManager::Get();

WarmindComponent* m_Warminds = m_EntityManager->GetComponentArray<WarmindComponent>();
UnitComponent* m_Units = m_EntityManager->GetComponentArray<UnitComponent>();
Transform* m_Transforms = m_EntityManager->GetComponentArray<Transform>();

struct SiegeCapitalConsideration : public Consideration
{
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
		//Judge distance to other army
		Vector2D contextPosition = m_Units[m_Warminds[context].m_UnitEntity].m_Target; 
		Vector2D targetPosition = m_Units[m_Warminds[target].m_UnitEntity].m_Target;

		float distance = (contextPosition - targetPosition).GetLength();
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

	void SetContext(EntityID context) override
	{
		m_Context = context;
	}

	float Evaluate(EntityID context, EntityID target) override
	{
		Vector2D contextPosition = m_Units[m_Warminds[context].m_UnitEntity].m_Target;
		Vector2D targetPosition = m_Units[m_Warminds[target].m_UnitEntity].m_Target;

		//y = a ^ x
		float distance = (contextPosition - targetPosition).GetLength();
		distance = std::clamp(distance * 0.1f, 0.0f, 1.0f);

		return std::clamp(std::pow(distance, distance), 0.0f, 1.0f);
	}
};