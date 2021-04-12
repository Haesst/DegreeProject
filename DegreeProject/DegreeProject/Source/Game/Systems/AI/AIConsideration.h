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
			float value = 0.5f * percentDiff + 0.5f;
			return std::clamp(value, 0.0f, 1.0f);
		}

		else
		{
			//y = 0.1^x
			return std::clamp(std::pow(0.1f, percentDiff - 1.0f), 0.0f, 1.0f);
		}

		return 0.0f;
	}
};

struct UpgradeSettlementConsideration : public Consideration
{
	UpgradeSettlementConsideration() : Consideration()
	{

	}

	UpgradeSettlementConsideration(CharacterID context) { m_Context = context; }

	void setContext(CharacterID context) override
	{
		m_Context = context;
	}

	float evaluate(CharacterID context, int& outRegion)
	{
		CharacterManager* characterManager = CharacterManager::get();

		unsigned int lowestTax = INT_MAX;
		unsigned int targetRegionID = INT_MAX;

		for (auto& region : characterManager->getCharacter(context).m_OwnedRegionIDs)
		{
			if (Map::get().getRegionById(region).m_RegionTax < lowestTax)
			{
				lowestTax = Map::get().getRegionById(region).m_RegionTax;
				targetRegionID = region;
			}
		}

		if (lowestTax == INT_MAX || targetRegionID == INT_MAX)
		{
			return 0.0f;
		}

		outRegion = targetRegionID;

		float goldSurplusWeight = 0.0f;
		float atWarWeight = 0.0f;

		int buildingIndex = rand() % GameData::m_Buildings.size();

		int buildingId = -1;

		int index = 0;
		for (auto& pair : GameData::m_Buildings)
		{
			if (index == buildingIndex)
			{
				buildingId = pair.second.m_Id;
				break;
			}
			++index;
		}

		if (buildingId < 0)
		{
			return 0.0f;
		}

		Building building = GameData::m_Buildings[buildingId];

		if ((characterManager->getCharacter(context).m_CurrentGold - building.m_Cost) > 20)
		{
			goldSurplusWeight += .2f;
		}

		if (characterManager->getCharacter(context).m_CurrentWars.size() > 0)
		{
			atWarWeight -= .5;
		}
		
		else
		{
			atWarWeight += .4f;
		}

		return std::clamp(atWarWeight + goldSurplusWeight, 0.0f, 1.0f);
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

struct MarriageConsideration : public Consideration
{
	MarriageConsideration() : Consideration()
	{
	}

	void setContext(CharacterID context)
	{
		m_Context = context;
	}

	float evaluate(CharacterID context, CharacterID potentialSpouse)
	{
		ArmySizeConsideration armyConsideration;
		GoldConsideration goldConsideration;

		float armyEval = armyConsideration.evaluate(potentialSpouse, context);
		float goldEval = goldConsideration.evaluate(potentialSpouse, context);

		return std::clamp(armyEval * goldEval, 0.0f, 1.0f);
	}
};