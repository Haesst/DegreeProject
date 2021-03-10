#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Game/Components/Warmind.h"
#include "Game/Components/CharacterComponent.h"

struct WarmindSystem : System
{
	EntityManager* m_EntityManager = nullptr;

	bool m_Active = false;

	WarmindComponent* warminds = nullptr;
	CharacterComponent* characters = nullptr;

	WarmindSystem()
	{
		AddComponentSignature<WarmindComponent>();
		AddComponentSignature<CharacterComponent>();
		m_EntityManager = &EntityManager::Get();


		warminds = m_EntityManager->GetComponentArray<WarmindComponent>();
		characters = m_EntityManager->GetComponentArray<CharacterComponent>();
	}

	virtual void Update() override
	{
		if (!m_Active)
		{
			return;
		}


		for (auto entity : m_Entities)
		{
		}
	}
};
