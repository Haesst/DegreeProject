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
		addComponentSignature<CharacterComponent>();
		addComponentSignature<WarmindComponent>();

		m_EntityManager = &EntityManager::get();
		m_Characters = m_EntityManager->getComponentArray<CharacterComponent>();
		m_Warminds = m_EntityManager->getComponentArray<WarmindComponent>();
	}

	void init(AIManager* manager)
	{
		m_AIManager = manager;

		for (auto entity : m_Entities)
		{
			auto hello = std::make_pair(entity, m_Warminds[entity]);
			m_EntityToWarminds.push_back(hello);
		}
	}

	virtual void update() override
	{
		m_Characters = m_EntityManager->getComponentArray<CharacterComponent>();
		m_Warminds = m_EntityManager->getComponentArray<WarmindComponent>();
		m_TickAccu += Time::deltaTime();

		if (m_TickAccu <= m_AIUpdateTickRate)
		{
			return;
		}

		//AI System update considerations
		for (auto entity : m_Entities)
		{
			if (expansionDecision(entity) > .2f) //Add personality weight
			{
				if (warDecision(entity) > .2f)
				{
					if (!m_Characters[entity].m_AtWar)
					{
						m_Characters[entity].declareWar(m_Warminds[entity].m_Opponent, m_Warminds[entity].m_WargoalRegionId);
					}
				}
			}

			m_TickAccu = 0.0f;
		}
	}

	float warDecision(EntityID ent);

	float expansionDecision(EntityID ent);
};
