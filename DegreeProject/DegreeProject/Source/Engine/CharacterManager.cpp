#include "CharacterManager.h"

#include "Engine/Log.h"
#include "Game/GameDate.h"
#include "Game/Map/Map.h"

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

void CharacterManager::start()
{
	Time::m_GameDate.subscribeToMonthChange(std::bind(&CharacterManager::onMonthChange, this, std::placeholders::_1));
}

void CharacterManager::update()
{
}

void CharacterManager::render()
{
}

void CharacterManager::onMonthChange(Date)
{

	//int incomingGold = 0;

	//for (auto& regionId : m_OwnedRegionIDs)
	//{
	//	MapRegion region = Map::get().getRegionById(regionId);

	//	incomingGold += region.m_RegionTax;
	//}

	//incomingGold -= (int)(m_RaisedArmySize * 0.1f);
	//m_Income = incomingGold;
	//m_CurrentGold += incomingGold;

	for (auto& character : m_Characters)
	{
		int incomingGold = 0;

		for (unsigned int id : character.m_OwnedRegionIDs)
		{
			incomingGold += Map::get().getRegionById(id).m_RegionTax;
		}
	}
}

CharacterID CharacterManager::createCharacter(const char* characterName, Title title, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled)
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

	newChar.m_RegionColor = color;

	newChar.m_Name = characterName;

	if (playerControlled)
	{
		ASSERT(m_PlayerCharacter.m_CharacterID == 0, "No multiplayer game yet, only one player controlled character allowed.");
		m_PlayerCharacter = newChar;
	}

	m_Characters.push_back(newChar);

	return id;
}