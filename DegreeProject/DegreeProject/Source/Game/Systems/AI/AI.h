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
#include "Game/WarManager.h"
#include "Game/AI/AIManager.h"

class CharacterSystem;

struct AISystem : System
{
	std::vector<std::pair<EntityID, WarmindComponent>> m_EntityToWarminds;

	EntityManager* m_EntityManager = nullptr;
	AIManager* m_AIManager = nullptr;

	float m_TickAccu = 0.0f;

	float m_AIUpdateTickRate = 10.0f;

	CharacterComponent* m_Characters = nullptr;
	WarmindComponent* m_Warminds = nullptr;
	WarManager* m_WarManager = nullptr;

	AISystem()
	{
		addComponentSignature<CharacterComponent>();
		addComponentSignature<WarmindComponent>();

		m_EntityManager = &EntityManager::get();
		m_Characters = m_EntityManager->getComponentArray<CharacterComponent>();
		m_WarManager = &WarManager::get();
		m_Warminds = m_EntityManager->getComponentArray<WarmindComponent>();
	}

	void init(AIManager* manager)
	{
		m_AIManager = manager;

		for (auto entity : m_Entities)
		{
			auto pair = std::make_pair(entity, m_Warminds[entity]);
			m_EntityToWarminds.push_back(pair);
		}
	}

	virtual void update() override;

	float warDecision(EntityID ent);

	float expansionDecision(EntityID ent);
};
