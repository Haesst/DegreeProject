#include "RegionBuilding.h"

#include "Game/Systems/CharacterManager.h"
#include "Game/Map/Map.h"

void RegionBuilding::startBuild(int buildId, int buildingSlot)
{
	if (GameData::m_Buildings.find(buildId) == GameData::m_Buildings.end())
	{
		LOG_ERROR("Building with id: {0} doesn't exist!", buildId);
		return;
	}

	m_BuildingSlot = buildingSlot;
	m_BuildingId = buildId;
	m_DaySubscribeHandle = Time::m_GameDate.subscribeToDayChange([](void* data) { RegionBuilding& building = *static_cast<RegionBuilding*>(data); building.onDayChange(Time::m_GameDate.m_Date); }, static_cast<void*>(this));

	LOG_WARN("Construction Started!");
}

void RegionBuilding::onDayChange(Date)
{
	if (m_Finished)
	{
		LOG_ERROR("Building still subscribed to day change when finished");
		return;
	}

	++m_DaysBuilt;

	if (GameData::m_Buildings[m_BuildingId].m_DaysToConstruct <= m_DaysBuilt)
	{
		m_Finished = true;
		Time::m_GameDate.unsubscribeToDayChange(m_DaySubscribeHandle);
		MapRegion& region = Map::get().getRegionById(m_RegionId);
		region.m_BuildingSlots[m_BuildingSlot].m_Finished = true;

		CharacterManager::get().getCharacter(region.m_OwnerID).m_MaxArmySize += GameData::m_Buildings[m_BuildingId].m_ArmyModifier;
	}
}
