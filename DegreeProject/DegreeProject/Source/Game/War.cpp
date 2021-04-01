#include "War.h"
#include <Engine\Log.h>


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
			endWar(ID);
		}
	}

	else if(getDefender() == ID)
 	{
		m_DefenderWarscore += warScore;
		m_AttackerWarscore -= warScore;

		if (m_DefenderWarscore >= 100)
		{
			endWar(ID);
		}
	}
}

void War::endWar(CharacterID winningEntity)
{
	//CharacterManager::get()->getCharacter(m_Attacker).m_AtWar = false;
	//CharacterManager::get()->getCharacter(m_Defender).m_AtWar = false;

	//LOG_INFO("{0}'s war against {1} is over. {2} Won!", CharacterManager::get()->getCharacter(m_Attacker), CharacterManager::get()->getCharacter(m_Defender), CharacterManager::get()->getCharacter(winningEntity));
	
	if (!CharacterManager::get()->getCharacter(m_Attacker).m_IsPlayerControlled)
	{
		//Peace
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
