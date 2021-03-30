#pragma once
#include <list>
#include "ECS/EntityManager.h"

struct WarmindComponent;
struct CharacterComponent;
struct UnitComponent;

struct War
{
	EntityManager* m_EntityManager = &EntityManager::get();

	int m_AttackerWarscore = 0;
	int m_DefenderWarscore = 0;

	CharacterComponent* m_Attacker = nullptr;
	CharacterComponent* m_Defender = nullptr;

	WarmindComponent* m_Warminds = nullptr;
	CharacterComponent* m_Characters = nullptr;
	UnitComponent* m_Units = nullptr;

	int m_WargoalRegion = -1;

	War() {};

	War(CharacterComponent& attacker, CharacterComponent& defender, int warGoalRegion, int handle);

	bool isWinning(EntityID ID, EntityID enemyID);
	int getWarscore(EntityID ID);
	void addWarscore(EntityID ID, int warScore);
	void endWar(EntityID winningEntity);
	bool isAttacker(EntityID ent);
	bool isDefender(EntityID ent);

	int getHandle();

	CharacterComponent& getAttacker();
	CharacterComponent& getDefender();

private:
	int m_Handle = -1;
};
