#pragma once

#include "Game/GameData.h"
#include <Engine/Log.h>
#include <Game/GameDate.h>

struct RegionBuilding
{
	int m_BuildingId = -1;
	int m_RegionId = -1;
	int m_DaySubscribeHandle = -1;
	bool m_Finished = false;
	int m_DaysBuilt = 0;

	void startBuild(int buildId);

	void onDayChange(Date);
};