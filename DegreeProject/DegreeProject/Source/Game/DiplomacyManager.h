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

struct Truce
{
	Truce(Date startDate, CharacterID char1, CharacterID char2, int handle)
	{
		m_StartDate = startDate;
		m_Char1 = char1;
		m_Char2 = char2;
		m_Handle = handle;
	}

	int m_Handle = -1;
	Date m_StartDate;
	CharacterID m_Char1;
	CharacterID m_Char2;
};

class DiplomacyManager
{
public:
	DiplomacyManager() {};

	//Returns handle to war
	int createWar(CharacterID attacker, CharacterID defender, int warGoalRegion);
	void endWar(int warHandle, CharacterID winner);
	void start();
	War* getWar(int handle);
	War* getWarAgainst(CharacterID character, CharacterID enemy);

	int getWarHandleAgainst(CharacterID character, CharacterID enemy);
	int getWarscore(int warHandle, CharacterID ID);

	bool atWarWith(CharacterID character, CharacterID enemy); 
	bool isEnemyOfEnemy(Unit& unit, Unit& enemyUnit);
	bool isValidWar(int warHandle);
	bool isWinning(int warHandle, CharacterID ID, CharacterID enemyID);
	bool alliesInWar(int warHandle, CharacterID ID);
	bool isAllyOf(int warHandle, CharacterID potentialAlly, CharacterID allyOf);
	bool isAttacker(int warHandle, CharacterID ent);
	bool isDefender(int warHandle, CharacterID ent);
	bool hasTruce(CharacterID char1, CharacterID char2);

	void invalidateWarsForRegionOnWonWar(War& wonWar);
	void invalidateWarsForRegion(int regionID);

	void invalidateWar(int warHandle);
	void InvalidateWarsOfCharacter(CharacterID ID);

	void update();
	void addWarscore(int warHandle, CharacterID ID, int warScore);
	void addAttacker(int warHandle, CharacterID character);
	void addDefender(int warHandle, CharacterID character);
	void handleOccupiedRegions(int warHandle, CharacterID winningCharacter);
	void removeAllyFromWar(CharacterID ally, int warHandle);

	CharacterID getOpposingForce(int warHandle, CharacterID ID);
	CharacterID getAttacker(int warHandle);
	CharacterID getDefender(int warHandle);

	std::vector<int> getWarsForRegion(int regionID);
	std::vector<int> getWarHandlesOfCharacter(CharacterID ID);

	std::vector<CharacterID> getOpposingSide(CharacterID ID);

	// Alliances
	void createAlliance(const CharacterID& characterOneID, const CharacterID& characterTwoID);
	void breakAlliance(const CharacterID& characterOneID, const CharacterID& characterTwoID);
	std::vector<CharacterID> getAlliances(const CharacterID& character);

	inline static DiplomacyManager& get()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = new DiplomacyManager();
		}

		return *m_Instance;
	}

private:
	void makeTruce(CharacterID char1, CharacterID char2);
	void endTruce(int truceHandle);
	void eraseWar(int handle);
	void onMonthChange(Date currentDate);

private:

	int m_TruceHandle = 0;
	int m_Warhandle = 0;
	
	std::vector<std::pair<int, War>> m_Wars = std::vector<std::pair<int, War>>();
	
	static DiplomacyManager* m_Instance;
	
	void removeAlly(CharacterID character, CharacterID ally);

	// Alliances
	std::map<CharacterID, std::vector<CharacterID>> m_Alliances;
	std::vector<Truce> m_ActiveTruces;
};
