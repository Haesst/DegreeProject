#pragma once
#include <vector>
#include "Game/GameDate.h"
#include "Game/Map/Map.h"
#include "Engine/Log.h"
#include "ECS/Component.h"
#include "Game/Components/Unit.h"
#include "Game/Components/SpriteRenderer.h"
#include "Game/Components/Warmind.h"
#include "Game/Data/Titles.h"

#pragma warning(push)
#pragma warning(disable: 26812)
//struct CharacterComponent : public Component
//{
//	sf::Sprite m_CharacterPortrait;
//	Title m_CharacterTitle = Title::Baron;
//	const char* m_KingdomName = "";
//	const char* m_Name = "";
//	std::vector<unsigned int> m_OwnedRegionIDs;
//	int m_CurrentGold = 0;
//	unsigned int m_RaisedArmySize = 0;
//	unsigned int m_CurrentMaxArmySize = 0;
//	unsigned int m_MaxArmySize = 0;
//	bool m_IsPlayerControlled = false;
//	sf::Color m_RegionColor = sf::Color::Black;
//	bool m_AtWar = false;
//	int m_PersonalityIndex = 0;
//	int m_Income = 0;
//	std::vector<int> m_CurrentWars = std::vector<int>();
//
//	EntityID m_TextUI = (SIZE_T)INT_MAX;
//
//	bool m_RecentlyAtWar = false;  //Flag for raising units when war starts
//
//	EntityID m_UnitEntity = INT_MAX;
//
//	CharacterComponent() {};
//
//	CharacterComponent(Title title, const char* kingdomName, const char* characterName, std::vector<unsigned int> regionIDs, int gold, int armySize, bool isPlayerControlled, sf::Color regionColor, int personalityIndex)
//	{
//		m_CharacterTitle = title;
//		m_KingdomName = kingdomName;
//		m_Name = characterName;
//		m_OwnedRegionIDs = regionIDs;
//		m_CurrentGold = gold;
//		m_MaxArmySize = armySize;
//		m_CurrentMaxArmySize = m_MaxArmySize;
//		m_IsPlayerControlled = isPlayerControlled;
//		m_RegionColor = regionColor;
//		m_PersonalityIndex = personalityIndex;
//		initUnit();
//	}
//
//	void start()
//	{
//		Time::m_GameDate.subscribeToMonthChange(std::bind(&CharacterComponent::onMonthChange, this, std::placeholders::_1));
//	}
//
//	void declareWar(EntityID target, int warGoalRegion)
//	{
////		EntityManager* entityManager = &EntityManager::get();
////
////#pragma warning(push)
////#pragma warning(disable: 26815)
////		CharacterSystem* characterSystem = (CharacterSystem*)entityManager->getSystem<CharacterSystem>().get();
////#pragma warning(pop)
////
////		characterSystem->declareWar(m_EntityID, target, warGoalRegion);
//	}
//
//	void onMonthChange(Date)
//	{
//		int incomingGold = 0;
//
//		for (auto& regionId : m_OwnedRegionIDs)
//		{
//			MapRegion region = Map::get().getRegionById(regionId);
//
//			incomingGold += region.m_RegionTax;
//		}
//
//		incomingGold -= (int)(m_RaisedArmySize * 0.1f);
//		m_Income = incomingGold;
//		m_CurrentGold += incomingGold;
//	}
//
//	void initUnit()
//	{
//		EntityManager* entityManager = &EntityManager::get();
//		CharacterComponent& character = entityManager->getComponent<CharacterComponent>(m_EntityID);
//
//		m_UnitEntity = entityManager->addNewEntity();
//		entityManager->addComponent<UnitComponent>(m_UnitEntity, character.m_MaxArmySize, m_EntityID);
//
//		AssetHandler handler;
//		entityManager->addComponent<SpriteRenderer>(m_UnitEntity, "Assets/Graphics/soldier unit.png", 32, 32, &handler);
//		SpriteRenderer& renderer = entityManager->getComponent<SpriteRenderer>(m_UnitEntity);
//		renderer.m_ShouldRender = false;
//	}
//
//	void constructBuilding(int buildingId, int regionId, int buildingSlot)
//	{
//		Building building = GameData::m_Buildings[buildingId];
//		MapRegion& region = Map::get().getRegionById(regionId);
//
//		if (building.m_Cost > m_CurrentGold)
//		{
//			return;
//		}
//
//		if (region.m_OwnerID != m_EntityID)
//		{
//			return;
//		}
//
//		if (region.m_BuildingSlots[buildingSlot].m_BuildingId != -1)
//		{
//			return;
//		}
//
//		Map::get().startConstructionOfBuilding(buildingId, buildingSlot, regionId);
//		m_CurrentGold -= building.m_Cost;
//	}
//};
#pragma warning(pop)
