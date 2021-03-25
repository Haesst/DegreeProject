#include "War.h"

#include "Game/Components/CharacterComponent.h"
#include "Game/Components/Warmind.h"
#include "Game/Components/Unit.h"

War::War(CharacterComponent& attacker, CharacterComponent& defender, int warGoalRegion)
{
	m_Attacker = &attacker;
	m_Defender = &defender;
	m_WargoalRegion = warGoalRegion;
	m_EntityManager = &EntityManager::Get();

	m_Units = m_EntityManager->GetComponentArray<UnitComponent>();
	m_Warminds = m_EntityManager->GetComponentArray<WarmindComponent>();
	m_Characters = m_EntityManager->GetComponentArray<CharacterComponent>();
}

void War::AddWarscore(War& war, int warScore, bool attackerWinning)
{
	if (attackerWinning)
	{
		m_AttackerWarscore += warScore;

		if (m_AttackerWarscore > 100)
		{
			EndWar(war, m_Attacker->GetID());
		}
	}

	else
	{
		m_DefenderWarscore += warScore;

		if (m_DefenderWarscore > 100)
		{
			EndWar(war, m_Defender->GetID());
		}
	}
}

void War::EndWar(War& warToEnd, EntityID winningEntity)
{
	m_Attacker->m_AtWar = false;
	m_Defender->m_AtWar = false;

	LOG_INFO("{0}'s war against {1} is over. {2} Won!", m_Attacker->m_Name, m_Defender->m_Name, m_Characters[winningEntity].m_Name);


	if (!m_Attacker->m_IsPlayerControlled)
	{
		m_Attacker->MakePeace();
	}

	if (!m_Defender->m_IsPlayerControlled)
	{
		m_Defender->MakePeace();

		//WarmindComponent& defenderWarmind = m_Warminds[m_Defender->GetID()];
		//defenderWarmind.m_Active = false;
		//defenderWarmind.m_AtWar = false;
		//m_Defender->m_RaisedArmySize = 0;
		//
		//m_Units[m_Defender->m_UnitEntity].m_Raised = false;
	}
}

bool War::IsAttacker(EntityID ent)
{
	if (m_Characters[ent].GetID() == m_Attacker->GetID())
	{
		return true;
	}

	return false;
}

bool War::IsDefender(EntityID ent)
{
	if (m_Characters[ent].GetID() == m_Defender->GetID())
	{
		return true;
	}

	return false;
}

CharacterComponent& War::GetAttacker(War& war)
{
	return *war.m_Attacker;
}

CharacterComponent& War::GetDefender(War& war)
{
	return *war.m_Defender;
}
