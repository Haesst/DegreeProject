#pragma once
#include "Game/Data/Types.h"

class WarManager;
struct AIData;

namespace relationshipConstants
{
	const float marriageAcceptance = .5f;
	const float allianceAcceptance = .5f;
};

class AIRelationshipManager
{
public:
	AIRelationshipManager();
	void update(AIData& data);

private:
	void marry(AIData& data);
	void makeAlliance(AIData& data);

	WarManager* m_WarManager;
	float marriageDecision(AIData& data, CharacterID potentialSpouse);
	float allianceDecision(AIData& data, CharacterID ally);
	CharacterID getPotentialSpouse(AIData& data);
	CharacterID getPotentialAlly(AIData& data);
};