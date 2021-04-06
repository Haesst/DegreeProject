#include "CharacterManager.h"

#include "Engine/Log.h"
#include "Game/Systems/UnitManager.h"

#include "Game/GameDate.h"
#include "Game/Map/Map.h"
#include "Game/AI/AIManager.h"
#include "Game/Player.h"

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

CharacterID CharacterManager::createCharacterWithRandomBirthday(const char* characterName, Title title, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, size_t minAge, size_t maxAge)
{
	ASSERT(minAge <= maxAge, "Minimum age can't be less than maximum age");

	Character& character = m_CharacterPool.Rent();

	character.m_Birthday = Time::m_GameDate.getRandomDate(false, minAge, maxAge);

	return internalCreateCharacter(character, characterName, title, ownedRegions, realmName, army, gold, color, playerControlled);
}

CharacterID CharacterManager::createCharacter(const char* characterName, Title title, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, Date birthday)
{
	Character& character = m_CharacterPool.Rent();

	character.m_Birthday = birthday;

	return internalCreateCharacter(character, characterName, title, ownedRegions, realmName, army, gold, color, playerControlled);
}

void CharacterManager::start()
{
	Time::m_GameDate.subscribeToMonthChange(std::bind(&CharacterManager::onMonthChange, this, std::placeholders::_1));

	for (auto& character : m_Characters)
	{
		for (int id : character.m_OwnedRegionIDs)
		{
			Map::get().setRegionColor(id, character.m_RegionColor);
			Map::get().getRegionById(id).m_OwnerID = character.m_CharacterID;
		}
	}
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
{}

CharacterManager::~CharacterManager()
{
	delete m_Player;
}

Character& CharacterManager::getPlayerCharacter()
{
	ASSERT(m_PlayerCharacterID != INVALID_CHARACTER_ID, "Player not set");
	return *m_PlayerCharacter;
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
		float incomingGold = 0;

		for (unsigned int id : character.m_OwnedRegionIDs)
		{
			incomingGold += (float)Map::get().getRegionById(id).m_RegionTax;
		}

		incomingGold -= (character.m_RaisedArmySize * 0.1f); // Todo: Declare armycost somewhere
		character.m_Income = incomingGold; // Todo: Change to prediction for upcoming month instead of showing last month.
		character.m_CurrentGold += incomingGold;
	}
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

CharacterID CharacterManager::internalCreateCharacter(Character& character, const char* characterName, Title title, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled)
{
	CharacterID id = m_CharacterIDs++;

	character.m_CharacterID = id;
	character.m_CharacterTitle = title;

	character.m_OwnedRegionIDs = ownedRegions;
	character.m_KingdomName = realmName;

	character.m_MaxArmySize = army;
	character.m_CurrentMaxArmySize = army;

	character.m_CurrentGold = gold;

	character.m_IsPlayerControlled = playerControlled;

	character.m_RegionColor = color;

	character.m_Name = characterName;
	character.m_UnitEntity = UnitManager::get().addUnit(id, army);
	m_Characters.push_back(character);

	if (playerControlled)
	{
		ASSERT(m_PlayerCharacter == nullptr, "No multiplayer game yet, only one player controlled character allowed.");

		m_PlayerCharacter = &m_Characters.back();
		m_PlayerCharacterID = id;

		m_Player = new Player(id);
	}

	else
	{
		AIManager::get().initAI(id);
	}

	return id;
}