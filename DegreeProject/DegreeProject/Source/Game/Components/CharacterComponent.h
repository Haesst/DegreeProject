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
	std::vector<unsigned int> m_OwnedRegionIDs;
	int m_CurrentGold = 0;
	int m_CurrentArmySize = 0;
	bool m_IsPlayerControlled = false;
	sf::Color m_RegionColor = sf::Color::Black;

	int m_PersonalityIndex = 0;

	CharacterComponent() {};

	CharacterComponent(Title title, const char* kingdomName, const char* characterName, std::vector<unsigned int> regionIDs, int gold, int armySize, bool isPlayerControlled, sf::Color regionColor, int personalityIndex)
	{
		m_CharacterTitle = title;
		m_KingdomName = kingdomName;
		m_Name = characterName;
		m_OwnedRegionIDs = regionIDs;
		m_CurrentGold = gold;
		m_CurrentArmySize = armySize;
		m_IsPlayerControlled = isPlayerControlled;
		m_RegionColor = regionColor;
		m_PersonalityIndex = personalityIndex;
	}
};

