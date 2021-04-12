#include "WarManager.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/AI/AIManager.h"
#include "Game/Data/AIData.h"
#include "Game/Data/Character.h"

WarManager* WarManager::m_Instance = nullptr;

int WarManager::createWar(CharacterID attacker, CharacterID defender, int warGoalRegion)
{
	int handle = m_Warhandle++;
	m_Wars.push_back(std::make_pair(handle, War(attacker, defender, warGoalRegion, handle)));
	return handle;
}

void WarManager::endWar(int warHandle, CharacterID winner)
{
	unsigned int index = 0;

	if (winner != INVALID_CHARACTER_ID)
	{
		if (!CharacterManager::get()->getCharacter(getWar(warHandle)->getAttacker()).m_IsPlayerControlled)
		{
			AIManager::get().getAIDataofCharacter(getWar(warHandle)->getAttacker()).m_CurrentAction = Action::NONE;
		}

		if (!CharacterManager::get()->getCharacter(getWar(warHandle)->getDefender()).m_IsPlayerControlled)
		{
			AIManager::get().getAIDataofCharacter(getWar(warHandle)->getDefender()).m_CurrentAction = Action::NONE;
		}
	}
	
	getWar(warHandle)->endWar(winner);

	for (auto& pair : m_Wars)
	{
		if (pair.first == warHandle)
		{
			m_Wars.erase(m_Wars.begin() + index);
			break;
		}

		index++;
	}
}

War* WarManager::getWar(int handle)
{
	for (auto& pair : m_Wars)
	{
		if (pair.first == handle)
		{
			return &pair.second;
		}
	}


	return nullptr;
}

bool WarManager::atWarWith(CharacterID character, CharacterID enemy)
{
	for (auto& war : m_Wars)
	{
		if ((war.second.getAttacker() == character || war.second.getDefender() == character) && 
			(war.second.getAttacker() == enemy || war.second.getDefender() == enemy))
		{
			return true;
		}
	}

	return false;
}

void WarManager::invalidateWarsForRegionOnWonWar(War& wonWar)
{
	for (auto& war : m_Wars)
	{
		if (war.second.m_WargoalRegion == wonWar.m_WargoalRegion)
		{
			if (war.second.getHandle() == wonWar.getHandle())
			{
				continue;
			}

			endWar(war.second.getHandle(), INVALID_CHARACTER_ID);
		}
	}
}

void WarManager::invalidateWarsForRegion(int regionID)
{
	for (auto& war : m_Wars)
	{
		if (war.second.m_WargoalRegion == regionID)
		{
			endWar(war.second.getHandle(), INVALID_CHARACTER_ID);
		}
	}
}

War* WarManager::getWarAgainst(CharacterID character, CharacterID enemy)
{
	for (auto& pair : m_Wars)
	{
		if (pair.second.getAttacker() == character && pair.second.getDefender() == enemy)
		{
			return &pair.second;
		}

		else if (pair.second.getDefender() == character && pair.second.getAttacker() == enemy)
		{
			return &pair.second;
		}
	}

	return nullptr;
}
