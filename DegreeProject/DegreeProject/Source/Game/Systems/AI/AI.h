#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Time.h"
#include "Engine/Window.h"

#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

#include <ECS\Components\CharacterComponent.h>
#include <Game\Systems\AI\AIConsideration.h>
#include <ECS\Components\Warmind.h>

struct Personality
{
	const char* m_PersonalityName;
	float m_DeclareWarModifier = 0.0f;

	Personality(const char* personalityName, float declareWarModifier)
	{
		m_PersonalityName = personalityName;
		m_DeclareWarModifier = declareWarModifier;
	}
};


struct AISystem : System
{
	EntityManager* m_EntityManager = nullptr;

	float m_TickAccu = 0.0f;

	float m_AIUpdateTickRate = 10.0f;

	std::vector<Personality> traits;

	AISystem()
	{
		AddComponentSignature<CharacterComponent>();
		AddComponentSignature<WarmindComponent>();
		m_EntityManager = &EntityManager::Get();
	}

	void LoadPersonalities()
	{
		std::ifstream file("Assets/Data/AI/AIPersonalities.json");
		json j;
		file >> j;

		traits.clear();

		for (auto& trait : j)
		{
			const char* personalityName = trait["debugName"].get<std::string>().c_str();
			float declareWarModifier = trait["declareWarMod"].get<float>();
			
			Personality personality(personalityName, declareWarModifier);
			traits.push_back(personality);
		}
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
};
