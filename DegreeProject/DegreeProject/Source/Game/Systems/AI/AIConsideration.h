#pragma once
#include <algorithm>
#include "Game/Map/Map.h"

struct Consideration
{
	CharacterID m_Context;

	Consideration() { m_Context = INVALID_CHARACTER_ID; };

	Consideration(CharacterID context) { m_Context = context; }

	virtual void setContext(CharacterID context)
	{
		m_Context = context;
	}

	//context = Who is making the decision, target is optional if we need to compare values with another character
	virtual float evaluate(CharacterID, CharacterID)
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

	void setContext(CharacterID context) override
	{
		m_Context = context;
	}

	float evaluate(CharacterID context, CharacterID target = INVALID_CHARACTER_ID) override
	{
		if (target == INVALID_CHARACTER_ID)
		{
			return 0.0f;
		}

		auto contextCharComp = CharacterManager::get()->getCharacter(context);
		auto targetCharComp = CharacterManager::get()->getCharacter(target);

		float armySizeDiff = (float)contextCharComp.m_MaxArmySize - targetCharComp.m_MaxArmySize;

		float percentDiff = (float)targetCharComp.m_MaxArmySize / contextCharComp.m_MaxArmySize;

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

	GoldConsideration(CharacterID context) { m_Context = context; }

	void setContext(CharacterID context) override
	{
		m_Context = context;
	}


	float evaluate(CharacterID context, CharacterID target = INVALID_CHARACTER_ID) override
	{
		float positiveGoldDiffWeight = 0.2f;

		if (target == INVALID_CHARACTER_ID)
		{
			return 0.0f;
		}

		auto contextCharComp = CharacterManager::get()->getCharacter(context);
		auto targetCharComp = CharacterManager::get()->getCharacter(target);

		float targetGold = (float)targetCharComp.m_CurrentGold;
		float contextGold = (float)contextCharComp.m_CurrentGold;

		float goldDiff = contextGold / targetGold;
		float percentDiff = (float)contextGold / targetGold;

		if (goldDiff > 0)
		{
			//y = x^2

			return std::clamp(std::pow(percentDiff, 2.0f), 0.0f, 1.0f) + positiveGoldDiffWeight;
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

	ExpansionConsideration(CharacterID context) { m_Context = context; }

	void setContext(CharacterID context)
	{
		m_Context = context;
	}


	float evaluate(CharacterID context, int regionIndex)
	{
		float distanceWeight = 0.0f;

		//for (int ownedRegion : characters[context].m_OwnedRegionIDs)
		//{
		//	if (std::abs(ownedRegion - regionIndex) > 1)
		//	{
		//		distanceWeight = -0.5f;
		//	}
		//
		//	else
		//	{
		//		distanceWeight = 0.0f;
		//		break;
		//	}
		//}

		//Consider potential gold gain
		int wantedRegionTax = Map::get().getRegionById(regionIndex).m_RegionTax;

		std::vector<int> regionTax;
		for (int region : CharacterManager::get()->getCharacter(context).m_OwnedRegionIDs)
		{
			regionTax.push_back(Map::get().getRegionById(region).m_RegionTax);
		}

		float avgTax = 1.0f * std::accumulate(regionTax.begin(), regionTax.end(), 0LL) / regionTax.size();
		float percentDiff = (float)wantedRegionTax / avgTax;

		return std::clamp(std::pow(percentDiff, 2.0f) + distanceWeight, 0.0f, 1.0f);
	}
};