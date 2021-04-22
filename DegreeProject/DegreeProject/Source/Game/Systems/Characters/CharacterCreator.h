#pragma once

#include <vector>

#include "Game/Data/Types.h"
#include "Game/Data/Titles.h"
#include "Game/Data/Character.h"

class CharacterPool;

class CharacterCreator
{
public:
	CharacterID createRandomUnlandedCharacter(CharacterPool& characterPool, int minAge, int maxAge);
	CharacterID createCharacterWithRandomBirthday(CharacterPool& characterPool, const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, size_t minAge, size_t maxAge);
	CharacterID createCharacter(CharacterPool& characterPool, const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, Date birthday);
	CharacterID createNewChild(CharacterPool& characterPool, CharacterID motherID);
	CharacterID internalCreateCharacter(Character& character, const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled);
};