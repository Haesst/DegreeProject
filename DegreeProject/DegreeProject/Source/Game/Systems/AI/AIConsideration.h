#pragma once
#include "Game/Components/CharacterComponent.h"
#include <algorithm>
#include "Game/MapInfo.h"
struct Consideration
{
	EntityID m_Context;

	Consideration() { m_Context = MAX_ENTITY_COUNT + 1; };

	EntityManager* m_EntityManager = nullptr;

	virtual void SetContext(EntityID context)
	{
		m_Context = context;
	}

	//context = Who is making the decision, target is optional if we need to compare values with another character
	virtual float Evaluate(EntityID context, EntityID target = MAX_ENTITY_COUNT + 1)
	{
		return 0.0f;
	}
};

//The smaller army size of the enemy, the greater chance of winning the war
struct ArmySizeConsideration : public Consideration
{
	ArmySizeConsideration() : Consideration()
	{

	}

	void SetContext(EntityID context) override
	{
		m_Context = context;
	}

	float Evaluate(EntityID context, EntityID target = MAX_ENTITY_COUNT + 1) override
	{
		if (target > MAX_ENTITY_COUNT)
		{
			return 0.0f;
		}

		m_EntityManager = &EntityManager::Get();

		auto contextCharComp = m_EntityManager->GetComponent<CharacterComponent>(context);
		auto targetCharComp = m_EntityManager->GetComponent<CharacterComponent>(target);

		float armySizeDiff = contextCharComp.m_CurrentArmySize - targetCharComp.m_CurrentArmySize;

		float percentDiff = (float)targetCharComp.m_CurrentArmySize / contextCharComp.m_CurrentArmySize;

		if (armySizeDiff > 0)
		{
			//y = x^2
			return std::clamp(std::pow(percentDiff, 2.0f), 0.0f, 1.0f);
		}

		else
		{
			//y = 0.1^x
			return std::clamp(std::pow(0.1f, percentDiff - 1.0f), 0.0f, 1.0f);
		}

		return 0.0f;
	}
};

struct GoldConsideration : public Consideration
{
	GoldConsideration() : Consideration()
	{
	}

	void SetContext(EntityID context) override
	{
		m_Context = context;
	}


	float Evaluate(EntityID context, EntityID target = MAX_ENTITY_COUNT + 1) override
	{
		if (target > MAX_ENTITY_COUNT)
		{
			return 0.0f;
		}

		m_EntityManager = &EntityManager::Get();

		auto contextCharComp = m_EntityManager->GetComponent<CharacterComponent>(context);
		auto targetCharComp = m_EntityManager->GetComponent<CharacterComponent>(target);

		float targetGold = targetCharComp.m_CurrentGold;
		float contextGold = contextCharComp.m_CurrentGold;

		float goldDiff = contextGold / targetGold;
		float percentDiff = (float)contextGold / targetGold;

		if (goldDiff > 0)
		{
			//y = x^2
			return std::clamp(std::pow(percentDiff, 2.0f), 0.0f, 1.0f);
		}

		else
		{
			//y = 0.1^x
			return std::clamp(std::pow(0.1f, percentDiff - 1.0f), 0.0f, 1.0f);
		}


		return 0.0f;
	}
};

struct ExpansionConsideration : public Consideration
{
	ExpansionConsideration() : Consideration()
	{

	}

	void SetContext(EntityID context)
	{
		m_Context = context;
	}


	float Evaluate(EntityID context, int regionIndex)
	{
		m_EntityManager = &EntityManager::Get();
		CharacterComponent* characters = m_EntityManager->GetComponentArray<CharacterComponent>();

		float distanceWeight = 0.0f;

		int wantedRegionIndex = MapInfo::GetRegionIndex(MapInfo::GetRegionPositions(regionIndex)[regionIndex]);

		for (int ownedRegion : characters[context].m_OwnedRegionIDs)
		{
			if (std::abs(ownedRegion - regionIndex) > 1)
			{
				distanceWeight = -0.5f;
			}

			else
			{
				distanceWeight = 0.0f;
				break;
			}
		}

		//Consider potential gold gain
		int wantedRegionTax = MapInfo::GetRegionTax(regionIndex);

		std::vector<int> regionTax;
		for (int region : characters[context].m_OwnedRegionIDs)
		{
			regionTax.push_back(MapInfo::GetRegionTax(region));
		}

		float avgTax = 1.0 * std::accumulate(regionTax.begin(), regionTax.end(), 0LL) / regionTax.size();
		float percentDiff = (float)wantedRegionTax / avgTax;

		return std::clamp(std::pow(percentDiff, 2.0f) + distanceWeight, 0.0f, 1.0f);
	}
};