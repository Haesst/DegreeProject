#pragma once
#include <list>
#include "Game/Data/Character.h"
#include <Game\Data\Types.h>
#include "Engine\CharacterManager.h"
struct War
{
	int m_AttackerWarscore = 0;
	int m_DefenderWarscore = 0;

	CharacterID m_Attacker = INVALID_CHARACTER_ID;
	CharacterID m_Defender = INVALID_CHARACTER_ID;

	int m_WargoalRegion = -1;

	War() {};

	War(CharacterID attacker, CharacterID defender, int warGoalRegion, int handle);

	bool isWinning(CharacterID ID, CharacterID enemyID);
	int getWarscore(CharacterID ID);
	void addWarscore(CharacterID ID, int warScore);
	void endWar(CharacterID winningEntity);
	bool isAttacker(CharacterID ent);
	bool isDefender(CharacterID ent);

	int getHandle();
	CharacterID getAttacker();
	CharacterID getDefender();

private:
	int m_Handle = -1;
};
