#include "AISettlementManager.h"
#include "Game/Systems/AI/AIConsideration.h"
#include "Game/Data/AIData.h"

AISettlementManager::AISettlementManager()
{
}

void AISettlementManager::update(AIData& data)
{
	int regionID = INVALID_REGION_ID;
	float settlementEval = upgradeDecision(data.m_OwnerID, regionID);

	if (settlementEval > settlementUpgradeConstants::minUpgradeEval)
	{
		data.m_SettlementToUpgrade = regionID;
		upgradeSettlement(data);
	}
}

void AISettlementManager::upgradeSettlement(AIData& data)
{
	if (data.m_SettlementToUpgrade == INVALID_REGION_ID)
	{
		return;
	}

	int buildingIndex = rand() % GameData::m_Buildings.size();

	int buildingId = INVALID_BUILDING_ID;

	int index = 0;
	for (auto& pair : GameData::m_Buildings)
	{
		if (index == buildingIndex)
		{
			buildingId = pair.second.m_Id;
			break;
		}

		++index;
	}

	for (int i = 0; i < 4; i++)
	{
		if (Map::get().getRegionById(data.m_SettlementToUpgrade).m_BuildingSlots[i].m_BuildingId == INVALID_BUILDING_ID)
		{
			CharacterManager::get().constructBuilding(data.m_OwnerID, buildingId, data.m_SettlementToUpgrade, i);
			data.m_LastAction = Action::Upgrade_Settlement;
			break;
		}
	}
}

float AISettlementManager::upgradeDecision(CharacterID ID, int& outRegion)
{
	UpgradeSettlementConsideration consideration;
	consideration.setContext(ID);
	return consideration.evaluate(ID, outRegion);
}
