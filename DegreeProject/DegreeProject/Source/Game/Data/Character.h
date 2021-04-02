#pragma once

#include "Game/Data/Types.h"
#include "Game/Data/Titles.h"

#include <vector>
#include <SFML/Graphics.hpp>

struct Character
{
	// Don't add any functions to this. Keep it as POD
	CharacterID m_CharacterID = 0;

	bool m_IsPlayerControlled = false;

	const char* m_Name = "";
	Title m_CharacterTitle = Title::Baron;

	const char* m_KingdomName = "";

	float m_Income = 0.0f;
	float m_CurrentGold = 0.0f;

	std::vector<unsigned int> m_OwnedRegionIDs;

	unsigned int m_MaxArmySize = 0;
	unsigned int m_RaisedArmySize = 0;
	unsigned int m_CurrentMaxArmySize = 0;

	std::vector<int> m_CurrentWars = std::vector<int>();
	
	int m_PersonalityIndex = 0; // Change to personality id, move away from indicies

	sf::Color m_RegionColor = sf::Color::Black;
	sf::Sprite m_CharacterPortrait;


	bool m_RecentlyAtWar = false;  //Flag for raising units when war starts // <- This might not be necessary

	CharacterID m_TextUI = (size_t)INT_MAX; // Moving away from the old entity system keeping EntityID until UI is moved
	UnitID m_UnitEntity = INT_MAX; // Don't think this will be necessary but still keep it until fixed
};