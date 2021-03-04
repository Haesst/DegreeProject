#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include <ECS\Components\Warmind.h>

struct WarmindSystem : System
{
	EntityManager* m_EntityManager = nullptr;


	WarmindSystem()
	{
		AddComponentSignature<WarmindComponent>();
		AddComponentSignature<CharacterComponent>();
		m_EntityManager = &EntityManager::Get();
	}

	virtual void Update() override
	{
		WarmindComponent* warminds = m_EntityManager->GetComponentArray<WarmindComponent>();
		CharacterComponent* characters = m_EntityManager->GetComponentArray<CharacterComponent>();

		for (auto entity : m_Entities)
		{
		}
	}


	bool IsAcceptableArmySize(int currentArmySize, int opponentArmySize, int maxDiff)
	{
		if (currentArmySize > opponentArmySize)
		{
			return true;
		}

		else if (opponentArmySize - currentArmySize <= maxDiff)
		{
			return true;
		}

		return false;
	}

	bool IsAcceptableGoldDiff(int currentGold, int opponentGold, int maxGoldDiff)
	{
		if (currentGold > opponentGold)
		{
			return true;
		}

		else if (opponentGold - currentGold <= maxGoldDiff)
		{
			return true;
		}

		return false;
	}
};
