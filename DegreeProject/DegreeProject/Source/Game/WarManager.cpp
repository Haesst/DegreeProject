#include "WarManager.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/Data/Character.h"

WarManager* WarManager::m_Instance = nullptr;

int WarManager::createWar(CharacterID attacker, CharacterID defender, int warGoalRegion)
{
	static int handle = m_Warhandle++;
	War war(attacker, defender, warGoalRegion, handle);
	m_Wars.push_back(std::make_pair(handle, war));
	return handle;
}

void WarManager::endWar(int warHandle, CharacterID winner)
{
	unsigned int index = 0;

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
