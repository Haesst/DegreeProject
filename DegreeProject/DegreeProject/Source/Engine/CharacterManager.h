#pragma once
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Game/Components/Player.h"
#include "Game/Data/Character.h"

enum class Title;
struct Date;

using CharacterID = size_t;
const int INVALID_CHARACTER_ID = 0;


class CharacterManager
{
public:
	void start();
	void update();
	void render();

	Character& getPlayerCharacter();
	CharacterID getPlayerCharacterID();
	
	Character& getCharacter(CharacterID id);
	
	void onMonthChange(Date date);

	void constructBuilding(CharacterID character, int buildingId, int regionId, int buildingSlot);

	void addRegion(CharacterID characterId, unsigned int regionId);
	void removeRegion(CharacterID characterId, unsigned int regionId);

	static CharacterManager* get();

	CharacterID createCharacter(const char* characterName, Title title, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled);

private:
	static CharacterManager* m_Instance;
	static CharacterID m_CharacterIDs;

	Player* m_Player;
	CharacterID m_PlayerCharacterID = INVALID_CHARACTER_ID;
	Character* m_PlayerCharacter;

	std::vector<Character> m_Characters;
};