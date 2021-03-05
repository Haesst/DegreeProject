#include "AI.h"

float AISystem::WarDecision(EntityID ent)
{
	CharacterComponent* characterComponents = m_EntityManager->GetComponentArray<CharacterComponent>();
	WarmindComponent* warmindComponents = m_EntityManager->GetComponentArray<WarmindComponent>();

	GoldConsideration goldConsideration;
	ArmySizeConsideration armySizeConsideration;

	goldConsideration.SetContext(ent);
	armySizeConsideration.SetContext(ent);

	float goldEvaluation = goldConsideration.Evaluate(ent, warmindComponents[ent].m_Opponent);
	float enemyArmyEvaluation = armySizeConsideration.Evaluate(ent, warmindComponents[ent].m_Opponent);

	float actionScore = goldEvaluation * enemyArmyEvaluation;
	return std::clamp(actionScore, 0.0f, 1.0f);
}
