#pragma once
#include "AIConsideration.h"
#include <Game\Components\Warmind.h>
#include <Game\Components\Unit.h>
#include <ECS\Components\Transform.h>
#include "Game/Components/CharacterComponent.h"


struct SiegeCapitalConsideration : public Consideration
{
	EntityManager* m_EntityManager = nullptr;

	WarmindComponent* m_Warminds = nullptr;
	UnitComponent* m_Units = nullptr;
	Transform* m_Transforms = nullptr;
	CharacterComponent* m_Characters = nullptr;

	SiegeCapitalConsideration() : Consideration()
	{
		m_EntityManager = &EntityManager::get();
		m_Units = m_EntityManager->getComponentArray<UnitComponent>();
		m_Transforms = m_EntityManager->getComponentArray<Transform>();
		m_Characters = m_EntityManager->getComponentArray<CharacterComponent>();
	}

	SiegeCapitalConsideration(EntityID context) { m_Context = context; }

	void setContext(EntityID context) override
	{
		m_Context = context;
	}

	float evaluate(EntityID context, EntityID target)
	{
		//Judge distance to other army
		Vector2D contextPosition = m_Transforms[m_Characters[context].m_UnitEntity].m_Position;
		Vector2D targetPosition = m_Transforms[m_Characters[target].m_UnitEntity].m_Position;

		float distance = (contextPosition - targetPosition).getLength();
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
	CharacterComponent* m_Characters = nullptr;

	FightEnemyArmyConsideration() : Consideration()
	{
		m_EntityManager = &EntityManager::get();
		m_Warminds = nullptr; // m_EntityManager->getComponentArray<WarmindComponent>();
		m_Units = m_EntityManager->getComponentArray<UnitComponent>();
		m_Transforms = m_EntityManager->getComponentArray<Transform>();
		m_Characters = m_EntityManager->getComponentArray<CharacterComponent>();
	}

	FightEnemyArmyConsideration(EntityID context) { m_Context = context; }

	void setContext(EntityID context) override
	{
		m_Context = context;
	}

	float evaluate(EntityID context, EntityID target) override
	{
		Vector2D contextPosition = m_Transforms[m_Characters[context].m_UnitEntity].m_Position;
		Vector2D targetPosition = m_Transforms[m_Characters[target].m_UnitEntity].m_Position;

		float distance = (contextPosition - targetPosition).getLength();
		distance = std::clamp(distance * 0.1f, 0.0f, 1.0f);

		return std::clamp(std::pow(distance, distance), 0.0f, 1.0f);
	}
};