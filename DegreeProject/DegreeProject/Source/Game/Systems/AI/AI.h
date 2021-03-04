#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Time.h"
#include "Engine/Window.h"
#include <ECS\Components\CharacterComponent.h>
#include <Game\Systems\AI\AIConsideration.h>
#include <ECS\Components\Warmind.h>

struct AISystem : System
{
	EntityManager* m_EntityManager = nullptr;

	float m_TickAccu = 0.0f;

	float m_AIUpdateTickRate = 2.0f;

	AISystem()
	{
		AddComponentSignature<CharacterComponent>();
		AddComponentSignature<WarmindComponent>();
		m_EntityManager = &EntityManager::Get();
	}

	virtual void Update() override
	{
		m_TickAccu += Time::DeltaTime();
		//Every tick (Light updates)
		for (auto entity : m_Entities)
		{

		}

		if (m_TickAccu <= m_AIUpdateTickRate)
		{
			return;
		}

		//AI System update considerations
		for (auto entity : m_Entities)
		{
						
		}

	}

	float WarDecision(EntityID ent)
	{
		auto CharacterComp = m_EntityManager->GetComponent<CharacterComponent>(ent);
		GoldConsideration goldConsideration;
		ArmyConsideration armyConsideration;

		goldConsideration.SetContext(CharacterComp);
		armyConsideration.SetContext(CharacterComp);

		float goldEvaluation = goldConsideration.Evaluate();
		float armyEvaluation = armyConsideration.Evaluate();

		float actionScore = goldEvaluation * armyEvaluation;
		return std::clamp(actionScore, 0.0f, 1.0f);
	}
};
