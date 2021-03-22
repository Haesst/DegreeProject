#pragma once

#include "Game/GameData.h"
#include <Engine/Log.h>
#include <Game/GameDate.h>

struct RegionBuilding
{
	int m_BuildingId = -1;
	bool m_Finished = false;
	int m_DaysBuilt = 0;

	void StartBuild(int buildId)
	{
		if (GameData::m_Buildings.find(buildId) == GameData::m_Buildings.end())
		{
			LOG_ERROR("Building with id: {0} doesn't exist!", buildId);
			return;
		}

		m_BuildingId = buildId;

		// Subscribe to day change
		Time::m_GameDate.SubscribeToDayChange(std::bind(&RegionBuilding::OnDayChange, this, std::placeholders::_1));

		LOG_WARN("Construction Started!");
	}

	void OnDayChange(Date date)
	{
		if (m_Finished)
		{
			LOG_ERROR("Building still subscribed to day change when finished");
			return;
		}

		++m_DaysBuilt;

		if (GameData::m_Buildings[m_BuildingId].m_DaysToConstruct <= m_DaysBuilt)
		{
			LOG_WARN("Build successful!! Days constructed: {0}, buildtime: {1}", m_DaysBuilt, GameData::m_Buildings[m_BuildingId].m_DaysToConstruct);
			m_Finished = true;
			Time::m_GameDate.UnsubscribeToDayChange(std::bind(&RegionBuilding::OnDayChange, this, std::placeholders::_1));
		}
	}
};