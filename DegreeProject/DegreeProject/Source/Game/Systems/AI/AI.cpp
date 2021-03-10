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
	CharacterComponent* characterComponents = m_EntityManager->GetComponentArray<CharacterComponent>();
	WarmindComponent* warmindComponents = m_EntityManager->GetComponentArray<WarmindComponent>();

	ExpansionConsideration expansionConsideration;

	expansionConsideration.SetContext(ent);

	float highestEvaluation = -1.f;

	//Get characters in certain range,
	//Iterate through them and return best evaluation.

	return 0.0f;
}
