#pragma once
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Game/Systems/Characters/CharacterPool.h"
#include "Game/Systems/Characters/CharacterCreator.h"
#include "Game/Data/Character.h"
#include "Game/Data/Types.h"

enum class Title;
struct Date;
class Player;
enum class PeaceType;


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

	void onMarriage(CharacterID sender, CharacterID reciever);
	void marry(CharacterID character, CharacterID spouse);

	static CharacterManager& get();

	CharacterID getNewCharacterID();
	void setPlayerCharacter(Character& character);
	void addNewCharacter(Character& character);
	
	CharacterID createCharacterWithRandomBirthday(const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, size_t minAge, size_t maxAge);
	CharacterID createCharacter(const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, Date birthday);
	CharacterID createNewChild(CharacterID motherID);
	void addTrait(CharacterID ID, Trait trait);
	void removeTrait(CharacterID ID, Trait trait);
	bool hasTrait(CharacterID ID, Trait trait);
	int getCharacterOpinion(CharacterID character, CharacterID other);
	Trait getTrait(const char* traitName);
	std::vector<Trait> getAllTraits() { return m_Traits; }

	bool isAlliedWith(CharacterID character, CharacterID other);
	void callAllies(CharacterID character, int war);

	void sendAllianceOffer(CharacterID sender, CharacterID reciever);
	void onWarEnded(CharacterID sender, CharacterID reciever);
	void sendPeaceOffer(CharacterID sender, CharacterID reciever, PeaceType type);
	void killCharacter(CharacterID characterID);
	std::vector<CharacterID> getAliveChildren(CharacterID characterID);

	CharacterID getUnlandedCharacterOfGender(Gender gender);

	bool chancePerPercent(float weight);

	bool ownsRegion(CharacterID ID, int regionIndex);

private:
	//CharacterID internalCreateCharacter(Character& character, const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled);

	void loadTraits(const char* path);

	void dailyUpdates(Character& character);
	void tryForPregnancy(Character& character);
	void progressPregnancy(Character& character);
	void createUnlandedCharacters(size_t amount);

	void handleInheritance(Character& character);
	void updateTitleAndUIText(Character& character);

private:
	static CharacterManager* m_Instance;
	static CharacterID m_CharacterIDs;

	mutable std::mutex m_TraitMtx;
	std::vector<Trait> m_Traits;

	Player* m_Player = nullptr;
	CharacterID m_PlayerCharacterID = INVALID_CHARACTER_ID;
	Character* m_PlayerCharacter = nullptr;
	CharacterCreator m_CharacterCreator;

	std::vector<Character> m_Characters;
	CharacterPool m_CharacterPool;

	Date m_LastDayUpdate;

	float m_MortalityRate = 0.13f;
};