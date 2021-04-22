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
	int getWarHandleAgainst(CharacterID character, CharacterID enemy);
	bool atWarWith(CharacterID character, CharacterID enemy); 
	void invalidateWarsForRegionOnWonWar(War& wonWar);
	void invalidateWarsForRegion(int regionID);
	bool isEnemyOfEnemy(Unit& unit, Unit& enemyUnit);
	bool isValidWar(War& war);
	void update();

	bool isWinning(int warHandle, CharacterID ID, CharacterID enemyID);
	int getWarscore(int warHandle, CharacterID ID);
	void addWarscore(int warHandle, CharacterID ID, int warScore);

	void addAttacker(int warHandle, CharacterID character);
	void addDefender(int warHandle, CharacterID character);

	void handleOccupiedRegions(int warHandle, CharacterID winningCharacter);
	
	bool alliesInWar(int warHandle, CharacterID ID);
	bool isAllyOf(int warHandle, CharacterID potentialAlly, CharacterID allyOf);

	bool isAttacker(int warHandle, CharacterID ent);
	bool isDefender(int warHandle, CharacterID ent);

	CharacterID getOpposingForce(int warHandle, CharacterID ID);
	CharacterID getAttacker(int warHandle);
	CharacterID getDefender(int warHandle);

	std::vector<int> getWarsForRegion(int regionID);
	std::vector<int> getWarHandlesOfCharacter(CharacterID ID);
	std::vector<CharacterID> getOpposingSide(CharacterID ID);

	void removeAllyFromWar(CharacterID ally, int warHandle);

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
