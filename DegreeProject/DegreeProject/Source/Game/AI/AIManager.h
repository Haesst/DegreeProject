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
	void onFileChange(std::string path, FileStatus status);
	void loadPersonalities(const char* path);
	WarmindComponent& GetWarmindOfCharacter(int handle);
	void AddWarmind(CharacterID ID);
	void Update();

private:
	float warDecision(CharacterID ID);
	float expansionDecision(CharacterID ID);

	int considerPrioritizedWar(WarmindComponent& warmind);
	void considerOrders(WarmindComponent& warmind, Unit& unit, CharacterID target);
	void GiveAttackerOrders(WarmindComponent& warmind, CharacterID target, Unit& unit, Unit& enemyUnit);
	void GiveDefenderOrders(WarmindComponent& warmind, CharacterID target, Unit& unit, Unit& enemyUnit);

private:
	WarOrders m_Orders;
	std::vector<WarmindComponent> m_Warminds;
	std::vector<Personality> m_Personalities;
	mutable std::mutex m_PersonalityMtx;
	WarManager* m_WarManager = nullptr;
	UnitManager* m_UnitManager = nullptr;
};
