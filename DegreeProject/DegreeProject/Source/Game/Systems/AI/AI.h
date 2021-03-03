#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Time.h"
#include "Engine/Window.h"
#include <ECS\Components\CharacterComponent.h>
#include <Game\Systems\AI\AIConsideration.h>

struct AISystem : System
{
	EntityManager* m_EntityManager = nullptr;

	std::vector<Consideration> m_Considerations;

	AISystem()
	{
		AddComponentSignature<CharacterComponent>();
		m_EntityManager = &EntityManager::Get();
	}

	virtual void Update() override
	{
		for (auto entity : m_Entities)
		{
			CharacterComponent* character = &m_EntityManager->GetComponent<CharacterComponent>(entity);
			if (!character->m_IsPlayerControlled)
			{
				WarConsideration war{ character };

				war.Evaluate(&character);
			}
		}
	}
};
