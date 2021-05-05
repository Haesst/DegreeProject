#pragma once

#include <vector>

#include "Game/Data/Types.h"
#include "Game/Data/Titles.h"
#include "Game/Data/Character.h"

class CharacterPool;

class CharacterCreator
{
public:
	CharacterCreator();
	CharacterID createRandomUnlandedCharacter(CharacterPool& characterPool, int minAge, int maxAge);
	CharacterID createCharacterWithRandomBirthday(CharacterPool& characterPool, const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, size_t minAge, size_t maxAge);
	CharacterID createCharacter(CharacterPool& characterPool, const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, Date birthday);
	CharacterID createNewChild(CharacterPool& characterPool, CharacterID motherID);
	CharacterID internalCreateCharacter(Character& character, const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled);
	
private:
	void loadAllPortraits();
	void setRandomFemalePortrait(Character& character);
	void setRandomMalePortrait(Character& character);
	const char* m_PortraitPath = "Assets/Graphics/Portraits/";
	const char* m_MalePath = "Male/(";
	const char* m_FemalePath = "Female/(";
	const char* m_PortraitPathEnding = ").png";
	const unsigned int m_NumberOfFemalePortraits = 121;
	const unsigned int m_NumberOfMalePortraits = 151;
	const unsigned int m_PortraitSize = 32;
	std::vector<sf::Texture> m_MalePortraitTextures;
	std::vector<sf::Texture> m_FemalePortraitTextures;
};