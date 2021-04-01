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
	const char* m_PersonalityName;
	float m_DeclareWarModifier = 0.0f;

	Personality(const char* personalityName, float declareWarModifier)
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
	void update();

private:
	float warDecision(CharacterID ID);
	float expansionDecision(CharacterID ID);

	int considerPrioritizedWar(WarmindComponent& warmind);
	void considerOrders(WarmindComponent& warmind, Unit& unit, CharacterID target);
	void GiveAttackerOrders(WarmindComponent& warmind, CharacterID target, Unit& unit, Unit& enemyUnit);
	void GiveDefenderOrders(WarmindComponent& warmind, CharacterID target, Unit& unit, Unit& enemyUnit);

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
