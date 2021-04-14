#include "War.h"
#include <Engine\Log.h>
#include "Game/Map/Map.h"
#include "Game/UI/UIManager.h"
#include "AI/AIManager.h"
#include "Game/WarManager.h"

War::War(CharacterID attacker, CharacterID defender, int warGoalRegion, int handle)
{
	m_Attackers.push_back(attacker);
	m_Defenders.push_back(defender);
	m_WargoalRegion = warGoalRegion;
	m_Handle = handle;
}

bool War::isWinning(CharacterID ID, CharacterID enemyID)
{
	if (getWarscore(ID) > getWarscore(enemyID))
	{
		return true;
	}

	return false;
}

int War::getWarscore(CharacterID ID)
{
	if (getAttacker() == ID)
	{
		return m_AttackerWarscore;
	}

	else if (getDefender() == ID)
	{
		return m_DefenderWarscore;
	}

	return -1;
}

void War::addWarscore(CharacterID ID, int warScore)
{
	if (!m_Attackers.empty() && !m_Defenders.empty()) //Todo: real solution
	{
		if (getAttacker() == ID)
		{
			m_AttackerWarscore += warScore;
			m_DefenderWarscore -= warScore;

			if (m_AttackerWarscore >= 100)
			{
				if (!CharacterManager::get()->getCharacter(getAttacker()).m_IsPlayerControlled)
				{
					CharacterManager::get()->sendPeaceOffer(getAttacker(), getDefender(), PeaceType::Enforce_Demands);
					return;
				}
			}
		}

		else if (getDefender() == ID)
		{
			m_DefenderWarscore += warScore;
			m_AttackerWarscore -= warScore;

			if (m_DefenderWarscore >= 100)
			{
				if (!CharacterManager::get()->getCharacter(getDefender()).m_IsPlayerControlled)
				{
					CharacterManager::get()->sendPeaceOffer(getDefender(), getAttacker(), PeaceType::Enforce_Demands);
					return;
				}
			}
		}
	}
}

void War::endWar(CharacterID winningCharacter)
{
	unsigned int index = 0;
	for (auto ID : m_Attackers)
	{
		index = 0;

		for (auto& war : CharacterManager::get()->getCharacter(ID).m_CurrentWars)
		{
			if (WarManager::get().getWar(war) == this)
			{
				CharacterManager::get()->getCharacter(ID).m_CurrentWars.erase(CharacterManager::get()->getCharacter(ID).m_CurrentWars.begin() + index);

				if (CharacterManager::get()->getCharacter(ID).m_IsPlayerControlled == false)
				{
					AIManager::get().getWarmindOfCharacter(ID).m_PrioritizedWarHandle = -1;
				}

				break;
			}
			index++;
		}
	}

	for (auto ID : m_Defenders)
	{
		for (auto& war : CharacterManager::get()->getCharacter(ID).m_CurrentWars)
		{
			index = 0;

			if (WarManager::get().getWar(war) == this)
			{
				CharacterManager::get()->getCharacter(ID).m_CurrentWars.erase(CharacterManager::get()->getCharacter(ID).m_CurrentWars.begin() + index);

				if (CharacterManager::get()->getCharacter(ID).m_IsPlayerControlled == false)
				{
					AIManager::get().getWarmindOfCharacter(ID).m_PrioritizedWarHandle = -1;
				}

				break;
			}
			index++;
		}
	}
	

	handleOccupiedRegions(winningCharacter);

	LOG_INFO("WAR IS OVER");
}

void War::addAttacker(CharacterID character)
{
	for (auto ID : m_Attackers)
	{
		if (ID == character)
		{
			return;
		}
	}

	m_Attackers.push_back(character);
	CharacterManager::get()->getCharacter(character).m_CurrentWars.push_back(m_Handle);
}

void War::addDefender(CharacterID character)
{
	for (auto ID : m_Defenders)
	{
		if (ID == character)
		{
			return;
		}
	}

	m_Defenders.push_back(character);
	CharacterManager::get()->getCharacter(character).m_CurrentWars.push_back(m_Handle);
}

void War::handleOccupiedRegions(CharacterID winningCharacter)
{
	Character& attacker = CharacterManager::get()->getCharacter(m_Attackers[0]);
	Character& defender = CharacterManager::get()->getCharacter(m_Defenders[0]);

	for (auto& region : m_AttackerOccupiedRegions)
	{
		Map::get().getRegionById(region).m_OccupiedBy = INVALID_CHARACTER_ID;
		defender.m_MaxArmySize += Map::get().getRegionById(region).m_ManPower;
	}

	if (getAttacker() == winningCharacter)
	{
		if (winningCharacter != INVALID_CHARACTER_ID)
		{
			CharacterManager::get()->removeRegion(Map::get().getRegionById(m_WargoalRegion).m_OwnerID, m_WargoalRegion);
			CharacterManager::get()->addRegion(winningCharacter, m_WargoalRegion);
			UIManager::get()->AdjustOwnership(winningCharacter, Map::get().getRegionById(m_WargoalRegion).m_OwnerID, Map::get().getRegionById(m_WargoalRegion).m_RegionId);
			Map::get().setRegionColor(Map::get().getRegionById(m_WargoalRegion).m_RegionId, CharacterManager::get()->getCharacter(winningCharacter).m_RegionColor);
		}
	}

	for (auto& region : m_DefenderOccupiedRegions)
	{
		Map::get().getRegionById(region).m_OccupiedBy = INVALID_CHARACTER_ID;
		attacker.m_MaxArmySize += Map::get().getRegionById(region).m_ManPower;
	}
}

bool War::alliesInWar(CharacterID ID)
{
	if (ID == getAttacker())
	{
		if (CharacterManager::get()->getCharacter(ID).m_Allies.size() + 1 == m_Attackers.size())
		{
			return true;
		}
	}

	if (ID == getAttacker())
	{
		if (CharacterManager::get()->getCharacter(ID).m_Allies.size() + 1 == m_Defenders.size())
		{
			return true;
		}
	}

	return false;
}


bool War::isAttacker(CharacterID ent)
{
	for (auto ID : m_Attackers)
	{
		if (ent == ID)
		{
			return true;
		}
	}

	return false;
}

bool War::isDefender(CharacterID ent)
{
	for (auto ID : m_Defenders)
	{
		if (ent == ID)
		{
			return true;
		}
	}

	return false;
}


int War::getHandle()
{
	return m_Handle;
}

CharacterID War::getAttacker()
{
	return m_Attackers[0];
}

CharacterID War::getDefender()
{
	return m_Defenders[0];
}

int War::getWarscoreFrom(CharacterID character)
{
	if (character == getAttacker())
	{
		return m_AttackerWarscore;
	}

	return m_DefenderWarscore;
}
