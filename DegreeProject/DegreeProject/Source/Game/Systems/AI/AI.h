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
	EntityManager* m_EntityManager = nullptr;
	AIManager* m_AIManager = nullptr;

	float m_TickAccu = 0.0f;

	float m_AIUpdateTickRate = 10.0f;

	AISystem()
	{
		AddComponentSignature<CharacterComponent>();
		AddComponentSignature<WarmindComponent>();
		m_EntityManager = &EntityManager::Get();
	}

	void Init(AIManager* manager)
	{
		m_AIManager = manager;
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
			WarDecision(entity);
		}
	}

	float WarDecision(EntityID ent);
	float ExpansionDecision(EntityID ent);
};
