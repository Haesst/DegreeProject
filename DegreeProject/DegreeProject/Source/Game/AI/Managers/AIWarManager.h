#pragma once

#include "Game/Data/Types.h"
struct AIData;
class DiplomacyManager;

namespace warConstants
{
	const float m_warAcceptance = .4f;
	const float m_expansionAcceptance = .4f;
	const float m_RandomWarWeight = .3f;
};

class AIWarManager
{
public:
	AIWarManager();
	~AIWarManager();

	void update(AIData& data);

private:
	float warDecision(CharacterID ID);
	float expansionDecision(CharacterID ID);
	void declareWar(AIData& data);

private:
	DiplomacyManager* m_Warmanager = nullptr;
};
