#pragma once
#include <Game/Data/Types.h>

struct AIData;

namespace settlementUpgradeConstants
{
	const float minUpgradeEval = .5f;
};

class AISettlementManager
{

public:
	AISettlementManager();
	void update(AIData& data);

private:
	CharacterID getPotentialSpouse(AIData& data);
	void upgradeSettlement(AIData& data);
	float upgradeDecision(CharacterID ID, int& outRegion);
};
