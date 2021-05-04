#pragma once
#include <algorithm>
#include "Game/Map/Map.h"
#include "Game/DiplomacyManager.h"
#include "Game/AI/AIManager.h"
#include "Game/Data/AIData.h"

namespace ConsiderationConstants
{
	const float maxBuildingCost = 20.0f;
	const float positiveGoldDiffWeight = 0.2f;
};

struct Consideration
{
	Consideration() {};

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

	float evaluate(CharacterID context, CharacterID target = INVALID_CHARACTER_ID) override
	{
		if (target == INVALID_CHARACTER_ID)
		{
			return 0.0f;
		}

		auto contextCharComp = CharacterManager::get().getCharacter(context);
		auto targetCharComp = CharacterManager::get().getCharacter(target);

		float friendlyArmy = 0.0f;

		for (auto& ally : DiplomacyManager::get().getAlliances(context))
		{
			friendlyArmy += CharacterManager::get().getCharacter(ally).m_CurrentMaxArmySize;
		}

		friendlyArmy += contextCharComp.m_CurrentMaxArmySize;

		float armySizeDiff = friendlyArmy - targetCharComp.m_MaxArmySize;

		float percentDiff = (float)targetCharComp.m_MaxArmySize / contextCharComp.m_MaxArmySize;

		if (armySizeDiff > 0)
		{
			//y = x^2
			float value = percentDiff + 0.5f;
			return std::clamp(value, 0.0f, 1.0f);
		}

		else
		{
			//y = 0.1^x
			return std::clamp(std::pow(0.1f, percentDiff), 0.0f, 1.0f);
		}

		return 0.0f;
	}
};

struct UpgradeSettlementConsideration : public Consideration
{
	UpgradeSettlementConsideration() : Consideration()
	{

	}

	float evaluate(CharacterID context, int& outRegion)
	{
		CharacterManager& characterManager = CharacterManager::get();

		unsigned int lowestTax = INT_MAX;
		unsigned int targetRegionID = INT_MAX;

		for (auto& region : characterManager.getCharacter(context).m_OwnedRegionIDs)
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

		if ((characterManager.getCharacter(context).m_CurrentGold - building.m_Cost) > ConsiderationConstants::maxBuildingCost)
		{
			goldSurplusWeight += .5f;
		}

		return std::clamp(goldSurplusWeight + (lowestTax * 0.1f), 0.0f, 1.0f);
	}
};

struct GoldConsideration : public Consideration
{
	GoldConsideration() : Consideration()
	{
	}

	float evaluate(CharacterID context, CharacterID target = INVALID_CHARACTER_ID) override
	{
		if (target == INVALID_CHARACTER_ID)
		{
			return 0.0f;
		}

		auto contextCharComp = CharacterManager::get().getCharacter(context);
		auto targetCharComp = CharacterManager::get().getCharacter(target);

		float targetGold = (float)targetCharComp.m_CurrentGold;
		float contextGold = (float)contextCharComp.m_CurrentGold;

		float goldDiff = contextGold / targetGold;
		float percentDiff = (float)contextGold / targetGold;

		if (goldDiff > 0)
		{
			//y = x^2
			return std::clamp(std::pow(percentDiff, 2.0f), 0.0f, 1.0f) + ConsiderationConstants::positiveGoldDiffWeight;
		}

		else
		{
			//y = 0.1^x
			return std::clamp(std::pow(0.1f, percentDiff), 0.0f, 1.0f);
		}

		return 0.0f;
	}
};

struct ExpansionConsideration : public Consideration
{
	ExpansionConsideration() : Consideration()
	{

	}

	float evaluate(CharacterID context, int regionIndex)
	{
		auto& character = CharacterManager::get().getCharacter(context);

		for (auto& region : character.m_OwnedRegionIDs)
		{
			if (region == regionIndex)
			{
				return 0.0f;
			}
		}

		float distanceWeight = 0.0f;

		Vector2DInt wantedRegionPos = Map::get().getRegionById(regionIndex).m_RegionCapital;

		for (int ownedRegion : CharacterManager::get().getCharacter(context).m_OwnedRegionIDs)
		{
			Vector2DInt regionPos = Map::get().getRegionById(ownedRegion).m_RegionCapital;

			float dist = (regionPos - wantedRegionPos).getLength();

			if (dist > 10)
			{
				distanceWeight = -0.6f;
			}

			else
			{
				distanceWeight = 0.0f;
				break;
			}
		}

		//Consider potential gold gain
		int wantedRegionTax = Map::get().getRegionById(regionIndex).m_RegionTax;

		std::vector<int> regionTax;
		for (int region : CharacterManager::get().getCharacter(context).m_OwnedRegionIDs)
		{
			regionTax.push_back(Map::get().getRegionById(region).m_RegionTax);
		}

		float avgTax = std::accumulate(regionTax.begin(), regionTax.end(), 0LL) / (float)regionTax.size();
		float percentDiff = (float)wantedRegionTax / avgTax;

		float warMongerWeight = AIManager::get().getAIDataofCharacter(context).m_Personality.m_DeclareWarModifier;
		return std::clamp(std::pow(percentDiff, 2.0f) + distanceWeight + warMongerWeight, 0.0f, 1.0f);
	}
};

struct MarriageConsideration : public Consideration
{
	MarriageConsideration() : Consideration()
	{
	}

	float evaluate(CharacterID context, CharacterID potentialSpouse)
	{
		ArmySizeConsideration armyConsideration;
		GoldConsideration goldConsideration;

		float armyEval = armyConsideration.evaluate(potentialSpouse, context);
		float goldEval = goldConsideration.evaluate(potentialSpouse, context);
		float fertilityWeight = CharacterManager::get().getCharacter(potentialSpouse).m_Fertility;

		return std::clamp((armyEval * goldEval) + fertilityWeight, 0.0f, 1.0f);
	}
};