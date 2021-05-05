#pragma once

#include "Game/Data/Types.h"
#include "Game/Data/Titles.h"
#include "Game/Data/Trait.h"
#include "Game/GameDate.h"

#include <vector>
#include <SFML/Graphics.hpp>

enum class Gender
{
	Male,
	Female
};

struct Character
{
	// Don't add any functions to this. Keep it as POD
	CharacterID m_CharacterID = 0;

	bool m_IsPlayerControlled = false;

	const char* m_Name = "";
	Title m_CharacterTitle = Title::Baron;
	Title m_TitleAtDeath = Title::Baron;
	Date m_Birthday;
	Date m_PregnancyDay;
	CharacterID m_LastChildFather = INVALID_CHARACTER_ID;

	std::string m_KingdomName = "";

	float m_Income = 0.0f;
	float m_CurrentGold = 0.0f;

	CharacterID m_Spouse = INVALID_CHARACTER_ID;
	CharacterID m_Mother = INVALID_CHARACTER_ID;
	CharacterID m_Father = INVALID_CHARACTER_ID;
	std::vector<CharacterID> m_TotalSpouses = std::vector<CharacterID>();
	//std::vector<CharacterID> m_Allies = std::vector<CharacterID>();
	std::vector<CharacterID> m_Children = std::vector<CharacterID>();
	CharacterID m_OldestChild = INVALID_CHARACTER_ID;
	CharacterID m_NextSibling = INVALID_CHARACTER_ID;

	Gender m_Gender = Gender::Male;
	float m_Fertility = -1.f;
	std::vector<unsigned int> m_OwnedRegionIDs;
	std::vector<Trait> m_Traits;

	unsigned int m_MaxArmySize = 0;
	//unsigned int m_RaisedArmySize = 0;
	unsigned int m_CurrentMaxArmySize = 0;

	//std::vector<int> m_CurrentWars = std::vector<int>();
	
	int m_PersonalityIndex = 0; // Change to personality id, move away from indices

	sf::Color m_RegionColor = sf::Color::Black;
	sf::Color m_ColorAtDeath = sf::Color::Black;
	sf::Sprite m_Portrait;
	sf::Texture* m_PortraitTexture;
	const char* m_PortraitPath;

	unsigned int m_DeadlyAge = 50;
	unsigned int m_AgeMax = 100;
	bool m_Dead = false;
	Date m_DeathDate;
	unsigned int m_AgeAtDeath = 0;
	bool m_Inherited = false;
	bool m_RecentlyAtWar = false;  //Flag for raising units when war starts // <- This might not be necessary

	UnitID m_UnitEntity = INVALID_UNIT_ID; // Don't think this will be necessary but still keep it until fixed
};