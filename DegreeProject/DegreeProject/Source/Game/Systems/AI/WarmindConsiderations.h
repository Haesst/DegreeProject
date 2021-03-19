#pragma once
#include "AIConsideration.h"
#include <Game\Components\Warmind.h>
#include <Game\Components\Unit.h>
#include <ECS\Components\Transform.h>


struct SiegeCapitalConsideration : public Consideration
{
	EntityManager* m_EntityManager = nullptr;

	WarmindComponent* m_Warminds = nullptr;
	UnitComponent* m_Units = nullptr;
	Transform* m_Transforms = nullptr;

	SiegeCapitalConsideration() : Consideration()
	{
		m_EntityManager = &EntityManager::Get();
		m_Warminds = m_EntityManager->GetComponentArray<WarmindComponent>();
		m_Units = m_EntityManager->GetComponentArray<UnitComponent>();
		m_Transforms = m_EntityManager->GetComponentArray<Transform>();
	}

	SiegeCapitalConsideration(EntityID context) { m_Context = context; }

	void SetContext(EntityID context) override
	{
		m_Context = context;
	}

	float Evaluate(EntityID context, EntityID target)
	{
		//Judge distance to other army
		Vector2D contextPosition = m_Transforms[m_Warminds[context].m_UnitEntity].m_Position;
		Vector2D targetPosition = m_Transforms[m_Warminds[target].m_UnitEntity].m_Position;

		float distance = (contextPosition - targetPosition).GetLength();
		distance = std::clamp(distance * 0.1f, 0.0f, 1.0f);

		return distance;
	}
};

struct FightEnemyArmyConsideration : public Consideration
{
	EntityManager* m_EntityManager = nullptr;

	WarmindComponent* m_Warminds = nullptr;
	UnitComponent* m_Units = nullptr;
	Transform* m_Transforms = nullptr;

	FightEnemyArmyConsideration() : Consideration()
	{
		m_EntityManager = &EntityManager::Get();
		m_Warminds = m_EntityManager->GetComponentArray<WarmindComponent>();
		m_Units = m_EntityManager->GetComponentArray<UnitComponent>();
		m_Transforms = m_EntityManager->GetComponentArray<Transform>();
	}

	FightEnemyArmyConsideration(EntityID context) { m_Context = context; }

	void SetContext(EntityID context) override
	{
		m_Context = context;
	}

	float Evaluate(EntityID context, EntityID target) override
	{
		Vector2D contextPosition = m_Transforms[m_Warminds[context].m_UnitEntity].m_Position;
		Vector2D targetPosition = m_Transforms[m_Warminds[target].m_UnitEntity].m_Position;

		float distance = (contextPosition - targetPosition).GetLength();
		distance = std::clamp(distance * 0.1f, 0.0f, 1.0f);

		return std::clamp(std::pow(distance, distance), 0.0f, 1.0f);
	}
};