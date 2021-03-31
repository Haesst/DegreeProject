#pragma once
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Game/Components/Player.h"
#include "Game/Data/Character.h"


using CharacterID = size_t;
#define INVALID_CHARACTER_ID = 0;

enum class Title;
struct Date;

class CharacterManager
{
public:
	void start();
	void update();
	void render();
	
	void onMonthChange(Date date);

	static CharacterManager* get();

	CharacterID createCharacter(const char* characterName, Title title, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled);

private:
	static CharacterManager* m_Instance;
	static CharacterID m_CharacterIDs;

	Player m_Player;
	Character m_PlayerCharacter;

	std::vector<Character> m_Characters;
};