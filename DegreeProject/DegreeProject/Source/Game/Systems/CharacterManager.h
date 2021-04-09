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

	void marry(CharacterID character, CharacterID spouse);

	static CharacterManager* get();
	
	CharacterID createCharacterWithRandomBirthday(const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, size_t minAge, size_t maxAge);
	CharacterID createCharacter(const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, Date birthday);
	void addTrait(CharacterID ID, Trait trait);
	void removeTrait(CharacterID ID, Trait trait);
	bool hasTrait(CharacterID ID, Trait trait);
	Trait getTrait(const char* traitName);

private:
	CharacterID internalCreateCharacter(Character& character, const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled);

	bool chancePerPercent(float weight);
	void loadTraits(const char* path);

	void dailyUpdates(Character& character);
	void tryForPregnancy(Character& character);
	void progressPregnancy(Character& character);
	void createNewChild(CharacterID motherID);
	void createUnlandedCharacters(size_t amount);

	void handleInheritance(Character& character);
private:
	static CharacterManager* m_Instance;
	static CharacterID m_CharacterIDs;

	mutable std::mutex m_TraitMtx;
	std::vector<Trait> m_Traits;
	const size_t m_PoolInitSize = 20000;
	const size_t m_PoolGrowSize = 10000;
	const size_t m_UnlandedCharactersAtStart = 200;

	const int m_FertilityBarrenAge = 60;
	const int m_FertilityBarrenSmoother = 59;
	const int m_FertilityCurveSteepness = 75000;
	const float m_OneOverFertilityCurveSteep = 1.0f / m_FertilityCurveSteepness;

	Player* m_Player = nullptr;
	CharacterID m_PlayerCharacterID = INVALID_CHARACTER_ID;
	Character* m_PlayerCharacter = nullptr;

	std::vector<Character> m_Characters;
	CharacterPool m_CharacterPool;

	Date m_LastDayUpdate;

	float m_MortalityRate = 4.0f;
};