#pragma once

#include <fstream>
#include <json.hpp>
using json = nlohmann::json;
#include <mutex>

#include "Engine/FileWatcher.h"
#include "Game/HotReloader.h"
#include "Engine/Log.h"
#include <Game\Components\Warmind.h>
#include "WarOrders.h"

class UnitManager;
class WarManager;
struct AIData;
enum class PeaceType;

struct Personality
{
	std::string m_PersonalityName;
	float m_DeclareWarModifier = 0.0f;

	Personality(std::string personalityName, float declareWarModifier)
	{
		m_PersonalityName = personalityName;
		m_DeclareWarModifier = declareWarModifier;
	}
};

class AIManager
{
public:
	AIManager();
	~AIManager();
	static AIManager& get();
	void start();
	void onFileChange(std::string path, FileStatus status);
	void loadPersonalities(const char* path);
	WarmindComponent& getWarmindOfCharacter(int handle);
	AIData& getAIDataofCharacter(int handle);
	void initAI(CharacterID ID);
	void deactivateAI(CharacterID ID);
	bool handleRecieveMarriageRequest(CharacterID reciever, CharacterID sender);
	bool handlePeaceRequest(CharacterID sender, CharacterID reciever, PeaceType type);
	bool handleAllianceRequest(CharacterID sender, CharacterID reciever);
	bool handleWarCallRequest(CharacterID sender, CharacterID reciever, int war);
	void update();

	void UpdateWarmind(WarmindComponent& warmind, CharacterManager& characterManager, UnitManager& unitManager, WarManager& warManager);
	void UpdateAIData(CharacterManager& characterManager, AIData& data, WarManager& warManager);


private:
	float upgradeDecision(CharacterID ID, int& outRegion);
	float warDecision(CharacterID ID);
	float expansionDecision(CharacterID ID);
	float marriageDecision(CharacterID ID, CharacterID spouse);
	float allianceDecision(CharacterID ID, CharacterID potentialAlly);

	int considerPrioritizedWar(WarmindComponent& warmind);
	void considerOrders(WarmindComponent& warmind, Unit& unit, CharacterID target);
	void giveAttackerOrders(WarmindComponent& warmind, CharacterID target, Unit& unit, Unit& enemyUnit);
	void giveDefenderOrders(WarmindComponent& warmind, CharacterID target, Unit& unit, Unit& enemyUnit);
	CharacterID getPotentialSpouse(AIData& data);
	CharacterID getPotentialAlly(AIData& data);

	void warAction(AIData& data);
	void upgradeAction(AIData& data);
	void marriageAction(AIData& data);
	void allianceAction(AIData& data);
	void handleHighestEvaluation(AIData& data);

	bool weightedRandom(float weight);
	bool isValidWarmind(CharacterID ID);


private:
	static AIManager* m_Instance;
	WarOrders m_Orders;
	std::vector<WarmindComponent> m_Warminds;
	std::vector<AIData> m_AIDatas;
	std::vector<Personality> m_Personalities;
	mutable std::mutex m_PersonalityMtx;
	WarManager* m_WarManager = nullptr;
	UnitManager* m_UnitManager = nullptr;

	std::vector<AIData> m_tickPrio1; //Every 5:th tick
	std::vector<AIData> m_tickPrio2; //Every 11:th tick
	std::vector<AIData> m_tickPrio3; //Every 17:th tick.

	int m_tickRate1 = 547;
	int m_tickRate2 = 811;
	int m_tickRate3 = 1087;

	std::map<int, std::vector<AIData>> m_TickPrio;
};
