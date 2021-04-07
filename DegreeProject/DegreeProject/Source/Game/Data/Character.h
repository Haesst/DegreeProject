#pragma once

#include "Game/Data/Types.h"
#include "Game/Data/Titles.h"
#include "Game/GameDate.h"

#include <vector>
#include <SFML/Graphics.hpp>

enum class Gender
{
	Male,
	Female
};

struct Trait
{
	std::string m_TraitName;

	Trait(std::string traitName)
	{
		m_TraitName = traitName;
	}
};

struct Character
{
	// Don't add any functions to this. Keep it as POD
	CharacterID m_CharacterID = 0;

	bool m_IsPlayerControlled = false;

	const char* m_Name = "";
	Title m_CharacterTitle = Title::Baron;
	Date m_Birthday;
	Date m_PregnancyDay;

	const char* m_KingdomName = "";

	float m_Income = 0.0f;
	float m_CurrentGold = 0.0f;

	CharacterID m_Spouse = INVALID_CHARACTER_ID;
	CharacterID m_Mother = INVALID_CHARACTER_ID;
	CharacterID m_Father = INVALID_CHARACTER_ID;
	CharacterID m_Child = INVALID_CHARACTER_ID;

	Gender m_Gender = Gender::Male;
	float m_Fertility = -1.f;
	std::vector<unsigned int> m_OwnedRegionIDs;
	std::vector<Trait> m_Traits;

	unsigned int m_MaxArmySize = 0;
	unsigned int m_RaisedArmySize = 0;
	unsigned int m_CurrentMaxArmySize = 0;

	std::vector<int> m_CurrentWars = std::vector<int>();
	
	int m_PersonalityIndex = 0; // Change to personality id, move away from indices

	sf::Color m_RegionColor = sf::Color::Black;
	sf::Sprite m_CharacterPortrait;

	unsigned int m_DieAge = 60;
	bool m_Dead = false;

	bool m_RecentlyAtWar = false;  //Flag for raising units when war starts // <- This might not be necessary

	UnitID m_UnitEntity = INVALID_UNIT_ID; // Don't think this will be necessary but still keep it until fixed
};