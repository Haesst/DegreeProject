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
		for (auto entity : m_Entities)
		{
			auto Warmind = &m_EntityManager->GetComponent<WarmindComponent>(entity);
			auto Character = &m_EntityManager->GetComponent<CharacterComponent>(entity);

			if (IsAcceptableGoldDiff(Character->m_CurrentGold, Warmind->m_Opponent.m_CurrentGold, Warmind->m_AcceptableGoldDiff))
			{
				//We could go to war?
			}

			if (IsAcceptableArmySize(Character->m_CurrentArmySize, Warmind->m_Opponent.m_CurrentArmySize, Warmind->m_AcceptableArmyDiff))
			{
				//We could go to war?
			}
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
