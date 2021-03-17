#include "AI.h"

float AISystem::WarDecision(EntityID ent)
{
	CharacterComponent* characterComponents = m_EntityManager->GetComponentArray<CharacterComponent>();
	WarmindComponent* warmindComponents = m_EntityManager->GetComponentArray<WarmindComponent>();

	GoldConsideration goldConsideration;
	ArmySizeConsideration armySizeConsideration;

	Personality personality = m_AIManager->m_Personalities[characterComponents->m_PersonalityIndex];

	goldConsideration.SetContext(ent);
	armySizeConsideration.SetContext(ent);

	float goldEvaluation = goldConsideration.Evaluate(ent, warmindComponents[ent].m_Opponent);
	float enemyArmyEvaluation = armySizeConsideration.Evaluate(ent, warmindComponents[ent].m_Opponent);

	float actionScore = goldEvaluation * enemyArmyEvaluation;
	actionScore += personality.m_DeclareWarModifier;

	return std::clamp(actionScore, 0.0f, 1.0f);
}

float AISystem::ExpansionDecision(EntityID ent)
{
	std::vector<std::pair<float, int>> actionScorePerRegion;

	CharacterComponent* characterComponents = m_EntityManager->GetComponentArray<CharacterComponent>();
	WarmindComponent* warmindComponents = m_EntityManager->GetComponentArray<WarmindComponent>();

	ExpansionConsideration expansionConsideration;

	expansionConsideration.SetContext(ent);

	//Get characters in certain range,
	std::vector<int> regionIndexes = Map::GetRegionIDs();

	for (size_t i = 0; i < regionIndexes.size(); i++)
	{
		if (std::find(characterComponents[ent].m_OwnedRegionIDs.begin(), characterComponents[ent].m_OwnedRegionIDs.end(),
			(size_t)regionIndexes[i]) != characterComponents[ent].m_OwnedRegionIDs.end())
		{
			continue;
		}

		float eval = expansionConsideration.Evaluate(ent, regionIndexes[i]);
		auto pair = std::make_pair(eval, i);
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
			bestIndex = i;
		}
	}

	if (bestIndex == -1)
	{
		return 0.0f;
	}

	return actionScorePerRegion[bestIndex].first;
}

void AISystem::DeclareWar(EntityID instigator, EntityID target, int warGoalRegion)
{
	LOG_INFO("{0} Declared war with {1} for {2}", m_Characters[instigator].m_Name, m_Characters[target].m_Name, Map::m_Data.m_Regions[warGoalRegion].m_RegionName);

	m_Characters[instigator].m_AtWar = true;
	m_Warminds[instigator].m_Defending = false;
	m_Warminds[instigator].m_Active = true;
	m_Warminds[instigator].m_RecentlyAtWar = true;

	m_Characters[target].m_AtWar = true;
	m_Warminds[target].m_Opponent = instigator;
	m_Warminds[target].m_Defending = true;
	m_Warminds[target].m_WargoalRegionId = warGoalRegion;
	m_Warminds[target].m_Active = true;
	m_Warminds[target].m_RecentlyAtWar = true;
}
