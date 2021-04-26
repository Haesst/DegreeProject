#pragma once

#include "Game/Data/Types.h"
struct AIData;
class WarManager;

namespace warConstants
{
	const float m_warAcceptance = .5f;
	const float m_expansionAcceptance = .5f;
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
	WarManager* m_Warmanager = nullptr;
};
