#include "AIWarManager.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/Systems/AI/AIConsideration.h"
#include "Game/DiplomacyManager.h"
#include "Game/UI/UIManager.h"
#include "Game/Data/AIData.h"
#include "../AIManager.h"

AIWarManager::AIWarManager()
{
	m_Warmanager = &DiplomacyManager::get();
}

AIWarManager::~AIWarManager()
{
	delete m_Warmanager;
}

void AIWarManager::update(AIData& data)
{
	if (data.m_CurrentAction != Action::War && m_Warmanager->getWarHandlesOfCharacter(data.m_OwnerID).size() == 0)
	{
		if (expansionDecision(data.m_OwnerID) > warConstants::m_expansionAcceptance)
		{
			float warEval = warDecision(data.m_OwnerID);

			if (data.m_LastAction == Action::War)
			{
				warEval -= .2f;
			}

			if (warEval > warConstants::m_warAcceptance)
			{
				if (!DiplomacyManager::get().hasTruce(data.m_OwnerID, AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent))
				{
					data.m_CurrentAction = Action::War;
					declareWar(data);
				}
			}
		}
	}

	for (auto& war : DiplomacyManager::get().getWarHandlesOfCharacter(data.m_OwnerID))
	{
		if (!m_Warmanager->isValidWar(war))
		{
			continue;
		}

		if (Time::m_GameDate.m_Date.m_Month - m_Warmanager->getWar(war)->getStartDate().m_Month >= 4)
		{
			bool sendOffer = (rand() % 100) < 10;

			if (sendOffer)
			{
				CharacterManager::get().sendPeaceOffer(data.m_OwnerID, m_Warmanager->getOpposingForce(war, data.m_OwnerID), PeaceType::White_Peace);
			}
		}
	}
}

float AIWarManager::warDecision(CharacterID ID)
{
	GoldConsideration goldConsideration;
	ArmySizeConsideration armySizeConsideration;

	goldConsideration.setContext(ID);
	armySizeConsideration.setContext(ID);

	float goldEvaluation = goldConsideration.evaluate(ID, AIManager::get().getWarmindOfCharacter(ID).m_Opponent);
	float enemyArmyEvaluation = armySizeConsideration.evaluate(ID, AIManager::get().getWarmindOfCharacter(ID).m_Opponent);
	float warMongerWeight = AIManager::get().getAIDataofCharacter(ID).m_Personality.m_DeclareWarModifier;
	float randomWeight = 0.0f;

	bool useRandomWeight = (rand() % 100) < 40;
	if (useRandomWeight)
	{
		randomWeight = warConstants::m_warAcceptance;
	}

	if (CharacterManager::get().isAlliedWith(ID, AIManager::get().getWarmindOfCharacter(ID).m_Opponent))
	{
		return 0.0f;
	}

	float actionScore = (goldEvaluation * enemyArmyEvaluation) + warMongerWeight;

	for (auto& war : DiplomacyManager::get().getWarsForRegion(AIManager::get().getWarmindOfCharacter(ID).m_WargoalRegionId))
	{
		actionScore -= .1f;
	}

	if (actionScore <= .3f)
	{
		return 0.0f;
	}

	return std::clamp(actionScore, 0.0f, 1.0f);
}

float AIWarManager::expansionDecision(CharacterID ID)
{
	std::vector<std::pair<float, int>> actionScorePerRegion;

	ExpansionConsideration expansionConsideration;

	expansionConsideration.setContext(ID);

	std::vector<int> regionIndexes = Map::get().getRegionIDs();

	Character& character = CharacterManager::get().getCharacter(ID);

	for (size_t i = 0; i < regionIndexes.size(); i++)
	{
		if (std::find(character.m_OwnedRegionIDs.begin(), character.m_OwnedRegionIDs.end(),
			(size_t)regionIndexes[i]) != character.m_OwnedRegionIDs.end())
		{
			continue;
		}

		float eval = expansionConsideration.evaluate(ID, regionIndexes[i]);

		if (eval > .5f)
		{
			auto pair = std::make_pair(eval, regionIndexes[i]);
			actionScorePerRegion.push_back(pair);
		}
	}

	if (actionScorePerRegion.empty())
	{
		return 0.0f;
	}

	std::sort(actionScorePerRegion.begin(), actionScorePerRegion.end());
	std::pair<float, int> region;

	if (actionScorePerRegion.size() > 2)
	{
		region = actionScorePerRegion[rand() % 2];
	}

	else
	{
		region = actionScorePerRegion[0];
	}


	AIManager::get().getWarmindOfCharacter(ID).m_WargoalRegionId = region.second;
	AIManager::get().getWarmindOfCharacter(ID).m_Opponent = Map::get().getRegionById(region.second).m_OwnerID;

	return region.first;
}

void AIWarManager::declareWar(AIData& data)
{
	CharacterManager& characterManager = CharacterManager::get();
	unsigned int opponent = AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent;

	if (opponent == INT_MAX || opponent == characterManager.getCharacter(data.m_OwnerID).m_Spouse)
	{
		return;
	}

	if (characterManager.isAlliedWith(data.m_OwnerID, AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent))
	{
		return;
	}

	if (!AIManager::get().isValidWarmind(AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent))
	{
		AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent = INVALID_CHARACTER_ID;
		return;
	}

	int warHandle = DiplomacyManager::get().createWar(data.m_OwnerID, AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent, AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_WargoalRegionId);

	characterManager.callAllies(data.m_OwnerID, warHandle);

	AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Active = true;

	WarmindComponent& warmind = AIManager::get().getWarmindOfCharacter(data.m_OwnerID);

	if (!characterManager.getCharacter(warmind.m_Opponent).m_IsPlayerControlled)
	{
		AIManager::get().getWarmindOfCharacter(warmind.m_Opponent).m_Active = true;
		AIManager::get().getWarmindOfCharacter(warmind.m_Opponent).m_Opponent = warmind.m_OwnerID;
	}

	else
	{
		UIManager::get().createUIEventElement(warmind.m_OwnerID, characterManager.getPlayerCharacterID(), UIType::WarDeclaration);
		UIManager::get().createWarIcon(warmind.m_OwnerID, characterManager.getPlayerCharacterID());
		characterManager.callAllies(characterManager.getPlayerCharacterID(), warHandle);
	}

	LOG_INFO("{0} Declared war against {1}", characterManager.getCharacter(data.m_OwnerID).m_Name, characterManager.getCharacter(AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent).m_Name);
	data.m_LastAction = Action::War;
	data.m_CurrentAction = Action::War;

	if (!characterManager.getCharacter(AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent).m_IsPlayerControlled)
	{
		AIManager::get().getAIDataofCharacter(characterManager.getCharacter(AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent).m_CharacterID).m_CurrentAction = Action::War;
		AIManager::get().getAIDataofCharacter(characterManager.getCharacter(AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent).m_CharacterID).m_LastAction = Action::War;
	}
}
