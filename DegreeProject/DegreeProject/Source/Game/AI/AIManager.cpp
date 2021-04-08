#include "AIManager.h"
#include "Game/WarManager.h"
#include <Game\Systems\AI\WarmindConsiderations.h>
#include "Game/Systems/UnitManager.h"
#include "Game/Data/Unit.h"
#include "Game\Pathfinding.h"
#include <Game\Data\AIData.h>

AIManager* AIManager::m_Instance = nullptr;

AIManager::AIManager()
{
	loadPersonalities("Assets\\Data\\AI\\AIPersonalities.json");
	HotReloader::get()->subscribeToFileChange("Assets\\Data\\AI\\AIPersonalities.json", std::bind(&AIManager::onFileChange, this, std::placeholders::_1, std::placeholders::_2));
	m_WarManager = &WarManager::get();
	m_UnitManager = &UnitManager::get();
	m_Orders = WarOrders();
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

		m_Personalities.push_back({ personalityName, declareWarModifier });
	}
	m_PersonalityMtx.unlock();
}

#pragma warning(push)
#pragma warning(disable: 4239 4172)
WarmindComponent& AIManager::GetWarmindOfCharacter(int handle)
{
	size_t convertedHandle = (size_t)handle;
	for (auto& warmind : m_Warminds)
	{
		if (warmind.m_OwnerID == convertedHandle)
		{
			return warmind;
		}
	}

	ASSERT(false, "No warmind belonging to that handle");

	return WarmindComponent();
}
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable: 4239 4172 26444)
AIData& AIManager::getAIDataofCharacter(int handle)
{
	size_t convertedHandle = (size_t)handle;
	for (auto& data : m_AIDatas)
	{
		if (data.m_OwnerID == convertedHandle)
		{
			return data;
		}
	}

	ASSERT(false, "No data belonging to that handle");
	return AIData();
}
#pragma warning(pop)


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

bool AIManager::handleRecieveMarriageRequest(CharacterID reciever, CharacterID sender)
{
	MarriageConsideration consideration;
	float eval = consideration.evaluate(reciever, sender);

	if (eval > .5)
	{
		return true;
	}

	return false;
}

void AIManager::update()
{
	for (auto& data : m_AIDatas)
	{
		if (CharacterManager::get()->getCharacter(data.m_OwnerID).m_CharacterTitle == Title::Unlanded)
		{
			continue;
		}

		data.m_ConsiderationAccu += Time::deltaTime();

		if (data.m_ConsiderationAccu > data.m_ConsiderationTimer)
		{
			if (data.m_CurrentAction == Action::War)
			{
				data.m_ConsiderationAccu = 0;
				continue;
			}

			if (data.m_CurrentAction != Action::War && CharacterManager::get()->getCharacter(data.m_OwnerID).m_CurrentWars.size() == 0)
			{
				if (expansionDecision(data.m_OwnerID) > .3f)
				{
					float warEval = warDecision(data.m_OwnerID);

					if (data.m_LastAction == Action::War)
					{
						warEval -= .3f;
					}

					data.m_Evaluations.push_back(std::make_pair(warEval, Action::War));
				}
			}

			int regionID = -1;
			float settlementEval = upgradeDecision(data.m_OwnerID, regionID);
			data.m_SettlementToUpgrade = regionID;
			data.m_Evaluations.push_back(std::make_pair(settlementEval, Action::War));

			if (CharacterManager::get()->getCharacter(data.m_OwnerID).m_Spouse == INVALID_CHARACTER_ID)
			{
				CharacterID potentialSpouse = getPotentialSpouse(data);

				if (potentialSpouse != INVALID_CHARACTER_ID)
				{
					float marriageEval = marriageDecision(data.m_OwnerID, potentialSpouse);
					data.m_PotentialSpouseID = potentialSpouse;
					data.m_Evaluations.push_back(std::make_pair(marriageEval, Action::Marriage));
				}
			}

			handleHighestEvaluation(data);
			data.m_ConsiderationAccu = 0;
		}
	}

	for (auto& warmind : m_Warminds)
	{
		if (!warmind.m_Active)
		{
			if (getAIDataofCharacter(warmind.m_OwnerID).m_CurrentAction == Action::War)
			{
				getAIDataofCharacter(warmind.m_OwnerID).m_CurrentAction = Action::NONE;
			}

			if (warmind.m_PrioritizedWarHandle != -1)
			{
				warmind.m_PrioritizedWarHandle = -1;
			}

			continue;
		}

		else if (warmind.m_Active)
		{
			warmind.m_PrioritizedWarHandle = considerPrioritizedWar(warmind);
		}

		warmind.m_TickAccu++;

		if (warmind.m_TickAccu > warmind.m_AtWarTickRate)
		{
			if (CharacterManager::get()->getCharacter(warmind.m_OwnerID).m_CurrentWars.empty() && m_UnitManager->getUnitOfCharacter(warmind.m_OwnerID).m_Raised)
			{
				UnitManager::get().dismissUnit(CharacterManager::get()->getCharacter(warmind.m_OwnerID).m_UnitEntity);
			}

			if (warmind.m_PrioritizedWarHandle == -1)
			{
				UnitManager::get().dismissUnit(UnitManager::get().getUnitOfCharacter(warmind.m_OwnerID).m_UnitID);
			}

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

			warmind.m_TickAccu = 0;
		}
	}
}

CharacterID AIManager::getPotentialSpouse(AIData& data)
{
	for (auto& region : Map::get().getRegionIDs())
	{
		if (Map::get().getRegionById(region).m_OwnerID != data.m_OwnerID)
		{
			if (CharacterManager::get()->getCharacter(data.m_OwnerID).m_Gender != CharacterManager::get()->getCharacter(Map::get().getRegionById(region).m_OwnerID).m_Gender)
			{
				if (marriageDecision(data.m_OwnerID, Map::get().getRegionById(region).m_OwnerID) > .4f)
				{
					return Map::get().getRegionById(region).m_OwnerID;
				}
			}
		}
	}

	return INVALID_CHARACTER_ID;
}

float AIManager::upgradeDecision(CharacterID ID, int& outRegion)
{
	UpgradeSettlementConsideration consideration;
	consideration.setContext(ID);
	return consideration.evaluate(ID, outRegion);
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

float AIManager::marriageDecision(CharacterID ID, CharacterID spouse)
{
	MarriageConsideration marriage;
	return marriage.evaluate(ID, spouse);
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

void AIManager::GiveDefenderOrders(WarmindComponent& warmind, CharacterID /*target*/, Unit& unit, Unit& enemyUnit)
{
	//FightEnemyArmyConsideration fightConsideration;
	//float fightEval = fightConsideration.evaluate(warmind.m_OwnerID, target);

	//if (fightEval > 0.7)
	//{
	//	m_Orders.orderFightEnemyArmy(warmind, unit);
	//	return;
	//}

	Vector2D unitPosition = unit.m_Position;
	Vector2D enemyUnitPosition = enemyUnit.m_Position;

	//float distance = (unitPosition - enemyUnitPosition).getLength();
	//if (distance < 100.0f)
	//{
	//	m_Orders.orderFlee(warmind, unit);
	//	return;
	//}

	int regionID = m_WarManager->getWar(warmind.m_PrioritizedWarHandle)->m_WargoalRegion;
	Vector2DInt regionPosition = Map::get().getRegionById(regionID).m_RegionCapital;
	//Order unit to move
	UnitManager::get().giveUnitPath(UnitManager::get().getUnitOfCharacter(warmind.m_OwnerID).m_UnitID, Pathfinding::get().findPath(Map::get().convertToMap(unitPosition), regionPosition));
}

void AIManager::warAction(AIData& data)
{
	int warHandle = WarManager::get().createWar(data.m_OwnerID, GetWarmindOfCharacter(data.m_OwnerID).m_Opponent, GetWarmindOfCharacter(data.m_OwnerID).m_WargoalRegionId);
	War* war = WarManager::get().getWar(warHandle);
	CharacterManager::get()->getCharacter(war->getAttacker()).m_CurrentWars.push_back(warHandle);
	CharacterManager::get()->getCharacter(war->getDefender()).m_CurrentWars.push_back(warHandle);

	GetWarmindOfCharacter(data.m_OwnerID).m_Active = true;

	WarmindComponent& warmind = GetWarmindOfCharacter(data.m_OwnerID);

	if (!CharacterManager::get()->getCharacter(warmind.m_Opponent).m_IsPlayerControlled)
	{
		GetWarmindOfCharacter(warmind.m_Opponent).m_Active = true;
	}

	if (!CharacterManager::get()->getCharacter(warmind.m_Opponent).m_IsPlayerControlled)
	{
		GetWarmindOfCharacter(warmind.m_Opponent).m_Opponent = warmind.m_OwnerID;
	}

	LOG_INFO("{0} Declared war against {1}", CharacterManager::get()->getCharacter(data.m_OwnerID).m_Name, CharacterManager::get()->getCharacter(GetWarmindOfCharacter(data.m_OwnerID).m_Opponent).m_Name);
	data.m_LastAction = Action::War;
	data.m_CurrentAction = Action::War;

	if (!CharacterManager::get()->getCharacter(GetWarmindOfCharacter(data.m_OwnerID).m_Opponent).m_IsPlayerControlled)
	{
		getAIDataofCharacter(CharacterManager::get()->getCharacter(GetWarmindOfCharacter(data.m_OwnerID).m_Opponent).m_CharacterID).m_CurrentAction = Action::War;
		getAIDataofCharacter(CharacterManager::get()->getCharacter(GetWarmindOfCharacter(data.m_OwnerID).m_Opponent).m_CharacterID).m_LastAction = Action::War;
	}
}

void AIManager::upgradeAction(AIData& data)
{
	if (data.m_SettlementToUpgrade == -1)
	{
		return;
	}

	int buildingIndex = rand() % GameData::m_Buildings.size();

	int buildingId = -1;

	int index = 0;
	for (auto& pair : GameData::m_Buildings)
	{
		if (index == buildingIndex)
		{
			buildingId = pair.second.m_Id;
			break;
		}
		++index;
	}

	for (int i = 0; i < 4; i++)
	{
		if (Map::get().getRegionById(data.m_SettlementToUpgrade).m_BuildingSlots[i].m_BuildingId == -1)
		{
			CharacterManager::get()->constructBuilding(data.m_OwnerID, buildingId, data.m_SettlementToUpgrade, i);
			data.m_LastAction = Action::Upgrade_Settlement;
			break;
		}
	}
}

void AIManager::marriageAction(AIData& data)
{
	if (data.m_PotentialSpouseID == INVALID_CHARACTER_ID)
	{
		return;
	}

	CharacterManager::get()->marry(data.m_OwnerID, data.m_PotentialSpouseID);
}

void AIManager::handleHighestEvaluation(AIData& data)
{
	float highest = -1.0f;
	Action bestAction = Action::NONE;

	for (auto& eval : data.m_Evaluations)
	{
		if (eval.first > highest)
		{
			highest = eval.first;
			bestAction = eval.second;
		}
	}

	if (highest < .3f)
	{
		data.m_CurrentAction = Action::NONE;
	}

	switch (bestAction)
	{
	case Action::War:
		warAction(data);
		data.m_LastAction = Action::War;
		break;
	case Action::Upgrade_Settlement:
		upgradeAction(data);
		data.m_LastAction = Action::Upgrade_Settlement;
		LOG_INFO("AI UPGRADING SETTLEMENT");
		break;
	case Action::Marriage:
		marriageAction(data);
		data.m_LastAction = Action::Marriage;
		LOG_INFO("AI MARRIED");
		break;
	case Action::NONE:
		data.m_PotentialSpouseID = INVALID_CHARACTER_ID;
		data.m_SettlementToUpgrade = -1;
		break;

	default:
		break;
	}

	data.m_Evaluations.clear();
}

int AIManager::considerPrioritizedWar(WarmindComponent& warmind)
{
	// WarManager* warManager = &WarManager::get();

	if (!CharacterManager::get()->getCharacter(warmind.m_OwnerID).m_CurrentWars.empty())
	{
		warmind.m_PrioritizedWarHandle = CharacterManager::get()->getCharacter(warmind.m_OwnerID).m_CurrentWars.front();
		return warmind.m_PrioritizedWarHandle;
	}

	return -1;
}

void AIManager::considerOrders(WarmindComponent& warmind, Unit& unit, CharacterID target)
{
	if (warmind.m_PrioritizedWarHandle == -1)
	{
		considerPrioritizedWar(warmind);
	}

	Unit& enemyUnit = UnitManager::get().getUnitOfCharacter(target);

	if (m_WarManager->getWar(warmind.m_PrioritizedWarHandle)->isDefender(warmind.m_OwnerID))
	{
		GiveDefenderOrders(warmind, target, unit, enemyUnit);
	}

	else if (m_WarManager->getWar(warmind.m_PrioritizedWarHandle)->isAttacker(warmind.m_OwnerID))
	{
		GiveAttackerOrders(warmind, target, unit, enemyUnit);
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
