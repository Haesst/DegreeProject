#pragma once

#include "Game/Data/Titles.h"
#include "ECS/Types.h"

#include <vector>
#include <SFML/Graphics.hpp>

using CharacterID = size_t;

struct Character
{
	// Don't add any functions to this. Keep it as POD
	CharacterID m_CharacterID = 0;

	bool m_IsPlayerControlled = false;

	const char* m_Name = "";
	Title m_CharacterTitle = Title::Baron;

	const char* m_KingdomName = "";

	int m_Income = 0;
	int m_CurrentGold = 0;

	std::vector<unsigned int> m_OwnedRegionIDs;

	unsigned int m_MaxArmySize = 0;
	unsigned int m_RaisedArmySize = 0;
	unsigned int m_CurrentMaxArmySize = 0;

	std::vector<int> m_CurrentWars = std::vector<int>();
	
	int m_PersonalityIndex = 0; // Change to personality id, move away from indicies

	sf::Color m_RegionColor = sf::Color::Black;
	sf::Sprite m_CharacterPortrait;


	bool m_RecentlyAtWar = false;  //Flag for raising units when war starts // <- This might not be necessary

	EntityID m_TextUI = (size_t)INT_MAX; // Moving away from the old entity system keeping EntityID until UI is moved
	EntityID m_UnitEntity = INT_MAX; // Don't think this will be necessary but still keep it until fixed
};