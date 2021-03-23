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

void War::AddWarscore(int warScore, bool attackerWinning)
{
	if (attackerWinning)
	{
		m_AttackerWarscore += warScore;

		if (m_AttackerWarscore > 100)
		{
			EndWar(/*Attacker*/);
		}
	}

	else
	{
		m_DefenderWarscore += warScore;

		if (m_DefenderWarscore > 100)
		{
			EndWar(/*Defender*/);
		}
	}
}

void War::EndWar()
{
	m_Attacker->m_AtWar = false;
	m_Defender->m_AtWar = false;

	if (!m_Attacker->m_IsPlayerControlled)
	{
		WarmindComponent& attackerWarmind = m_Warminds[m_Attacker->GetID()];
		attackerWarmind.m_Active = false;
		attackerWarmind.m_AtWar = false;
		m_Attacker->m_RaisedArmySize = 0;
		m_Units[attackerWarmind.m_UnitEntity].m_Raised = false;
	}

	if (!m_Defender->m_IsPlayerControlled)
	{
		WarmindComponent& defenderWarmind = m_Warminds[m_Defender->GetID()];
		defenderWarmind.m_Active = false;
		defenderWarmind.m_AtWar = false;
		m_Defender->m_RaisedArmySize = 0;
		m_Units[defenderWarmind.m_UnitEntity].m_Raised = false;
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