#include "AI.h"
#include "Game/Systems/CharacterSystem.h"

void AISystem::update()
{
	m_Characters = m_EntityManager->getComponentArray<CharacterComponent>();
	m_Warminds = m_EntityManager->getComponentArray<WarmindComponent>();
	m_TickAccu += Time::deltaTime();

	if (m_TickAccu <= m_AIUpdateTickRate)
	{
		return;
	}

	//AI System update considerations
	for (auto entity : m_Entities)
	{
		if (expansionDecision(entity) > .2f) //Add personality weight
		{
			if (warDecision(entity) > .2f)
			{
				if (!m_Characters[entity].m_AtWar)
				{
#pragma warning(push)
#pragma warning(disable: 26815)
					CharacterSystem* characterSystem = (CharacterSystem*)m_EntityManager->getSystem<CharacterSystem>().get();
#pragma warning(pop)
					characterSystem->declareWar(m_Characters[entity].getID(), m_Warminds[entity].m_Opponent, m_Warminds[entity].m_WargoalRegionId);
				}
			}
		}

		m_TickAccu = 0.0f;
	}
}

float AISystem::warDecision(EntityID ent)
{
	CharacterComponent* characterComponents = m_EntityManager->getComponentArray<CharacterComponent>();
	WarmindComponent* warmindComponents = m_EntityManager->getComponentArray<WarmindComponent>();

	GoldConsideration goldConsideration;
	ArmySizeConsideration armySizeConsideration;

	Personality personality = m_AIManager->m_Personalities[characterComponents->m_PersonalityIndex];

	goldConsideration.setContext(ent);
	armySizeConsideration.setContext(ent);

	float goldEvaluation = goldConsideration.evaluate(ent, warmindComponents[ent].m_Opponent);
	float enemyArmyEvaluation = armySizeConsideration.evaluate(ent, warmindComponents[ent].m_Opponent);

	float actionScore = goldEvaluation * enemyArmyEvaluation;
	actionScore += personality.m_DeclareWarModifier;

	War* war = m_WarManager->getWarAgainst(m_Characters[ent], m_Characters[m_Warminds[ent].m_Opponent]);

	if (war == nullptr)
	{
		LOG_INFO("VALID WAR");
		return std::clamp(actionScore, 0.0f, 1.0f);
	}

	LOG_INFO("Cant declare war against someone you are at war with");
	return 0.0f;
}

float AISystem::expansionDecision(EntityID ent)
{
	std::vector<std::pair<float, int>> actionScorePerRegion;

	CharacterComponent* characterComponents = m_EntityManager->getComponentArray<CharacterComponent>();
	WarmindComponent* warmindComponents = m_EntityManager->getComponentArray<WarmindComponent>();

	ExpansionConsideration expansionConsideration;

	expansionConsideration.setContext(ent);

	//Get characters in certain range,
	std::vector<int> regionIndexes = Map::get().getRegionIDs();

	for (size_t i = 0; i < regionIndexes.size(); i++)
	{
		if (std::find(characterComponents[ent].m_OwnedRegionIDs.begin(), characterComponents[ent].m_OwnedRegionIDs.end(),
			(size_t)regionIndexes[i]) != characterComponents[ent].m_OwnedRegionIDs.end())
		{
			continue;
		}

		float eval = expansionConsideration.evaluate(ent, regionIndexes[i]);
		auto pair = std::make_pair(eval, regionIndexes[i]);
		actionScorePerRegion.push_back(pair);
	}

	float highest = -1.0f;
	int bestIndex = -1;

	for (unsigned int i = 0; i < actionScorePerRegion.size(); i++)
	{
		if (actionScorePerRegion[i].first > highest)
		{
			highest = actionScorePerRegion[i].first;
			warmindComponents[ent].m_WargoalRegionId = actionScorePerRegion[i].second;
			warmindComponents[ent].m_Opponent = Map::get().getRegionById(actionScorePerRegion[i].second).m_OwnerID;
			bestIndex = i;
		}
	}

	if (bestIndex == -1)
	{
		return 0.0f;
	}

	return actionScorePerRegion[bestIndex].first;
}