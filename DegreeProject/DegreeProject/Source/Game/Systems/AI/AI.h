#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Time.h"
#include "Engine/Window.h"

#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

#include <Game\Components\CharacterComponent.h>
#include <Game\Systems\AI\AIConsideration.h>
#include <Game\Components\Warmind.h>
#include "Game/AI/AIManager.h"

struct AISystem : System
{
	std::vector<std::pair<EntityID, WarmindComponent>> m_EntityToWarminds;

	EntityManager* m_EntityManager = nullptr;
	AIManager* m_AIManager = nullptr;

	float m_TickAccu = 0.0f;

	float m_AIUpdateTickRate = 10.0f;

	CharacterComponent* m_Characters;
	WarmindComponent* m_Warminds;

	AISystem()
	{
		AddComponentSignature<CharacterComponent>();
		AddComponentSignature<WarmindComponent>();

		m_EntityManager = &EntityManager::Get();
		m_Characters = m_EntityManager->GetComponentArray<CharacterComponent>();
		m_Warminds = m_EntityManager->GetComponentArray<WarmindComponent>();
	}

	void Init(AIManager* manager)
	{
		m_AIManager = manager;

		for (auto entity : m_Entities)
		{
			auto hello = std::make_pair(entity, m_Warminds[entity]);
			m_EntityToWarminds.push_back(hello);
		}
	}

	virtual void Update() override
	{
		m_TickAccu += Time::DeltaTime();

		if (m_TickAccu <= m_AIUpdateTickRate)
		{
			return;
		}

		//AI System update considerations
		for (auto entity : m_Entities)
		{
			if (ExpansionDecision(entity) > .1f) //Add personality weight
			{
				if (WarDecision(entity) > .1f)
				{
					if (!m_Characters[entity].m_AtWar)
					{
						DeclareWar(entity, m_Warminds[entity].m_Opponent, m_Warminds[entity].m_WargoalRegionId);
					}
				}
			}

			m_TickAccu = 0.0f;
		}
	}

	float WarDecision(EntityID ent);

	float ExpansionDecision(EntityID ent);

	void DeclareWar(EntityID instigator, EntityID target, int warGoalRegion);
};
