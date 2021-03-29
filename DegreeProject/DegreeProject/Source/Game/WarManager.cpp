#include "WarManager.h"

WarManager* WarManager::m_Instance = nullptr;

int WarManager::createWar(CharacterComponent& attacker, CharacterComponent& defender, int warGoalRegion)
{
	static int handle = m_Warhandle++;

	War war(attacker, defender, warGoalRegion);
	m_Wars.push_back(std::make_pair(handle, war));
	return handle;
}

void WarManager::endWar(int warHandle)
{
	unsigned int index = 0;
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

War* WarManager::getWarAgainst(CharacterComponent& character, CharacterComponent& enemy)
{
	for (auto& pair : m_Wars)
	{
		if (pair.second.getAttacker().getID() == character.getID() && pair.second.getDefender().getID() == enemy.getID())
		{
			return &pair.second;
		}

		else if (pair.second.getDefender().getID() == character.getID() && pair.second.getAttacker().getID() == enemy.getID())
		{
			return &pair.second;
		}
	}

	return nullptr;
}
