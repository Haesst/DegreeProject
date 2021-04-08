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
	WarmindComponent& GetWarmindOfCharacter(int handle);
	AIData& getAIDataofCharacter(int handle);
	void addWarmind(CharacterID ID);
	void initAI(CharacterID ID);
	bool handleRecieveMarriageRequest(CharacterID reciever, CharacterID sender);
	void update();


private:
	float upgradeDecision(CharacterID ID, int& outRegion);
	float warDecision(CharacterID ID);
	float expansionDecision(CharacterID ID);
	float marriageDecision(CharacterID ID, CharacterID spouse);

	int considerPrioritizedWar(WarmindComponent& warmind);
	void considerOrders(WarmindComponent& warmind, Unit& unit, CharacterID target);
	void GiveAttackerOrders(WarmindComponent& warmind, CharacterID target, Unit& unit, Unit& enemyUnit);
	void GiveDefenderOrders(WarmindComponent& warmind, CharacterID target, Unit& unit, Unit& enemyUnit);
	CharacterID getPotentialSpouse(AIData& data);

	void warAction(AIData& data);
	void upgradeAction(AIData& data);
	void marriageAction(AIData& data);
	void handleHighestEvaluation(AIData& data);

private:
	static AIManager* m_Instance;
	WarOrders m_Orders;
	std::vector<WarmindComponent> m_Warminds;
	std::vector<AIData> m_AIDatas;
	std::vector<Personality> m_Personalities;
	mutable std::mutex m_PersonalityMtx;
	WarManager* m_WarManager = nullptr;
	UnitManager* m_UnitManager = nullptr;
};
