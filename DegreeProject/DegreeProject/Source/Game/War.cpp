#include "War.h"
#include <Engine\Log.h>
#include "Game/Map/Map.h"
#include "Game/UI/UIManager.h"
#include "AI/AIManager.h"
#include "Game/WarManager.h"

War::War(CharacterID attacker, CharacterID defender, int warGoalRegion, int handle)
{
	m_Attacker = attacker;
	m_Defender = defender;
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
	if (getAttacker() == ID)
	{
		m_AttackerWarscore += warScore;
		m_DefenderWarscore -= warScore;
		if (m_AttackerWarscore >= 100)
		{
			WarManager::get().endWar(m_Handle, getAttacker());
		}
	}

	else if(getDefender() == ID)
 	{
		m_DefenderWarscore += warScore;
		m_AttackerWarscore -= warScore;

		if (m_DefenderWarscore >= 100)
		{
			WarManager::get().endWar(m_Handle, getDefender());
		}
	}
}

void War::endWar(CharacterID winningCharacter)
{
	CharacterManager::get()->getCharacter(m_Attacker);
	CharacterManager::get()->getCharacter(m_Defender);

	unsigned int index = 0;
	for (auto& war : CharacterManager::get()->getCharacter(m_Attacker).m_CurrentWars)
	{
		if (WarManager::get().getWar(war) == this)
		{
			CharacterManager::get()->getCharacter(m_Attacker).m_CurrentWars.erase(CharacterManager::get()->getCharacter(m_Attacker).m_CurrentWars.begin() + index);
			break;
		}
		index++;
	}

	index = 0;

	for (auto& war : CharacterManager::get()->getCharacter(m_Defender).m_CurrentWars)
	{
		if (WarManager::get().getWar(war) == this)
		{
			CharacterManager::get()->getCharacter(m_Defender).m_CurrentWars.erase(CharacterManager::get()->getCharacter(m_Defender).m_CurrentWars.begin() + index);
			break;
		}
		index++;
	}

	handleOccupiedRegions(winningCharacter);

	if (CharacterManager::get()->getCharacter(m_Attacker).m_IsPlayerControlled == false)
	{
		AIManager::get().GetWarmindOfCharacter(m_Attacker).m_PrioritizedWarHandle = -1;
	}

	if (CharacterManager::get()->getCharacter(m_Defender).m_IsPlayerControlled == false)
	{
		AIManager::get().GetWarmindOfCharacter(m_Defender).m_PrioritizedWarHandle = -1;
	}

	LOG_INFO("WAR IS OVER");
}

void War::handleOccupiedRegions(CharacterID winningCharacter)
{
	for (auto& region : m_AttackerOccupiedRegions)
	{
		Map::get().getRegionById(region).m_OccupiedBy = INVALID_CHARACTER_ID;
	}

	if (getAttacker() == winningCharacter)
	{
		CharacterManager::get()->removeRegion(Map::get().getRegionById(m_WargoalRegion).m_OwnerID, m_WargoalRegion);
		CharacterManager::get()->addRegion(winningCharacter, m_WargoalRegion);

		UIManager::get()->AdjustOwnership(winningCharacter, Map::get().getRegionById(m_WargoalRegion).m_OwnerID, Map::get().getRegionById(m_WargoalRegion).m_RegionId);
		Map::get().setRegionColor(Map::get().getRegionById(m_WargoalRegion).m_RegionId, CharacterManager::get()->getCharacter(winningCharacter).m_RegionColor);
	}

	for (auto& region : m_DefenderOccupiedRegions)
	{
		Map::get().getRegionById(region).m_OccupiedBy = INVALID_CHARACTER_ID;
	}
}


bool War::isAttacker(CharacterID ent)
{
	if (ent == m_Attacker)
	{
		return true;
	}

	return false;
}

bool War::isDefender(CharacterID ent)
{
	if (ent == m_Defender)
	{
		return true;
	}

	return false;
}


int War::getHandle()
{
	return m_Handle;
}

CharacterID War::getAttacker()
{
	return m_Attacker;
}

CharacterID War::getDefender()
{
	return m_Defender;
}
