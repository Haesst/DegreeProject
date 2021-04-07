#include "CharacterManager.h"

#include "Engine/Log.h"
#include "Game/Systems/UnitManager.h"
#include "Game/Systems/Characters/CharacterNamePool.h"

#include "Game/GameDate.h"
#include "Game/Map/Map.h"
#include "Game/AI/AIManager.h"
#include "Game/UI/UIManager.h"
#include "Game/Game.h"
#include "Game/Player.h"
#include <random>

#include <fstream>
#include <json.hpp>
using json = nlohmann::json;
#include <mutex>

CharacterManager* CharacterManager::m_Instance = nullptr;
CharacterID CharacterManager::m_CharacterIDs = 1;

CharacterManager* CharacterManager::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new CharacterManager();
	}

	return m_Instance;
}

CharacterID CharacterManager::createCharacterWithRandomBirthday(const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, size_t minAge, size_t maxAge)
{
	ASSERT(minAge <= maxAge, "Minimum age can't be less than maximum age");

	Character& character = m_CharacterPool.Rent();

	character.m_Birthday = Time::m_GameDate.getRandomDate(false, minAge, maxAge);

	return internalCreateCharacter(character, characterName, title, gender, ownedRegions, realmName, army, gold, color, playerControlled);
}

void CharacterManager::loadTraits(const char* path)
{
	std::ifstream file(path);
	json j;
	file >> j;

	m_TraitMtx.lock();
	m_Traits.clear();

	for (auto& element : j)
	{
		std::string traitName = element["debugName"].get<std::string>();

		m_Traits.push_back(traitName);
	}

	m_TraitMtx.unlock();
}

void CharacterManager::createNewChild()
{
	bool male = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) <= 0.5f;
	Gender gender = male ? Gender::Male : Gender::Female;
	char* name = male ? CharacterNamePool::getMaleName() : CharacterNamePool::getFemaleName();
	std::vector<unsigned int> regions = std::vector<unsigned int>();
	createCharacter(name, Title::Unlanded, gender, regions, "NONAME", 0, 0, sf::Color::Black, false, Time::m_GameDate.m_Date);
}

void CharacterManager::createUnlandedCharacters(size_t amount)
{
	for (size_t i = 0; i < amount; ++i)
	{
		bool male = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) <= 0.5f;
		Gender gender = male ? Gender::Male : Gender::Female;
		char* name = male ? CharacterNamePool::getMaleName() : CharacterNamePool::getFemaleName();
		std::vector<unsigned int> regions = std::vector<unsigned int>();

		createCharacterWithRandomBirthday(name, Title::Unlanded, gender, regions, "NONAME", 0, 0, sf::Color::White, false, 1, 72);
		LOG_INFO("Created character {0}", name);
	}
}

CharacterID CharacterManager::createCharacter(const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, Date birthday)
{
	Character& character = m_CharacterPool.Rent();

	character.m_Birthday = birthday;

	return internalCreateCharacter(character, characterName, title, gender, ownedRegions, realmName, army, gold, color, playerControlled);
}

void CharacterManager::addTrait(CharacterID ID, Trait trait)
{
	for (auto& t : getCharacter(ID).m_Traits)
	{
		if (t.m_TraitName == trait.m_TraitName)
		{
			return;
		}
	}

	getCharacter(ID).m_Traits.push_back(trait);
}

void CharacterManager::removeTrait(CharacterID ID, Trait trait)
{
	unsigned int index = 0;
	for (auto& t : getCharacter(ID).m_Traits)
	{
		if (t.m_TraitName == trait.m_TraitName)
		{
			getCharacter(ID).m_Traits.erase(getCharacter(ID).m_Traits.begin() + index);
			break;
		}

		index++;
	}
}

bool CharacterManager::hasTrait(CharacterID ID, Trait trait)
{
	for (auto& t : getCharacter(ID).m_Traits)
	{
		if (t.m_TraitName == trait.m_TraitName)
		{
			return true;
		}
	}

	return false;
}

Trait CharacterManager::getTrait(const char* traitName)
{
	for (auto trait : m_Traits)
	{
		if (trait.m_TraitName == traitName)
		{
			return trait;
		}
	}

	return Trait("INVALID");
}

void CharacterManager::start()
{
	Time::m_GameDate.subscribeToMonthChange(std::bind(&CharacterManager::onMonthChange, this, std::placeholders::_1));
	loadTraits("Assets\\Data\\Traits.json");

	for (auto& character : m_Characters)
	{
		for (int id : character.m_OwnedRegionIDs)
		{
			Map::get().setRegionColor(id, character.m_RegionColor);
			Map::get().getRegionById(id).m_OwnerID = character.m_CharacterID;
		}
	}

	createUnlandedCharacters(m_UnlandedCharactersAtStart);
}

void CharacterManager::update()
{
	m_Player->update();
}

void CharacterManager::render()
{
	m_Player->render();
}

CharacterManager::CharacterManager()
	: m_CharacterPool(CharacterPool(m_PoolInitSize, m_PoolGrowSize))
{
}

CharacterManager::~CharacterManager()
{
	delete m_Player;
}

Character& CharacterManager::getPlayerCharacter()
{
	ASSERT(m_PlayerCharacterID != INVALID_CHARACTER_ID, "Player not set");
	return getCharacter(m_PlayerCharacterID);
}

CharacterID CharacterManager::getPlayerCharacterID()
{
	ASSERT(m_PlayerCharacterID != INVALID_CHARACTER_ID, "Player not set");
	return m_PlayerCharacterID;
}

#pragma warning(push)
#pragma warning(disable: 4239 4172)
Character& CharacterManager::getCharacter(CharacterID id)
{
	ASSERT(id != INVALID_CHARACTER_ID, "Invalid character id requested");

	for (auto& character : m_Characters)
	{
		if (character.m_CharacterID == id)
		{
			return character;
		}
	}

	ASSERT(false, "Character id not found");
	return Character();
}
#pragma warning(pop)

void CharacterManager::onMonthChange(Date)
{
	for (auto& character : m_Characters)
	{
		if (hasTrait(character.m_CharacterID, getTrait("Pregnant")))
		{
			Date currentDate = Time::m_GameDate.m_Date;

			if ((currentDate.m_Month - character.m_PregnancyDay.m_Month) >= 9)
			{
				//Give birth
				createNewChild();

				removeTrait(character.m_CharacterID, getTrait("Pregnant"));
			}
		}

		float incomingGold = 0;

		for (unsigned int id : character.m_OwnedRegionIDs)
		{
			incomingGold += (float)Map::get().getRegionById(id).m_RegionTax;
		}

		incomingGold -= (character.m_RaisedArmySize * 0.1f); // Todo: Declare army cost somewhere
		character.m_Income = incomingGold; // Todo: Change to prediction for upcoming month instead of showing last month.
		character.m_CurrentGold += incomingGold;

		if (character.m_Spouse != INVALID_CHARACTER_ID)
		{
			float fertilityWeight = character.m_Fertility + getCharacter(character.m_Spouse).m_Fertility;
			bool rand = weightedRandom(fertilityWeight);

			if (rand)
			{
				//Mark character as pregnant.
				addTrait(character.m_Spouse, getTrait("Pregnant"));
				getCharacter(character.m_Spouse).m_PregnancyDay = Time::m_GameDate.m_Date;
			}
		}

		if (Time::m_GameDate.getAge(character.m_Birthday) >= character.m_DieAge && character.m_CharacterTitle != Title::Unlanded)
		{
			character.m_Dead = true;
			float giveawayGold = character.m_CurrentGold / character.m_OwnedRegionIDs.size();
			unsigned int giveawayArmy = character.m_MaxArmySize / character.m_OwnedRegionIDs.size();
			character.m_CurrentGold = 0;
			character.m_MaxArmySize = 0;
			character.m_RaisedArmySize = 0;
			for (unsigned int ownedRegionID : character.m_OwnedRegionIDs)
			{
				for (Character& otherCharacter : m_Characters)
				{
					if (otherCharacter.m_CharacterTitle == Title::Unlanded && !otherCharacter.m_Dead)
					{
						addRegion(otherCharacter.m_CharacterID, ownedRegionID);
						MapRegion& mapRegion = Map::get().getRegionById(ownedRegionID);
						mapRegion.m_OwnerID = otherCharacter.m_CharacterID;
						otherCharacter.m_CurrentGold = giveawayGold;
						otherCharacter.m_MaxArmySize = giveawayArmy;
						otherCharacter.m_RegionColor = sf::Color((sf::Uint8)std::rand(), (sf::Uint8)std::rand(), (sf::Uint8)std::rand());
						otherCharacter.m_KingdomName = mapRegion.m_RegionName.c_str();
						otherCharacter.m_CharacterTitle = Title::Baron;
						Map::get().setRegionColor(ownedRegionID, otherCharacter.m_RegionColor);
						UIManager::get()->createUITextElement(Game::m_UIFont, otherCharacter.m_CharacterID, otherCharacter.m_KingdomName, otherCharacter.m_OwnedRegionIDs);
						UIManager::get()->AdjustOwnership(otherCharacter.m_CharacterID, character.m_CharacterID, ownedRegionID);
						break;
					}
				}
			}
			character.m_OwnedRegionIDs.clear();
			character.m_CharacterTitle = Title::Unlanded;
		}
	}
}

bool CharacterManager::weightedRandom(float weight)
{
	float f = rand() * 1.0f / RAND_MAX;
	float vv = weight / 10.0f;
	return f < vv;
}


void CharacterManager::constructBuilding(const CharacterID characterId, const int buildingId, const int regionId, const int buildingSlot)
{
	Character& character = getCharacter(characterId);
	Building building = GameData::m_Buildings[buildingId];
	MapRegion& region = Map::get().getRegionById(regionId);

	if (building.m_Cost > character.m_CurrentGold)
	{
		return;
	}

	if (region.m_OwnerID != characterId)
	{
		return;
	}

	if (region.m_BuildingSlots[buildingSlot].m_BuildingId != -1)
	{
		return;
	}

	Map::get().startConstructionOfBuilding(buildingId, buildingSlot, regionId);
	character.m_CurrentGold -= building.m_Cost;
}

void CharacterManager::addRegion(const CharacterID characterId, const unsigned int regionId)
{
	getCharacter(characterId).m_OwnedRegionIDs.push_back(regionId);
}

void CharacterManager::removeRegion(const CharacterID characterId, const unsigned int regionId)
{
	Character& character = getCharacter(characterId);

	for (size_t i = 0; i < character.m_OwnedRegionIDs.size(); ++i)
	{
		if (character.m_OwnedRegionIDs[i] == regionId)
		{
			character.m_OwnedRegionIDs.erase(character.m_OwnedRegionIDs.begin() + i);
			break;
		}
	}
}

void CharacterManager::marry(CharacterID character, CharacterID spouse)
{
	CharacterManager* characterManager = CharacterManager::get();

	if (characterManager->getCharacter(spouse).m_Spouse != INVALID_CHARACTER_ID)
	{
		return;
	}

	if (characterManager->getCharacter(character).m_Spouse != INVALID_CHARACTER_ID)
	{
		return;
	}

	if (!characterManager->getCharacter(spouse).m_IsPlayerControlled)
	{
		if (AIManager::get().handleRecieveMarriageRequest(spouse, character))
		{
			characterManager->getCharacter(character).m_Spouse = spouse;
			characterManager->getCharacter(spouse).m_Spouse = character;
		}
	}

	else
	{
		characterManager->getCharacter(character).m_Spouse = spouse;
		characterManager->getCharacter(spouse).m_Spouse = character;
	}
}

CharacterID CharacterManager::internalCreateCharacter(Character& character, const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled)
{
	CharacterID id = m_CharacterIDs++;

	character.m_CharacterID = id;
	character.m_CharacterTitle = title;

	character.m_OwnedRegionIDs = ownedRegions;
	character.m_KingdomName = realmName;

	character.m_MaxArmySize = army;
	character.m_CurrentMaxArmySize = army;

	//Generate random number between 0 - 1, will be used as percent
	character.m_Fertility = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	character.m_Gender = gender;

	character.m_CurrentGold = gold;

	character.m_IsPlayerControlled = playerControlled;

	character.m_RegionColor = color;

	character.m_Name = characterName;
	character.m_UnitEntity = UnitManager::get().addUnit(id, army);
	m_Characters.push_back(character);

	if (playerControlled)
	{
		ASSERT(m_PlayerCharacter == nullptr, "No multiplayer game yet, only one player controlled character allowed.");

		m_PlayerCharacter = &getCharacter(id);
		m_PlayerCharacterID = id;

		m_Player = new Player(id);
	}

	else
	{
		AIManager::get().initAI(id);
	}

	return id;
}