#pragma once
#include <vector>
#include "ECS/Component.h"

enum class Title
{
	Emperor,
	King,
	Duke,
	Count,
	Baron
};

struct CharacterComponent : public Component
{
	sf::Sprite m_CharacterPortrait;
	Title m_CharacterTitle = Title::Baron;
	const char* m_KingdomName = "";
	const char* m_Name = "";
	std::vector<int> m_OwnedRegionIDs;
	int m_CurrentGold;
	int m_CurrentArmySize;
	bool m_IsPlayerControlled = false;

	CharacterComponent(Title title, const char* kingdomName, const char* characterName, std::vector<int> regionIDs, int gold, int armySize, bool isPlayerControlled)
	{
		m_CharacterTitle = title;
		m_KingdomName = kingdomName;
		m_Name = characterName;
		m_OwnedRegionIDs = regionIDs;
		m_CurrentGold = gold;
		m_CurrentArmySize = armySize;
		m_IsPlayerControlled = isPlayerControlled;
	}
};

