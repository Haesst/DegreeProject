#include "SystemManager.h"

#include "Engine/Log.h"

SystemManager* SystemManager::m_Instance = nullptr;
CharacterID SystemManager::m_CharacterIDs = 1;

SystemManager* SystemManager::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new SystemManager();
	}

	return m_Instance;
}

void SystemManager::start()
{
}

void SystemManager::update()
{
}

void SystemManager::render()
{
}

CharacterID SystemManager::createCharacter(const char* characterName, Title title, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, int gold, sf::Color color, bool playerControlled)
{
	CharacterID id = m_CharacterIDs++;

	Character newChar;

	newChar.m_CharacterID = id;
	newChar.m_CharacterTitle = title;

	newChar.m_OwnedRegionIDs = ownedRegions;
	newChar.m_KingdomName = realmName;

	newChar.m_MaxArmySize = army;
	newChar.m_CurrentMaxArmySize = army;

	newChar.m_CurrentGold = gold;

	newChar.m_IsPlayerControlled = playerControlled;

	if (playerControlled)
	{
		ASSERT(m_PlayerCharacter.m_CharacterID == 0, "No multiplayer game yet, only one player controlled character allowed.");
		m_PlayerCharacter = newChar;
	}

	return id;
}