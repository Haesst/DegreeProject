#pragma once
#include <vector>
#include <map>
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
	bool isValidWar(War& war);

	std::vector<War> getWarsOfCharacter(CharacterID ID);

	std::vector<CharacterID> getOpposingSide(CharacterID ID);

	// Alliances
	void createAlliance(const CharacterID& characterOneID, const CharacterID& characterTwoID);
	void breakAlliance(const CharacterID& characterOneID, const CharacterID& characterTwoID);
	std::vector<CharacterID> getAlliances(const CharacterID& character);

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
	void removeAlly(CharacterID character, CharacterID ally);

	// Alliances
	std::map<CharacterID, std::vector<CharacterID>> m_Alliances;
};
