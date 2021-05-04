#include "RegionBuilding.h"

#include "Game/Systems/CharacterManager.h"
#include "Game/Map/Map.h"

void RegionBuilding::startBuild(int buildId)
{
	if (GameData::m_Buildings.find(buildId) == GameData::m_Buildings.end())
	{
		LOG_ERROR("Building with id: {0} doesn't exist!", buildId);
		return;
	}

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

	//LOG_INFO("Building. Day {0} of {1}", m_DaysBuilt, GameData::m_Buildings[m_BuildingId].m_DaysToConstruct);

	if (GameData::m_Buildings[m_BuildingId].m_DaysToConstruct <= m_DaysBuilt)
	{
		//LOG_WARN("Build successful!! Days constructed: {0}, buildtime: {1}", m_DaysBuilt, GameData::m_Buildings[m_BuildingId].m_DaysToConstruct);
		m_Finished = true;
		Time::m_GameDate.unsubscribeToDayChange(m_DaySubscribeHandle);
		CharacterManager::get().getCharacter(Map::get().getRegionById(m_RegionId).m_OwnerID).m_MaxArmySize += GameData::m_Buildings[m_BuildingId].m_ArmyModifier;
	}
}
