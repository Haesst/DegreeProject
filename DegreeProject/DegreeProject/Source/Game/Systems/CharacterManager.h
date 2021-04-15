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

	static CharacterManager* get();
	
	CharacterID createCharacterWithRandomBirthday(const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, size_t minAge, size_t maxAge);
	CharacterID createCharacter(const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, Date birthday);
	CharacterID createNewChild(CharacterID motherID);
	void addTrait(CharacterID ID, Trait trait);
	void removeTrait(CharacterID ID, Trait trait);
	bool hasTrait(CharacterID ID, Trait trait);
	int getCharacterOpinion(CharacterID character, CharacterID other);
	Trait getTrait(const char* traitName);

	bool isAlliedWith(CharacterID character, CharacterID other);
	void callAllies(CharacterID character, int war);

	void onAllianceCreated(CharacterID character, CharacterID other);
	void sendAllianceOffer(CharacterID sender, CharacterID reciever);
	void breakAlliance(CharacterID characterID, CharacterID otherID);
	void removeAlly(CharacterID characterID, CharacterID otherID);
	void onWarEnded(CharacterID sender, CharacterID reciever);
	void sendPeaceOffer(CharacterID sender, CharacterID reciever, PeaceType type);
	void killCharacter(CharacterID characterID);

	bool chancePerPercent(float weight);

	bool ownsRegion(CharacterID ID, int regionIndex);

private:
	CharacterID internalCreateCharacter(Character& character, const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled);

	void loadTraits(const char* path);

	void dailyUpdates(Character& character);
	void tryForPregnancy(Character& character);
	void progressPregnancy(Character& character);
	void createUnlandedCharacters(size_t amount);

	void handleInheritance(Character& character);
	void updateTitle(Character& character);
private:
	static CharacterManager* m_Instance;
	static CharacterID m_CharacterIDs;

	mutable std::mutex m_TraitMtx;
	std::vector<Trait> m_Traits;
	const size_t m_PoolInitSize = 20000;
	const size_t m_PoolGrowSize = 10000;
	const size_t m_UnlandedCharactersAtStart = 200;

	const unsigned int m_AgeOfConsent = 16;
	const unsigned int m_FertilityBarrenAge = 60;
	const unsigned int m_FertilityBarrenSmoother = 59;
	const unsigned int m_FertilityCurveSteepness = 75000;
	const unsigned int m_PregnancyDays = 280;
	const unsigned int m_MaxPrematureBirth = 14;
	const unsigned int m_MaxLateBirth = 14;
	const float m_BirthChance = 0.13f;
	const float m_OneOverFertilityCurveSteep = 1.0f / m_FertilityCurveSteepness;

	const int m_SpouseOpinion = 50;
	const int m_ParentOpinion = 80;
	const int m_ChildOpinion = 80;

	const float m_InheritTraitChance = 0.4f;
	const float m_NewTraitChance = 0.05f;

	Player* m_Player = nullptr;
	CharacterID m_PlayerCharacterID = INVALID_CHARACTER_ID;
	Character* m_PlayerCharacter = nullptr;

	std::vector<Character> m_Characters;
	CharacterPool m_CharacterPool;

	Date m_LastDayUpdate;

	float m_MortalityRate = 4.0f;
};