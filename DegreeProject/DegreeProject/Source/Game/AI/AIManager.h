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
class DiplomacyManager;
class AISettlementManager;
class AIRelationshipManager;
class AIWarManager;

struct Personality;
struct AIData;
enum class PeaceType;



class AIManager
{
public:
	AIManager();
	~AIManager();

	static AIManager& get();
	void onFileChange(std::string path, FileStatus status);
	void loadPersonalities(const char* path);
	WarmindComponent& getWarmindOfCharacter(CharacterID handle);
	AIData& getAIDataofCharacter(int handle);
	void initAI(CharacterID ID);
	Personality* getPersonality(const char* personalityName);
	void deactivateAI(CharacterID ID);
	bool handleRecieveMarriageRequest(CharacterID reciever, CharacterID sender);
	bool handlePeaceRequest(CharacterID sender, CharacterID reciever, PeaceType type);
	bool handleAllianceRequest(CharacterID sender, CharacterID reciever);
	bool handleWarCallRequest(CharacterID sender, CharacterID reciever, int war);
	void update();

	void UpdateWarmind(WarmindComponent& warmind, CharacterManager& characterManager, UnitManager& unitManager, DiplomacyManager& warManager);
	void UpdateAIData(CharacterManager& characterManager, AIData& data, DiplomacyManager& warManager);
	bool isValidWarmind(CharacterID ID);

private:
	int considerPrioritizedWar(WarmindComponent& warmind);
	void considerOrders(WarmindComponent& warmind, Unit& unit, CharacterID target);
	void giveAttackerOrders(WarmindComponent& warmind, CharacterID target, Unit& unit, Unit& enemyUnit);
	void giveDefenderOrders(WarmindComponent& warmind, CharacterID target, Unit& unit, Unit& enemyUnit);
	void giveAllyOrders(WarmindComponent& warmind, Unit& unit, CharacterID mainAlly);

	bool weightedRandom(float weight);

private:
	static AIManager* m_Instance;

	AISettlementManager* m_SettlementManager = nullptr;
	AIRelationshipManager* m_RelationshipManager = nullptr;
	AIWarManager* m_AIWarManager = nullptr;
	DiplomacyManager* m_WarManager = nullptr;
	UnitManager* m_UnitManager = nullptr;

	WarOrders m_Orders;
	std::vector<WarmindComponent> m_Warminds;
	std::vector<AIData> m_AIDatas;
	std::vector<Personality> m_Personalities;
	mutable std::mutex m_PersonalityMtx;

	int m_TickRate1 = 401;
	int m_TickRate2 = 421;
	int m_TickRate3 = 433;
	int m_tickRate4 = 547;
	int m_TickRate5 = 661;
	int m_tickRate6 = 811;
	int m_tickRate7 = 953;
	int m_TickRate8 = 1087;
	std::map<int, std::vector<AIData>> m_TickPrio;
};
