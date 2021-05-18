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
	if (m_Warmanager->getWarHandlesOfCharacter(data.m_OwnerID).size() == 0)
	{
		float expansionEval = expansionDecision(data.m_OwnerID);
		if (expansionEval > warConstants::m_expansionAcceptance)
		{
			float warEval = warDecision(data.m_OwnerID);
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

		if (Time::m_GameDate.m_Date.m_Month - m_Warmanager->getWar(war)->getStartDate().m_Month >= 6)
		{
			if (m_Warmanager->getWarscore(war, data.m_OwnerID) == 100)
			{
				CharacterManager::get().sendPeaceOffer(data.m_OwnerID, m_Warmanager->getOpposingForce(war, data.m_OwnerID), PeaceType::Enforce_Demands);
				continue;
			}

			if (m_Warmanager->getWarscore(war, data.m_OwnerID) >= 80)
			{
				bool tryEnforce = (rand() % 100) < 20;

				if (tryEnforce)
				{
					CharacterManager::get().sendPeaceOffer(data.m_OwnerID, m_Warmanager->getOpposingForce(war, data.m_OwnerID), PeaceType::Enforce_Demands);
					continue;
				}
			}

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
	AIManager* aiManager = &AIManager::get();

	float goldEvaluation = goldConsideration.evaluate(ID, AIManager::get().getWarmindOfCharacter(ID).m_Opponent);
	float enemyArmyEvaluation = armySizeConsideration.evaluate(ID, AIManager::get().getWarmindOfCharacter(ID).m_Opponent);
	float warMongerWeight = AIManager::get().getAIDataofCharacter(ID).m_Personality.m_DeclareWarModifier;
	float randomWeight = 0.0f;

	bool useRandomWeight = (rand() % 100) < 40;
	if (useRandomWeight)
	{
		randomWeight = warConstants::m_RandomWarWeight;
	}

	if (CharacterManager::get().isAlliedWith(ID, AIManager::get().getWarmindOfCharacter(ID).m_Opponent) && aiManager->getAIDataofCharacter(ID).m_Personality.m_DeclareWarModifier >= 0.0f)
	{
		return 0.0f;
	}

	float actionScore = (goldEvaluation * enemyArmyEvaluation) + warMongerWeight;

	if (AIManager::get().getWarmindOfCharacter(ID).m_Opponent == CharacterManager::get().getPlayerCharacterID())
	{
		actionScore += warConstants::m_PlayerWeight;
	}

	return std::clamp(actionScore, 0.0f, 1.0f);
}

float AIWarManager::expansionDecision(CharacterID ID)
{
	std::vector<std::pair<float, int>> actionScorePerRegion;

	ExpansionConsideration expansionConsideration;

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

		if (eval > .3f)
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
	std::pair<float, int> bestRegionPair;

	int bestRegion = -1;
	float bestEval = -1.f;

	int warsAgainstPlayer = DiplomacyManager::get().getWarHandlesAgainstCharacter(CharacterManager::get().getPlayerCharacterID()).size();

	for (auto& region : actionScorePerRegion)
	{
		if (Map::get().getRegionById(region.second).m_OwnerID == CharacterManager::get().getPlayerCharacterID())
		{
			region.first += warConstants::m_PlayerWeight;
		}

		if (region.first > bestEval)
		{
			bestEval = region.first;
			bestRegion = region.second;
			continue;
		}
	}

	bestRegionPair = std::make_pair(bestEval, bestRegion);
	AIManager::get().getWarmindOfCharacter(ID).m_WargoalRegionId = bestRegionPair.second;
	AIManager::get().getWarmindOfCharacter(ID).m_Opponent = Map::get().getRegionById(bestRegionPair.second).m_OwnerID;

	return bestRegionPair.first;
}

void AIWarManager::declareWar(AIData& data)
{
	CharacterManager& characterManager = CharacterManager::get();
	DiplomacyManager& diplomacyManager = DiplomacyManager::get();
	unsigned int opponent = AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent;

	if (opponent == INT_MAX || opponent == characterManager.getCharacter(data.m_OwnerID).m_Spouse)
	{
		return;
	}

	if (diplomacyManager.isAllied(data.m_OwnerID, opponent))
	{
		characterManager.getCharacter(data.m_OwnerID).m_CurrentGold -= warConstants::goldPenaltyOnBrokenAlliance;
		diplomacyManager.breakAlliance(data.m_OwnerID, opponent);
	}

	if (!AIManager::get().isValidWarmind(AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent) && !characterManager.getCharacter(opponent).m_IsPlayerControlled)
	{
		AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent = INVALID_CHARACTER_ID;
		return;
	}

	int warHandle = DiplomacyManager::get().createWar(data.m_OwnerID, AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent, AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_WargoalRegionId);

	WarmindComponent& warmind = AIManager::get().getWarmindOfCharacter(data.m_OwnerID);

	if (!characterManager.getCharacter(warmind.m_Opponent).m_IsPlayerControlled)
	{
		AIManager::get().getWarmindOfCharacter(warmind.m_Opponent).m_Active = true;
		AIManager::get().getWarmindOfCharacter(warmind.m_Opponent).m_Opponent = warmind.m_OwnerID;
		characterManager.callAllies(opponent, warHandle);
	}

	else
	{
		UIManager::get().createUIEventElement(warmind.m_OwnerID, characterManager.getPlayerCharacterID(), UIType::WarDeclaration);
		UIManager::get().createWarIcon(warmind.m_OwnerID, characterManager.getPlayerCharacterID());
		characterManager.callAllies(characterManager.getPlayerCharacterID(), warHandle);
	}

	characterManager.callAllies(data.m_OwnerID, warHandle);
	AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Active = true;

	LOG_WARN("{0} Declared war against {1}", characterManager.getCharacter(data.m_OwnerID).m_Name, characterManager.getCharacter(AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent).m_Name);
	data.m_LastAction = Action::War;
	data.m_CurrentAction = Action::War;

	if (!characterManager.getCharacter(AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent).m_IsPlayerControlled)
	{
		AIManager::get().getAIDataofCharacter(characterManager.getCharacter(AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent).m_CharacterID).m_CurrentAction = Action::War;
		AIManager::get().getAIDataofCharacter(characterManager.getCharacter(AIManager::get().getWarmindOfCharacter(data.m_OwnerID).m_Opponent).m_CharacterID).m_LastAction = Action::War;
	}
}
