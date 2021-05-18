#include "AIRelationshipManager.h"
#include "Game/Data/AIData.h"
#include "Game/Systems/AI/AIConsideration.h"
#include "Game/Map/Map.h"
#include "Game/DiplomacyManager.h"

AIRelationshipManager::AIRelationshipManager()
{
	m_WarManager = &DiplomacyManager::get();
}

void AIRelationshipManager::update(AIData& data)
{
	CharacterID potentialSpouse = getPotentialSpouse(data);

	if (potentialSpouse != INVALID_CHARACTER_ID && Time::m_GameDate.m_Date.m_Month - data.m_LastMarriageOfferDate.m_Month >= relationshipConstants::marriageOfferCooldown)
	{
		float marriageEval = marriageDecision(data, potentialSpouse);

		if (marriageEval > relationshipConstants::marriageAcceptance)
		{
			data.m_LastAction = Action::Marriage;
			data.m_PotentialSpouseID = potentialSpouse;
			marry(data);
		} 

		if (Time::m_GameDate.getAge(CharacterManager::get().getCharacter(data.m_OwnerID).m_Birthday) > 30)
		{
			if (CharacterManager::get().getCharacter(data.m_OwnerID).m_Spouse == INVALID_CHARACTER_ID)
			{
				CharacterID spouse = getUnlandedPotentialSpouse(data);

				if (spouse != INVALID_CHARACTER_ID)
				{
					data.m_PotentialSpouseID = spouse;
					marry(data);
				}
			}
		}
	}

	if (m_WarManager->getAlliances(data.m_OwnerID).size() < relationshipConstants::maxAllies)
	{
		CharacterID potentialAlly = getPotentialAlly(data);
		float eval = allianceDecision(data, potentialAlly);

		if (eval > relationshipConstants::allianceAcceptance)
		{
			data.m_LastAction = Action::Seek_Alliance;
			data.m_PotentialAllyID = potentialAlly;
			makeAlliance(data);
		}
	}
}

void AIRelationshipManager::marry(AIData& data)
{
	if (data.m_PotentialSpouseID == INVALID_CHARACTER_ID)
	{
		return;
	}
	data.m_LastMarriageOfferDate = Time::m_GameDate.m_Date;
	CharacterManager::get().marry(data.m_OwnerID, data.m_PotentialSpouseID);
}

void AIRelationshipManager::makeAlliance(AIData& data)
{
	if (data.m_PotentialAllyID == INVALID_CHARACTER_ID)
	{
		return;
	}

	CharacterManager::get().sendAllianceOffer(data.m_OwnerID, data.m_PotentialAllyID);
}

float AIRelationshipManager::marriageDecision(AIData& data, CharacterID potentialSpouse)
{
	MarriageConsideration marriage;
	float opinionWeight = (float)CharacterManager::get().getCharacterOpinion(data.m_OwnerID, potentialSpouse);
	return marriage.evaluate(data.m_OwnerID, potentialSpouse) + (opinionWeight * 0.1f);
}

float AIRelationshipManager::allianceDecision(AIData& data, CharacterID ally)
{
	GoldConsideration goldConsideration;
	ArmySizeConsideration armyConsideration;

	float goldEval = goldConsideration.evaluate(data.m_OwnerID, ally);
	float armyEval = armyConsideration.evaluate(data.m_OwnerID, ally);
	float actionScore = (armyEval * goldEval);

	if (actionScore > .5f)
	{
		return actionScore;
	}

	return 0.0f;
}

CharacterID AIRelationshipManager::getPotentialSpouse(AIData& data)
{
	std::vector<std::pair<float, int>> evalToSpouse;

	if (CharacterManager::get().getCharacter(data.m_OwnerID).m_Spouse != INVALID_CHARACTER_ID)
	{
		return INVALID_CHARACTER_ID;
	}

	for (auto& region : Map::get().getRegionIDs())
	{
		if (Map::get().getRegionById(region).m_OwnerID == data.m_OwnerID)
			continue;

		if (CharacterManager::get().getCharacter(data.m_OwnerID).m_Gender == CharacterManager::get().getCharacter(Map::get().getRegionById(region).m_OwnerID).m_Gender)
			continue;

		if(DiplomacyManager::get().getWarAgainst(data.m_OwnerID, Map::get().getRegionById(region).m_OwnerID) != nullptr)
			continue;

		float eval = marriageDecision(data, Map::get().getRegionById(region).m_OwnerID);

		if (eval > relationshipConstants::marriageAcceptance)
			evalToSpouse.push_back(std::make_pair(eval, Map::get().getRegionById(region).m_OwnerID));
	}

	float highestEval = -1.f;
	CharacterID bestSpouse = INVALID_CHARACTER_ID;

	for (auto& pair : evalToSpouse)
	{
		if (pair.first > highestEval)
		{
			highestEval = pair.first;
			bestSpouse = pair.second;
		}
	}

	return bestSpouse;
}

CharacterID AIRelationshipManager::getUnlandedPotentialSpouse(AIData& data)
{
	CharacterManager* characterManager = &CharacterManager::get();

	if (characterManager->getCharacter(data.m_OwnerID).m_Gender == Gender::Male)
	{
		return CharacterManager::get().getUnlandedCharacterOfGender(Gender::Female);
	}

	if (characterManager->getCharacter(data.m_OwnerID).m_Gender == Gender::Female)
	{
		return CharacterManager::get().getUnlandedCharacterOfGender(Gender::Male);
	}

	return INVALID_CHARACTER_ID;
}

CharacterID AIRelationshipManager::getPotentialAlly(AIData& data)
{
	std::vector<std::pair<float, int>> evalToAlly;
	DiplomacyManager& diplomacyManager = DiplomacyManager::get();
	Map& gameMap = Map::get();

	for (auto& region : Map::get().getRegionIDs())
	{
		if (Map::get().getRegionById(region).m_OwnerID != data.m_OwnerID)
		{
			if (CharacterManager::get().getCharacter(gameMap.getRegionById(region).m_OwnerID).m_CharacterTitle == Title::Unlanded)
			{
				continue;
			}

			if (diplomacyManager.atWarWith(data.m_OwnerID, gameMap.getRegionById(region).m_OwnerID))
			{
				continue;
			}

			float eval = allianceDecision(data, gameMap.getRegionById(region).m_OwnerID);

			if (diplomacyManager.getAlliances(gameMap.getRegionById(region).m_OwnerID).size() > 2)
			{
				eval -= (.2f * diplomacyManager.getAlliances(gameMap.getRegionById(region).m_OwnerID).size());
			}

			if (eval > .4f)
			{
				evalToAlly.push_back(std::make_pair(eval, gameMap.getRegionById(region).m_OwnerID));
			}
		}
	}

	float highestEval = -1.f;
	CharacterID bestAlly = INVALID_CHARACTER_ID;

	for (auto& pair : evalToAlly)
	{
		if (pair.first > highestEval)
		{
			highestEval = pair.first;
			bestAlly = pair.second;
			data.m_PotentialAllyID = bestAlly;
		}
	}

	return bestAlly;
}