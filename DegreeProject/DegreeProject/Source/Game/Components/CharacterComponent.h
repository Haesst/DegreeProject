#pragma once
#include <vector>
#include "Game/GameDate.h"
#include "Game/Map/Map.h"
#include "Engine/Log.h"
#include "ECS/Component.h"

enum class Title
{
	Emperor,
	King,
	Duke,
	Count,
	Baron
};

#pragma warning(push)
#pragma warning(disable: 26812)
struct CharacterComponent : public Component
{
	sf::Sprite m_CharacterPortrait;
	Title m_CharacterTitle = Title::Baron;
	const char* m_KingdomName = "";
	const char* m_Name = "";
	std::vector<unsigned int> m_OwnedRegionIDs;
	int m_CurrentGold = 0;
	int m_RaisedArmySize = 0;
	unsigned int m_MaxArmySize = 0;
	bool m_IsPlayerControlled = false;
	sf::Color m_RegionColor = sf::Color::Black;
	bool m_AtWar = false;
	int m_PersonalityIndex = 0;

	CharacterComponent() {};

	CharacterComponent(Title title, const char* kingdomName, const char* characterName, std::vector<unsigned int> regionIDs, int gold, int armySize, bool isPlayerControlled, sf::Color regionColor, int personalityIndex)
	{
		m_CharacterTitle = title;
		m_KingdomName = kingdomName;
		m_Name = characterName;
		m_OwnedRegionIDs = regionIDs;
		m_CurrentGold = gold;
		m_MaxArmySize = armySize;
		m_IsPlayerControlled = isPlayerControlled;
		m_RegionColor = regionColor;
		m_PersonalityIndex = personalityIndex;
	}

	void Start()
	{
		Time::m_GameDate.SubscribeToMonthChange(std::bind(&CharacterComponent::OnMonthChange, this, std::placeholders::_1));
	}

	void MakePeace()
	{
		m_AtWar = false;
	}

	void OnMonthChange(Date)
	{
		int incomingGold = 0;

		for (auto& regionId : m_OwnedRegionIDs)
		{
			MapRegion region = Map::GetRegionById(regionId);

			incomingGold += region.m_RegionTax;
		}

		incomingGold -= (int)(m_RaisedArmySize * 0.1f);

		m_CurrentGold += incomingGold;
	}
};
#pragma warning(pop)
