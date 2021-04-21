#include "AIManager.h"
#include "Game/WarManager.h"
#include <Game\Systems\AI\WarmindConsiderations.h>
#include "Game/Systems/UnitManager.h"
#include "Game/Data/Unit.h"
#include "Game\Pathfinding.h"
#include <Game\Data\AIData.h>
#include "Game/UI/UIManager.h"

AIManager* AIManager::m_Instance = nullptr;

AIManager::AIManager()
{
	loadPersonalities("Assets\\Data\\AI\\AIPersonalities.json");
	HotReloader::get()->subscribeToFileChange("Assets\\Data\\AI\\AIPersonalities.json", std::bind(&AIManager::onFileChange, this, std::placeholders::_1, std::placeholders::_2));
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
WarmindComponent& AIManager::getWarmindOfCharacter(int handle)
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

	int randomTickRate = (rand() % 3) + 1;
	data.m_TickPrio = randomTickRate;
	m_TickPrio[randomTickRate].push_back(data);
}

void AIManager::deactivateAI(CharacterID ID)
{
	unsigned int index = 0;

	for (auto& warmind : m_Warminds)
	{
		if (warmind.m_OwnerID == ID)
		{
			break;
		}

		index++;
	}

	m_Warminds.erase(m_Warminds.begin() + index);

	index = 0;
	int tickRate = getAIDataofCharacter(ID).m_TickPrio;

	if (tickRate == -1)
	{
		return;
	}

	for (auto& data : m_TickPrio[tickRate])
	{
		if (data.m_OwnerID == ID)
		{
			break;
		}

		index++;
	}

	m_TickPrio[tickRate].erase(m_TickPrio[tickRate].begin() + index);
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

bool AIManager::handlePeaceRequest(CharacterID sender, CharacterID reciever, PeaceType type)
{
	War* war = WarManager::get().getWarAgainst(sender, reciever);
	ASSERT(war != NULL, "This war doesn't exist");

	if (type == PeaceType::Surrender)
	{
		return true;
	}

	bool allRegionsSiezed = true;
	for (auto ID : CharacterManager::get().getCharacter(reciever).m_OwnedRegionIDs)
	{
		if (Map::get().getRegionById(ID).m_OccupiedBy == INVALID_CHARACTER_ID)
		{
			allRegionsSiezed = false;
			break;
		}
	}

	if (allRegionsSiezed)
	{
		return true;
	}

	if (type == PeaceType::Enforce_Demands && war->getWarscore(sender) >= 100)
	{
		return true;
	}

	float acceptance = 0.0f;

	if (war->getWarscore(sender) > war->getWarscore(reciever))
	{
		acceptance += .4f;
	}

	else
	{
		acceptance -= .4f;
	}

	int senderArmySize = CharacterManager::get().getCharacter(sender).m_RaisedArmySize;
	int recieverArmySize = CharacterManager::get().getCharacter(reciever).m_RaisedArmySize;

	if (senderArmySize > recieverArmySize)
	{
		acceptance += .5f;
	}

	else
	{
		acceptance -= .3f;
	}

	if (Map::get().getRegionById(war->m_WargoalRegion).m_OccupiedBy == war->getAttacker())
	{
		if (war->getAttacker() == sender)
		{
			acceptance += .5f;
		}

		else
		{
			acceptance -= .3f;
		}
	}

	return weightedRandom(acceptance);
}

bool AIManager::handleAllianceRequest(CharacterID sender, CharacterID reciever)
{
	GoldConsideration goldConsideration;
	ArmySizeConsideration armyConsideration;

	float goldEval = goldConsideration.evaluate(sender, reciever);
	float armyEval = armyConsideration.evaluate(sender, reciever);
	float actionScore = armyEval * goldEval;

	//Todo: Add opinion
	if (actionScore > .5f)
	{
		return true;
	}

	return false;
}

bool AIManager::handleWarCallRequest(CharacterID sender, CharacterID reciever, int war)
{
	War* currentWar = WarManager::get().getWar(war);
	CharacterManager& characterManager = CharacterManager::get();

	if (currentWar->getAttacker() == characterManager.getCharacter(sender).m_CharacterID)
	{
		for (auto ally : WarManager::get().getAlliances(sender))
		{
			if (ally == currentWar->getDefender())
			{
				return false;
			}
		}
	}

	else if (currentWar->getDefender() == characterManager.getCharacter(sender).m_CharacterID)
	{
		for (auto ally : WarManager::get().getAlliances(reciever))
		{
			if (ally == currentWar->getAttacker())
			{
				return false;
			}
		}
	}

	for (auto& handle : WarManager::get().getWarHandlesOfCharacter(reciever))
	{
		if (handle == currentWar->getHandle())
		{
			//Already in that war
			return false;
		}
	}

	return true;
}

void AIManager::update()
{
	CharacterManager& characterManager = CharacterManager::get();
	WarManager& warManager = WarManager::get();

	if (Time::gamePaused())
	{
		return;
	}

	if (Time::m_Ticks == 1)
	{
		return;
	}

	if (Time::m_Ticks % m_tickRate1 == 1)
	{
		for (auto& data : m_TickPrio[1])
		{
			UpdateAIData(characterManager, data, warManager);
		}
	}

	if (Time::m_Ticks % m_tickRate2 == 1)
	{
		for (auto& data : m_TickPrio[2])
		{
			UpdateAIData(characterManager, data, warManager);
		}
	}

	if (Time::m_Ticks % m_tickRate3 == 1)
	{
		for (auto& data : m_TickPrio[3])
		{
			UpdateAIData(characterManager, data, warManager);
		}
	}
}

void AIManager::UpdateWarmind(WarmindComponent& warmind, CharacterManager& characterManager, UnitManager& unitManager, WarManager& warManager)
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

		return;
	}

	else if (warmind.m_Active && warmind.m_PrioritizedWarHandle == -1)
	{
		warmind.m_PrioritizedWarHandle = considerPrioritizedWar(warmind);
	}

	if (warManager.getWarHandlesOfCharacter(warmind.m_OwnerID).empty() && m_UnitManager->getUnitOfCharacter(warmind.m_OwnerID).m_Raised)
	{
		unitManager.dismissUnit(unitManager.getUnitOfCharacter(warmind.m_OwnerID).m_UnitID);
		warmind.m_Active = false;
	}

	else
	{
		if (m_UnitManager->getUnitOfCharacter(warmind.m_OwnerID).m_Raised)
		{
			considerOrders(warmind, m_UnitManager->getUnitOfCharacter(warmind.m_OwnerID), warManager.getWar(warmind.m_PrioritizedWarHandle)->getOpposingForce(warmind.m_OwnerID));
		}

		else
		{
			Character& character = characterManager.getCharacter(warmind.m_OwnerID);
			if (m_UnitManager->getUnitOfCharacter(warmind.m_OwnerID).m_RepresentedForce >= character.m_MaxArmySize * 0.5f && character.m_MaxArmySize > 0)
			{
				unitManager.raiseUnit(character.m_UnitEntity, Map::get().getRegionCapitalLocation(character.m_OwnedRegionIDs[0]));
			}
		}
	}
}


void AIManager::UpdateAIData(CharacterManager& characterManager, AIData& data, WarManager& warManager)
{
	if (characterManager.getCharacter(data.m_OwnerID).m_CharacterTitle == Title::Unlanded)
	{
		return;
	}

	if (data.m_CurrentAction == Action::War || data.m_LastAction == Action::War)
	{
		return;
	}

	if (data.m_CurrentAction != Action::War && warManager.getWarHandlesOfCharacter(data.m_OwnerID).size() == 0)
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

	if (warManager.getAlliances(data.m_OwnerID).empty())
	{
		float eval = allianceDecision(data.m_OwnerID, getPotentialAlly(data));

		if (eval > .5f)
		{
			data.m_Evaluations.push_back(std::make_pair(eval, Action::Seek_Alliance));
		}
	}

	int regionID = INVALID_REGION_ID;
	float settlementEval = upgradeDecision(data.m_OwnerID, regionID);
	data.m_SettlementToUpgrade = regionID;
	data.m_Evaluations.push_back(std::make_pair(settlementEval, Action::War));

	if (characterManager.getCharacter(data.m_OwnerID).m_Spouse == INVALID_CHARACTER_ID)
	{
		CharacterID potentialSpouse = getPotentialSpouse(data);

		if (potentialSpouse != INVALID_CHARACTER_ID)
		{
			float marriageEval = marriageDecision(data.m_OwnerID, potentialSpouse);
			data.m_PotentialSpouseID = potentialSpouse;
			data.m_Evaluations.push_back(std::make_pair(marriageEval, Action::Marriage));
		}
	}

	for (auto& war : WarManager::get().getWarHandlesOfCharacter(data.m_OwnerID))
	{
		War* currentWar = WarManager::get().getWar(war);

		if (!warManager.isValidWar(*currentWar))
		{
			continue;
		}

		if (Time::m_GameDate.m_Date.m_Month - currentWar->getStartDate().m_Month >= 4)
		{
			bool sendOffer = (rand() % 100) < 20;

			if (sendOffer)
			{
				characterManager.sendPeaceOffer(data.m_OwnerID, currentWar->getOpposingForce(data.m_OwnerID), PeaceType::White_Peace);
			}
		}
	}

	handleHighestEvaluation(data);
	UpdateWarmind(getWarmindOfCharacter(data.m_OwnerID), characterManager, UnitManager::get(), warManager);
}

CharacterID AIManager::getPotentialSpouse(AIData& data)
{
	std::vector<std::pair<float, int>> evalToSpouse;

	for (auto& region : Map::get().getRegionIDs())
	{
		if (Map::get().getRegionById(region).m_OwnerID != data.m_OwnerID)
		{
			if (CharacterManager::get().getCharacter(data.m_OwnerID).m_Gender != CharacterManager::get().getCharacter(Map::get().getRegionById(region).m_OwnerID).m_Gender)
			{
				if (CharacterManager::get().getCharacter(data.m_OwnerID).m_Spouse == INVALID_CHARACTER_ID)
				{
					continue;
				}

				float eval = marriageDecision(data.m_OwnerID, Map::get().getRegionById(region).m_OwnerID);

				if (eval > .4f)
				{
					evalToSpouse.push_back(std::make_pair(eval, Map::get().getRegionById(region).m_OwnerID));
				}
			}
		}
	}

	float highestEval = -1.f;
	CharacterID bestSpouse = INVALID_CHARACTER_ID;

	for (auto& pair : evalToSpouse)
	{
		if (pair.first > highestEval)
		{
			highestEval = pair.first;
			bestSpouse = pair.second;
		}
	}

	return bestSpouse;
}

CharacterID AIManager::getPotentialAlly(AIData& data)
{
	std::vector<std::pair<float, int>> evalToAlly;

	for (auto& region : Map::get().getRegionIDs())
	{
		if (Map::get().getRegionById(region).m_OwnerID != data.m_OwnerID)
		{
			if (CharacterManager::get().getCharacter(Map::get().getRegionById(region).m_OwnerID).m_CharacterTitle == Title::Unlanded)
			{
				continue;
			}

			if (WarManager::get().atWarWith(data.m_OwnerID, Map::get().getRegionById(region).m_OwnerID))
			{
				continue;
			}

			float eval = allianceDecision(data.m_OwnerID, Map::get().getRegionById(region).m_OwnerID);

			if (eval > .4f)
			{
				evalToAlly.push_back(std::make_pair(eval, Map::get().getRegionById(region).m_OwnerID));
			}
		}
	}

	float highestEval = -1.f;
	CharacterID bestAlly = INVALID_CHARACTER_ID;

	for (auto& pair : evalToAlly)
	{
		if (pair.first > highestEval)
		{
			highestEval = pair.first;
			bestAlly = pair.second;
			data.m_PotentialAllyID = bestAlly;
		}
	}

	return bestAlly;
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

	//Personality personality = m_Personalities[CharacterManager::get().getCharacter(ID).m_PersonalityIndex];
	goldConsideration.setContext(ID);
	armySizeConsideration.setContext(ID);

	float goldEvaluation = goldConsideration.evaluate(ID, getWarmindOfCharacter(ID).m_Opponent);
	float enemyArmyEvaluation = armySizeConsideration.evaluate(ID, getWarmindOfCharacter(ID).m_Opponent);
	float allyDebuff = 0.0f;

	if (CharacterManager::get().isAlliedWith(ID, getWarmindOfCharacter(ID).m_Opponent))
	{
		return 0.0f;
	}

	float actionScore = (goldEvaluation * enemyArmyEvaluation) - allyDebuff;

	for (auto& war : WarManager::get().getWarsForRegion(getWarmindOfCharacter(ID).m_WargoalRegionId))
	{
		actionScore -= .1f;
	}

	if (actionScore <= .3f)
	{
		return 0.0f;
	}

	return std::clamp(actionScore, 0.0f, 1.0f);
}

float AIManager::expansionDecision(CharacterID ID)
{
	std::vector<std::pair<float, int>> actionScorePerRegion;

	ExpansionConsideration expansionConsideration;

	expansionConsideration.setContext(ID);

	//Get characters in certain range,
	std::vector<int> regionIndexes = Map::get().getRegionIDs();

	Character& character = CharacterManager::get().getCharacter(ID);

	for (size_t i = 0; i < regionIndexes.size(); i++)
	{
		if (std::find(character.m_OwnedRegionIDs.begin(), character.m_OwnedRegionIDs.end(),
			(size_t)regionIndexes[i]) != character.m_OwnedRegionIDs.end())
		{
			continue;
		}

		float eval = expansionConsideration.evaluate(ID, regionIndexes[i]);

		if (eval > .5f)
		{
			auto pair = std::make_pair(eval, regionIndexes[i]);
			actionScorePerRegion.push_back(pair);
		}
	}

	if (actionScorePerRegion.empty())
	{
		return 0.0f;
	}

	std::sort(actionScorePerRegion.begin(), actionScorePerRegion.end());
	std::pair<float, int> region;

	if (actionScorePerRegion.size() > 2)
	{
		region = actionScorePerRegion[rand() % 2];
	}

	else
	{
		region = actionScorePerRegion[0];
	}


	getWarmindOfCharacter(ID).m_WargoalRegionId = region.second;
	getWarmindOfCharacter(ID).m_Opponent = Map::get().getRegionById(region.second).m_OwnerID;

	return region.first;
}

float AIManager::marriageDecision(CharacterID ID, CharacterID spouse)
{
	MarriageConsideration marriage;
	return marriage.evaluate(ID, spouse);
}

float AIManager::allianceDecision(CharacterID ID, CharacterID potentialAlly)
{
	GoldConsideration goldConsideration;
	ArmySizeConsideration armyConsideration;

	float goldEval = goldConsideration.evaluate(ID, potentialAlly);
	float armyEval = armyConsideration.evaluate(ID, potentialAlly);
	float actionScore = armyEval * goldEval;

	if (actionScore > .5f)
	{
		return actionScore;
	}

	return 0.0f;
}

void AIManager::giveAttackerOrders(WarmindComponent& warmind, CharacterID target, Unit& unit, Unit& enemyUnit)
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
			m_Orders.orderFightEnemyArmy(warmind, unit);
			return;
		}
	}

	if (Map::get().regionOccupiedByFriendlies(CharacterManager::get().getCharacter(unit.m_Owner), warmind.m_WargoalRegionId))
	{
		m_Orders.orderAttackEnemyRegion(unit, enemyUnit);
		return;
	}

	//Siege wargoal region
	m_Orders.orderSiegeCapital(warmind, unit);
}

void AIManager::giveDefenderOrders(WarmindComponent& warmind, CharacterID /*target*/, Unit& unit, Unit& enemyUnit)
{
	if (CharacterManager::get().ownsRegion(unit.m_Owner, enemyUnit.m_SeizingRegionID))
	{
		m_Orders.orderAttackArmy(unit, enemyUnit);
		return;
	}

	if (enemyUnit.m_RepresentedForce < unit.m_RepresentedForce)
	{
		if (CharacterManager::get().getCharacter(enemyUnit.m_Owner).m_OwnedRegionIDs.size() > 0)
		{
			m_Orders.orderAttackEnemyRegion(unit, enemyUnit);
			return;
		}
	}

	m_Orders.orderDefendWargoal(warmind, unit, enemyUnit);
}

void AIManager::warAction(AIData& data)
{
	CharacterManager& characterManager = CharacterManager::get();
	unsigned int opponent = getWarmindOfCharacter(data.m_OwnerID).m_Opponent;

	if (opponent == INT_MAX || opponent == characterManager.getCharacter(data.m_OwnerID).m_Spouse)
	{
		return;
	}

	if (characterManager.isAlliedWith(data.m_OwnerID, getWarmindOfCharacter(data.m_OwnerID).m_Opponent))
	{
		return;
	}

	int warHandle = WarManager::get().createWar(data.m_OwnerID, getWarmindOfCharacter(data.m_OwnerID).m_Opponent, getWarmindOfCharacter(data.m_OwnerID).m_WargoalRegionId);

	characterManager.callAllies(data.m_OwnerID, warHandle);

	getWarmindOfCharacter(data.m_OwnerID).m_Active = true;

	WarmindComponent& warmind = getWarmindOfCharacter(data.m_OwnerID);

	if (!characterManager.getCharacter(warmind.m_Opponent).m_IsPlayerControlled)
	{
		getWarmindOfCharacter(warmind.m_Opponent).m_Active = true;
		getWarmindOfCharacter(warmind.m_Opponent).m_Opponent = warmind.m_OwnerID;
	}
	else
	{
		UIManager::get().createUIEventElement(warmind.m_OwnerID, characterManager.getPlayerCharacterID(), UIType::WarDeclaration);
		UIManager::get().createWarIcon(warmind.m_OwnerID, characterManager.getPlayerCharacterID());
		characterManager.callAllies(warmind.m_OwnerID, warHandle);
		characterManager.callAllies(characterManager.getPlayerCharacterID(), warHandle);
	}

	LOG_INFO("{0} Declared war against {1}", characterManager.getCharacter(data.m_OwnerID).m_Name, characterManager.getCharacter(getWarmindOfCharacter(data.m_OwnerID).m_Opponent).m_Name);
	data.m_LastAction = Action::War;
	data.m_CurrentAction = Action::War;

	if (!characterManager.getCharacter(getWarmindOfCharacter(data.m_OwnerID).m_Opponent).m_IsPlayerControlled)
	{
		getAIDataofCharacter(characterManager.getCharacter(getWarmindOfCharacter(data.m_OwnerID).m_Opponent).m_CharacterID).m_CurrentAction = Action::War;
		getAIDataofCharacter(characterManager.getCharacter(getWarmindOfCharacter(data.m_OwnerID).m_Opponent).m_CharacterID).m_LastAction = Action::War;
	}
}

void AIManager::upgradeAction(AIData& data)
{
	if (data.m_SettlementToUpgrade == INVALID_REGION_ID)
	{
		return;
	}

	int buildingIndex = rand() % GameData::m_Buildings.size();

	int buildingId = INVALID_BUILDING_ID;

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
		if (Map::get().getRegionById(data.m_SettlementToUpgrade).m_BuildingSlots[i].m_BuildingId == INVALID_BUILDING_ID)
		{
			CharacterManager::get().constructBuilding(data.m_OwnerID, buildingId, data.m_SettlementToUpgrade, i);
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

	CharacterManager::get().marry(data.m_OwnerID, data.m_PotentialSpouseID);
}

void AIManager::allianceAction(AIData& data)
{
	if (data.m_PotentialAllyID == INVALID_CHARACTER_ID)
	{
		return;
	}

	CharacterManager::get().sendAllianceOffer(data.m_OwnerID, data.m_PotentialAllyID);
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
		return;
	}

	switch (bestAction)
	{
	case Action::War:
		if (data.m_LastAction != Action::War)
		{
			warAction(data);
			data.m_LastAction = Action::War;
			data.m_CurrentAction = Action::War;
		}
		break;
	case Action::Upgrade_Settlement:
		upgradeAction(data);
		data.m_LastAction = Action::Upgrade_Settlement;
		data.m_CurrentAction = Action::Upgrade_Settlement;

		break;
	case Action::Marriage:
		marriageAction(data);
		data.m_LastAction = Action::Marriage;
		data.m_CurrentAction = Action::Marriage;

		break;

	case Action::Seek_Alliance:
		allianceAction(data);
		data.m_LastAction = Action::Seek_Alliance;
		data.m_CurrentAction = Action::Seek_Alliance;
		break;

	case Action::NONE:
		data.m_PotentialSpouseID = INVALID_CHARACTER_ID;
		data.m_SettlementToUpgrade = INVALID_REGION_ID;
		break;

	default:
		break;
	}

	data.m_Evaluations.clear();
}

bool AIManager::weightedRandom(float weight)
{
	float f = rand() * 1.0f / RAND_MAX;
	float vv = weight / 10.0f;
	return f < vv;
}

int AIManager::considerPrioritizedWar(WarmindComponent& warmind)
{
	WarManager* warManager = &WarManager::get();
	Character& character = CharacterManager::get().getCharacter(warmind.m_OwnerID);

	if (!warManager->getWarHandlesOfCharacter(character.m_CharacterID).empty())
	{
		warmind.m_PrioritizedWarHandle = warManager->getWarHandlesOfCharacter(warmind.m_OwnerID).front();

		if (warManager->getWar(warmind.m_PrioritizedWarHandle) != nullptr)
		{
			warmind.m_Opponent = warManager->getWar(warmind.m_PrioritizedWarHandle)->getOpposingForce(warmind.m_OwnerID);
			return warmind.m_PrioritizedWarHandle;
		}
	}

	return -1;
}

void AIManager::considerOrders(WarmindComponent& warmind, Unit& unit, CharacterID target)
{
	WarManager* warManager = &WarManager::get();

	if (warmind.m_PrioritizedWarHandle == -1)
	{
		considerPrioritizedWar(warmind);
	}

	Unit& enemyUnit = UnitManager::get().getUnitOfCharacter(target);

	if (warManager->getWar(warmind.m_PrioritizedWarHandle) != nullptr)
	{
		if (warManager->getWar(warmind.m_PrioritizedWarHandle)->isDefender(warmind.m_OwnerID))
		{
			giveDefenderOrders(warmind, target, unit, enemyUnit);
		}

		else if (warManager->getWar(warmind.m_PrioritizedWarHandle)->isAttacker(warmind.m_OwnerID))
		{
			giveAttackerOrders(warmind, target, unit, enemyUnit);
		}
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
