#pragma once

#include "ECS/System.h"
#include "Game/Components/Player.h"


struct PlayerSystem : System
{
	EntityManager* m_EntityManager;

	PlayerSystem()
	{
		AddComponentSignature<Player>();
		m_EntityManager = &EntityManager::Get();
	}

	virtual void Update() override
	{
		Player* playerComponents = m_EntityManager->GetComponentArray<Player>();

		for (auto& entity : m_Entities)
		{
			HoverOverRegion(playerComponents[entity]);
		}
	}

	void HoverOverRegion(Player& playerComp)
	{
		Vector2D currentMousePosition = InputHandler::GetMousePosition();
		Vector2DInt mapPos = Map::ConvertToMap(currentMousePosition);

		if (Map::m_MapUnitData.find(mapPos) != Map::m_MapUnitData.end())
		{
			EntityID regionID = Map::m_MapUnitData[mapPos].m_RegionID;

			if (playerComp.m_HighlightedRegion != regionID)
			{
				Map::GetRegionById(playerComp.m_HighlightedRegion).m_Highlighted = false;

				Map::GetRegionById(regionID).m_Highlighted = true;
				playerComp.m_HighlightedRegion = regionID;
			}
		}
		else
		{
			if (playerComp.m_HighlightedRegion >= 0)
			{
				Map::GetRegionById(playerComp.m_HighlightedRegion).m_Highlighted = false;
				playerComp.m_HighlightedRegion = -1;
			}
		}
	}
};