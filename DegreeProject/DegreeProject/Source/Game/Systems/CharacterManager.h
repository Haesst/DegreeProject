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

	void constructBuilding(CharacterID character, int buildingId, int regionId, int buildingSlot);

	void addRegion(CharacterID characterId, unsigned int regionId);
	void removeRegion(CharacterID characterId, unsigned int regionId);

	static CharacterManager* get();

	CharacterID createCharacter(const char* characterName, Title title, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled);

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