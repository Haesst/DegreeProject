#pragma once
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Game/Components/Player.h"
#include "Game/Data/Character.h"


using CharacterID = size_t;
#define INVALID_CHARACTER_ID = 0;

enum class Title;

class SystemManager
{
public:
	void start();
	void update();
	void render();

	static SystemManager* get();

	CharacterID createCharacter(const char* characterName, Title title, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, int gold, sf::Color color, bool playerControlled);

private:
	static SystemManager* m_Instance;
	static CharacterID m_CharacterIDs;

	Player m_Player;
	Character m_PlayerCharacter;

	std::vector<Character> m_Characters;
};