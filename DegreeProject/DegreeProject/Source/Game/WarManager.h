#pragma once
#include <vector>
#include <Game\War.h>

struct Unit;

enum class PeaceType
{
	Enforce_Demands,
	White_Peace,
	Surrender
};

class WarManager
{
public:
	WarManager() {};

	//Returns handle to war
	int createWar(CharacterID attacker, CharacterID defender, int warGoalRegion);
	void endWar(int warHandle, CharacterID winner);
	War* getWar(int handle);
	War* getWarAgainst(CharacterID character, CharacterID enemy);
	bool atWarWith(CharacterID character, CharacterID enemy); 
	void invalidateWarsForRegionOnWonWar(War& wonWar);
	void invalidateWarsForRegion(int regionID);
	bool isEnemyOfEnemy(Unit& unit, Unit& enemyUnit);

	std::vector<War> getWarsOfCharacter(CharacterID ID);

	std::vector<CharacterID> getOpposingSide(CharacterID ID);

	inline static WarManager& get()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = new WarManager();
		}

		return *m_Instance;
	}

private:
	int m_Warhandle = 0;
	std::vector<std::pair<int, War>> m_Wars = std::vector<std::pair<int, War>>();
	static WarManager* m_Instance;
};
