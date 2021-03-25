#pragma once
#include <list>
#include "ECS/EntityManager.h"

struct WarmindComponent;
struct CharacterComponent;
struct UnitComponent;

struct War
{
	EntityManager* m_EntityManager = &EntityManager::Get();

	int m_AttackerWarscore = 0;
	int m_DefenderWarscore = 0;

	CharacterComponent* m_Attacker = nullptr;
	CharacterComponent* m_Defender = nullptr;

	WarmindComponent* m_Warminds = nullptr;
	CharacterComponent* m_Characters = nullptr;
	UnitComponent* m_Units = nullptr;

	int m_WargoalRegion = -1;

	War() {};

	War(CharacterComponent& attacker, CharacterComponent& defender, int warGoalRegion);

	void AddWarscore(War& war, int warScore, bool attackerWinning);
	void EndWar(War& warToEnd, EntityID winningEntity);
	bool IsAttacker(EntityID ent);
	bool IsDefender(EntityID ent);

	CharacterComponent& GetAttacker(War& war);
	CharacterComponent& GetDefender(War& war);
};
