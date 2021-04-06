#pragma once
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Game/Systems/Characters/CharacterPool.h"
#include "Game/Data/Character.h"
#include "Game/Data/Types.h"

enum class Title;
struct Date;
class Player;

class CharacterManager
{
public:
	void start();
	void update();
	void render();

	CharacterManager();
	~CharacterManager();

	Character& getPlayerCharacter();
	CharacterID getPlayerCharacterID();
	
	Character& getCharacter(CharacterID id);
	
	void onMonthChange(Date date);

	void constructBuilding(const CharacterID character, const int buildingId, const int regionId, const int buildingSlot);
	void addRegion(const CharacterID characterId, const unsigned int regionId);
	void removeRegion(const CharacterID characterId, const unsigned int regionId);

	static CharacterManager* get();
	
	CharacterID createCharacterWithRandomBirthday(const char* characterName, Title title, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, size_t minAge, size_t maxAge);
	CharacterID createCharacter(const char* characterName, Title title, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, Date birthday);

private:
	CharacterID internalCreateCharacter(Character& character, const char* characterName, Title title, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled);

private:
	static CharacterManager* m_Instance;
	static CharacterID m_CharacterIDs;

	const size_t m_PoolInitSize = 20000;
	const size_t m_PoolGrowSize = 10000;

	Player* m_Player = nullptr;
	CharacterID m_PlayerCharacterID = INVALID_CHARACTER_ID;
	Character* m_PlayerCharacter = nullptr;

	std::vector<Character> m_Characters;
	CharacterPool m_CharacterPool;
};