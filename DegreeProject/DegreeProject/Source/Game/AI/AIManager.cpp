#include "AIManager.h"
#include "Game/WarManager.h"
#include <Game\Systems\AI\WarmindConsiderations.h>
#include "Engine/UnitManager.h"
#include "Game/Data/Unit.h"
#include <Game\Data\AIData.h>

AIManager* AIManager::m_Instance = nullptr;

AIManager::AIManager()
{
	loadPersonalities("Assets\\Data\\AI\\AIPersonalities.json");
	HotReloader::get()->subscribeToFileChange("Assets\\Data\\AI\\AIPersonalities.json", std::bind(&AIManager::onFileChange, this, std::placeholders::_1, std::placeholders::_2));
	m_WarManager = &WarManager::get();
	m_UnitManager = &UnitManager::get();
}

AIManager::~AIManager()
{
	delete m_Instance;
}

AIManager& AIManager::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new AIManager();
	}

	return *m_Instance;
}

void AIManager::start()
{

}

void AIManager::loadPersonalities(const char* path)
{
	std::ifstream file(path);
	json j;
	file >> j;

	m_PersonalityMtx.lock();
	m_Personalities.clear();

	for (auto& element : j)
	{
		std::string personalityName = element["debugName"].get<std::string>();
		float declareWarModifier = element["declareWarMod"].get<float>();

		m_Personalities.push_back({ personalityName.c_str(), declareWarModifier });
	}
	m_PersonalityMtx.unlock();
}

WarmindComponent& AIManager::GetWarmindOfCharacter(int handle)
{
	for (auto& warmind : m_Warminds)
	{
		if (warmind.m_OwnerID == handle)
		{
			return warmind;
		}
	}

	ASSERT(false, "No warmind belonging to that handle");
	return WarmindComponent();
}

AIData& AIManager::getAIDataofCharacter(int handle)
{
	for (auto& data : m_AIDatas)
	{
		if (data.m_OwnerID == handle)
		{
			return data;
		}
	}

	ASSERT(false, "No data belonging to that handle");
	return AIData();
}


void AIManager::initAI(CharacterID ID)
{
	WarmindComponent warmind;
	warmind.m_OwnerID = ID;
	m_Warminds.push_back(warmind);

	AIData data;
	data.m_OwnerID = ID;
	data.m_LastAction = Action::NONE;
	data.m_CurrentAction = Action::NONE;
	m_AIDatas.push_back(data);
}

void AIManager::update()
{
	for (auto& data : m_AIDatas)
	{
		if (data.m_CurrentAction == Action::NONE)
		{
			if (expansionDecision(data.m_OwnerID) > .2f)
			{
				if (warDecision(data.m_OwnerID) > .2)
				{
					WarManager::get().createWar(data.m_OwnerID, GetWarmindOfCharacter(data.m_OwnerID).m_Opponent, GetWarmindOfCharacter(data.m_OwnerID).m_WargoalRegionId);
					GetWarmindOfCharacter(data.m_OwnerID).m_Active = true;
					data.m_CurrentAction = Action::War;
				}
			}
		}
	}

	for (auto& warmind : m_Warminds)
	{
		if (!warmind.m_Active)
		{
			continue;
		}

		warmind.m_PrioritizedWarHandle = considerPrioritizedWar(warmind);

		if (m_UnitManager->getUnitOfCharacter(warmind.m_OwnerID).m_Raised) //TODO: Fix when units are using new system
		{
			warmind.m_OrderAccu += Time::deltaTime();
		
			if (warmind.m_OrderAccu >= warmind.m_OrderTimer)
			{
				warmind.m_OrderAccu = 0.0f;
				considerOrders(warmind, m_UnitManager->getUnitOfCharacter(warmind.m_OwnerID), warmind.m_Opponent);
			}
		}

		else
		{
			UnitManager::get().raiseUnit(CharacterManager::get()->getCharacter(warmind.m_OwnerID).m_UnitEntity, Map::get().getRegionCapitalLocation(CharacterManager::get()->getCharacter(warmind.m_OwnerID).m_OwnedRegionIDs[0]));
		}
	}
}

float AIManager::warDecision(CharacterID ID)
{
	GoldConsideration goldConsideration;
	ArmySizeConsideration armySizeConsideration;

	//Personality personality = m_AIManager->m_Personalities[characterComponents->m_PersonalityIndex];

	goldConsideration.setContext(ID);
	armySizeConsideration.setContext(ID);

	float goldEvaluation = goldConsideration.evaluate(ID, GetWarmindOfCharacter(ID).m_Opponent);
	float enemyArmyEvaluation = armySizeConsideration.evaluate(ID, GetWarmindOfCharacter(ID).m_Opponent);

	float actionScore = goldEvaluation * enemyArmyEvaluation;
	//actionScore += personality.m_DeclareWarModifier;

	War* war = nullptr; //m_WarManager->getWarAgainst(m_Characters[ent], m_Characters[m_Warminds[ent].m_Opponent]);

	if (war == nullptr)
	{
		return std::clamp(actionScore, 0.0f, 1.0f);
	}

	return 0.0f;
}

float AIManager::expansionDecision(CharacterID ID)
{
	std::vector<std::pair<float, int>> actionScorePerRegion;

	ExpansionConsideration expansionConsideration;

	expansionConsideration.setContext(ID);

	//Get characters in certain range,
	std::vector<int> regionIndexes = Map::get().getRegionIDs();

	Character& character = CharacterManager::get()->getCharacter(ID);

	for (size_t i = 0; i < regionIndexes.size(); i++)
	{
		if (std::find(character.m_OwnedRegionIDs.begin(), character.m_OwnedRegionIDs.end(),
			(size_t)regionIndexes[i]) != character.m_OwnedRegionIDs.end())
		{
			continue;
		}

		float eval = expansionConsideration.evaluate(ID, regionIndexes[i]);
		auto pair = std::make_pair(eval, regionIndexes[i]);
		actionScorePerRegion.push_back(pair);
	}

	float highest = -1.0f;
	int bestIndex = -1;

	for (unsigned int i = 0; i < actionScorePerRegion.size(); i++)
	{
		if (actionScorePerRegion[i].first > highest)
		{
			highest = actionScorePerRegion[i].first;
			GetWarmindOfCharacter(ID).m_WargoalRegionId = actionScorePerRegion[i].second;
			GetWarmindOfCharacter(ID).m_Opponent = Map::get().getRegionById(actionScorePerRegion[i].second).m_OwnerID;
			bestIndex = i;
		}
	}

	if (bestIndex == -1)
	{
		return 0.0f;
	}

	return actionScorePerRegion[bestIndex].first;
}

void AIManager::GiveAttackerOrders(WarmindComponent& warmind, CharacterID target, Unit& unit, Unit& enemyUnit)
{
	FightEnemyArmyConsideration fightConsideration;
	float fightEval = fightConsideration.evaluate(warmind.m_OwnerID, target);

	if (fightEval > .7)
	{
		Vector2D unitPosition = unit.m_Position; //NEEDS TO CHANGE
		Vector2D enemyUnitPosition = enemyUnit.m_Position;

		float distance = (unitPosition - enemyUnitPosition).getLength();
		if (distance < 100.0f)
		{
			//Hunt enemy army
			//LOG_INFO("Warmind belonging to {0} decided to hunt the enemy army", m_Characters[warmind.m_OwnerID].m_Name);
			m_Orders.orderFightEnemyArmy(warmind, unit);
			return;
		}

		else
		{
			//Siege wargoal region
			//LOG_INFO("Warmind belonging to {0} decided to siege the enemy capital", m_Characters[warmind].m_Name);
			m_Orders.orderSiegeCapital(warmind, unit);
			return;
		}
	}
}

void AIManager::GiveDefenderOrders(WarmindComponent& warmind, CharacterID target, Unit& unit, Unit& enemyUnit)
{
	FightEnemyArmyConsideration fightConsideration;
	float fightEval = fightConsideration.evaluate(warmind.m_OwnerID, target);

	if (fightEval > 0.7)
	{
		m_Orders.orderFightEnemyArmy(warmind, unit);
		return;
	}

	Vector2D unitPosition = unit.m_Position; //NEEDS TO CHANGE
	Vector2D enemyUnitPosition = enemyUnit.m_Position; //NEEDS TO CHANGe

	float distance = (unitPosition - enemyUnitPosition).getLength();
	if (distance < 100.0f)
	{
		m_Orders.orderFlee(warmind, unit);
		return;
	}

	int regionID = m_WarManager->getWar(warmind.m_PrioritizedWarHandle)->m_WargoalRegion;
	Vector2DInt regionPosition = Map::get().getRegionById(regionID).m_RegionCapital;
	//Order unit to move
}

int AIManager::considerPrioritizedWar(WarmindComponent& warmind)
{
	//WarManager* warManager = &WarManager::get();
	//
	//if (!warmind.m_CurrentWars.empty())
	//{
	//	m_Warminds[ent].m_PrioritizedWarHandle = m_Characters[ent].m_CurrentWars.front();
	//	return m_Warminds[ent].m_PrioritizedWarHandle;
	//}
	//
	return -1;
}

void AIManager::considerOrders(WarmindComponent& warmind, Unit& unit, CharacterID target)
{
	if (warmind.m_PrioritizedWarHandle == -1)
	{
		return;
	}

	Unit* enemyUnit = nullptr; //m_UnitManager->getUnitOfCharacter(warmind.m_Opponent); // m_Units[m_Characters[target].m_UnitEntity];

	if (m_WarManager->getWar(warmind.m_PrioritizedWarHandle)->isDefender(warmind.m_OwnerID))
	{
		GiveDefenderOrders(warmind, target, unit, *enemyUnit);
	}

	else if (m_WarManager->getWar(warmind.m_PrioritizedWarHandle)->isAttacker(warmind.m_OwnerID))
	{
		GiveAttackerOrders(warmind, target, unit, *enemyUnit);
	}
}

void AIManager::onFileChange(std::string path, FileStatus status)
{
	if (status != FileStatus::Modified)
	{
		return;
	}

	loadPersonalities(path.c_str());
}
