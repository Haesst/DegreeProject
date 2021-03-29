#include "War.h"

#include "Game/Components/CharacterComponent.h"
#include "Game/Components/Warmind.h"
#include "Game/Components/Unit.h"

War::War(CharacterComponent& attacker, CharacterComponent& defender, int warGoalRegion)
{
	m_Attacker = &attacker;
	m_Defender = &defender;
	m_WargoalRegion = warGoalRegion;
	m_EntityManager = &EntityManager::get();

	m_Units = m_EntityManager->getComponentArray<UnitComponent>();
	m_Warminds = m_EntityManager->getComponentArray<WarmindComponent>();
	m_Characters = m_EntityManager->getComponentArray<CharacterComponent>();
}

bool War::isWinning(EntityID ID, EntityID enemyID)
{
	if (getWarscore(ID) > getWarscore(enemyID))
	{
		return true;
	}

	return false;
}

int War::getWarscore(EntityID ID)
{
	if (getAttacker().getID() == ID)
	{
		return m_AttackerWarscore;
	}

	else if (getDefender().getID() == ID)
	{
		return m_DefenderWarscore;
	}

	return -1;
}

void War::addWarscore(EntityID ID, int warScore)
{
	if (getAttacker().getID() == ID)
	{
		m_AttackerWarscore += warScore;
		if (m_AttackerWarscore > 100)
		{
			endWar(ID);
		}
	}

	else if(getDefender().getID() == ID)
 	{
		m_DefenderWarscore += warScore;

		if (m_DefenderWarscore > 100)
		{
			endWar(ID);
		}
	}
}

void War::endWar(EntityID winningEntity)
{
	m_Attacker->m_AtWar = false;
	m_Defender->m_AtWar = false;

	LOG_INFO("{0}'s war against {1} is over. {2} Won!", m_Attacker->m_Name, m_Defender->m_Name, m_Characters[winningEntity].m_Name);


	if (!m_Attacker->m_IsPlayerControlled)
	{
		m_Attacker->makePeace();
	}

	if (!m_Defender->m_IsPlayerControlled)
	{
		m_Defender->makePeace();

		//WarmindComponent& defenderWarmind = m_Warminds[m_Defender->GetID()];
		//defenderWarmind.m_Active = false;
		//defenderWarmind.m_AtWar = false;
		//m_Defender->m_RaisedArmySize = 0;
		//
		//m_Units[m_Defender->m_UnitEntity].m_Raised = false;
	}
}

bool War::isAttacker(EntityID ent)
{
	if (m_Characters[ent].getID() == m_Attacker->getID())
	{
		return true;
	}

	return false;
}

bool War::isDefender(EntityID ent)
{
	if (m_Characters[ent].getID() == m_Defender->getID())
	{
		return true;
	}

	return false;
}

CharacterComponent& War::getAttacker()
{
	return *m_Attacker;
}

CharacterComponent& War::getDefender()
{
	return *m_Defender;
}
