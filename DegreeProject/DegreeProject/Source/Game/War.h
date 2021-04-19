#pragma once
#include <list>
#include "Game/Data/Character.h"
#include <Game\Data\Types.h>
#include "Game\Systems\CharacterManager.h"

struct War
{
	War(CharacterID attacker, CharacterID defender, int warGoalRegion, int handle);

	bool isWinning(CharacterID ID, CharacterID enemyID);
	int getWarscore(CharacterID ID);
	void addWarscore(CharacterID ID, int warScore);
	void endWar(CharacterID winningEntity);

	void addAttacker(CharacterID character);
	void addDefender(CharacterID character);

	void handleOccupiedRegions(CharacterID winningCharacter);

	bool alliesInWar(CharacterID ID);

	bool isAttacker(CharacterID ent);
	bool isDefender(CharacterID ent);

	int getHandle();

	CharacterID getOpposingForce(CharacterID character);

	CharacterID getAttacker();
	CharacterID getDefender();

public:
	int m_AttackerWarscore = 0;
	int m_DefenderWarscore = 0;

	float warGoalRegionCapturedTimer = 10.0f;
	float warGoalRegionTimerAccu = 0.0f;

	std::vector<int> m_AttackerOccupiedRegions;
	std::vector<int> m_DefenderOccupiedRegions;

	std::vector<CharacterID> m_Attackers = std::vector<CharacterID>();
	std::vector<CharacterID> m_Defenders = std::vector<CharacterID>();

	int m_WargoalRegion = -1;

private:
	int m_Handle = -1;
};
