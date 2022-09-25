#include "AIManager.h"
#include "Game/DiplomacyManager.h"
#include <Game\Systems\AI\WarmindConsiderations.h>
#include "Game/Systems/UnitManager.h"
#include "Game/Data/Unit.h"
#include "Game\Pathfinding.h"
#include <Game\Data\AIData.h>
#include "Managers/AIRelationshipManager.h"
#include "Managers/AIWarManager.h"
#include "Managers/AISettlementManager.h"
#include "Game/UI/UIManager.h"
#include "Game/Data/CharacterConstants.h"

AIManager* AIManager::m_Instance = nullptr;

AIManager::AIManager()
{
	loadPersonalities("Assets\\Data\\AI\\AIPersonalities.json");
	HotReloader::get()->subscribeToFileChange("Assets\\Data\\AI\\AIPersonalities.json", std::bind(&AIManager::onFileChange, this, std::placeholders::_1, std::placeholders::_2));
	m_UnitManager = &UnitManager::get();

	m_SettlementManager = new AISettlementManager();
	m_RelationshipManager = new AIRelationshipManager();
	m_AIWarManager = new AIWarManager();
	m_Orders = WarOrders();
}

AIManager::~AIManager()
{
	m_AIWarManager->~AIWarManager();

	delete m_Instance;
	delete m_AIWarManager;
	delete m_RelationshipManager;
	delete m_SettlementManager;
	delete m_UnitManager;
}

AIManager& AIManager::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new AIManager();
	}

	return *m_Instance;
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
WarmindComponent& AIManager::getWarmindOfCharacter(CharacterID handle)
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

	AIData data;
	return data;
}
#pragma warning(pop)


void AIManager::initAI(CharacterID ID)
{
	WarmindComponent warmind;
	warmind.m_OwnerID = ID;
	m_Warminds.push_back(warmind);

	AIData data;
	
	data.m_Personality = m_Personalities[rand() % m_Personalities.size()];
	data.m_OwnerID = ID;
	data.m_LastAction = Action::NONE;
	data.m_CurrentAction = Action::NONE;
	data.m_LastMarriageOfferDate = Time::m_GameDate.m_Date;

	int randomTickRate = (rand() % 8) + 1;
	data.m_TickPrio = randomTickRate;

	m_AIDatas.push_back(data);
	m_TickPrio[randomTickRate].push_back(data);
}

Personality* AIManager::getPersonality(const char* personalityName)
{
	for (auto& personality : m_Personalities)
	{
		if (personality.m_PersonalityName == personalityName)
		{
			return &personality;
		}
	}

	return nullptr;
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

	if (eval > .3f)
	{
		return true;
	}

	return false;
}

bool AIManager::handlePeaceRequest(CharacterID sender, CharacterID reciever, PeaceType type)
{
	DiplomacyManager& warManager = DiplomacyManager::get();
	int war = warManager.getWarHandleAgainst(sender, reciever);

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

	if (type == PeaceType::Enforce_Demands && warManager.getWarscore(war, sender) >= 100)
	{
		return true;
	}

	if (type == PeaceType::Enforce_Demands && warManager.getWarscore(war, sender) >= 80)
	{
		return (rand() % 100) < 50;
	}

	float acceptance = 0.0f;

	if (warManager.getWarscore(war, sender) > warManager.getWarscore(war, reciever))
	{
		acceptance += .4f;
	}

	else
	{
		acceptance -= .4f;
	}

	int senderArmySize = UnitManager::get().getUnitOfCharacter(sender).m_RepresentedForce;
	int recieverArmySize = UnitManager::get().getUnitOfCharacter(reciever).m_RepresentedForce;

	if (!UnitManager::get().getUnitOfCharacter(sender).m_Raised)
	{
		senderArmySize = 0;
	}

	if (!UnitManager::get().getUnitOfCharacter(reciever).m_Raised)
	{
		recieverArmySize = 0;
	}

	if (senderArmySize > recieverArmySize)
	{
		acceptance += .5f;
	}

	else
	{
		acceptance -= .3f;
	}

	if (Map::get().getRegionById(warManager.getWar(war)->m_WargoalRegion).m_OccupiedBy == warManager.getAttacker(war))
	{
		if (warManager.getAttacker(war) == sender)
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
	DiplomacyManager& warManager = DiplomacyManager::get();
	int currentWar = warManager.getWar(war)->getHandle();
	CharacterManager& characterManager = CharacterManager::get();

	if (warManager.getAttacker(currentWar) == characterManager.getCharacter(sender).m_CharacterID)
	{
		for (auto ally : DiplomacyManager::get().getAlliances(sender))
		{
			if (ally == warManager.getDefender(currentWar))
			{
				return false;
			}
		}
	}

	else if (warManager.getDefender(currentWar) == characterManager.getCharacter(sender).m_CharacterID)
	{
		for (auto ally : DiplomacyManager::get().getAlliances(reciever))
		{
			if (ally == warManager.getAttacker(currentWar))
			{
				return false;
			}
		}
	}

	for (auto& handle : DiplomacyManager::get().getWarHandlesOfCharacter(reciever))
	{
		if (handle == currentWar)
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
	DiplomacyManager& warManager = DiplomacyManager::get();

	if (Time::gamePaused())
	{
		return;
	}

	if (Time::m_Ticks == 1)
	{
		return;
	}

	if (Time::m_Ticks % m_TickRate1 == 1)
	{
		for (auto& data : m_TickPrio[1])
		{
			UpdateAIData(characterManager, data, warManager);
		}
	}

	if (Time::m_Ticks % m_TickRate2 == 1)
	{
		for (auto& data : m_TickPrio[2])
		{
			UpdateAIData(characterManager, data, warManager);
		}
	}

	if (Time::m_Ticks % m_TickRate3 == 1)
	{
		for (auto& data : m_TickPrio[3])
		{
			UpdateAIData(characterManager, data, warManager);
		}
	}

	if (Time::m_Ticks % m_tickRate4 == 1)
	{
		for (auto& data : m_TickPrio[4])
		{
			UpdateAIData(characterManager, data, warManager);
		}
	}

	if (Time::m_Ticks % m_TickRate5 == 1)
	{
		for (auto& data : m_TickPrio[5])
		{
			UpdateAIData(characterManager, data, warManager);
		}
	}

	if (Time::m_Ticks % m_tickRate6 == 1)
	{
		for (auto& data : m_TickPrio[6])
		{
			UpdateAIData(characterManager, data, warManager);
		}
	}

	if (Time::m_Ticks % m_tickRate7 == 1)
	{
		for (auto& data : m_TickPrio[7])
		{
			UpdateAIData(characterManager, data, warManager);
		}
	}

	if (Time::m_Ticks % m_TickRate8 == 1)
	{
		for (auto& data : m_TickPrio[8])
		{
			UpdateAIData(characterManager, data, warManager);
		}
	}
}

void AIManager::UpdateWarmind(WarmindComponent& warmind, CharacterManager& characterManager, UnitManager& unitManager, DiplomacyManager& warManager)
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

		if (unitManager.getUnitOfCharacter(warmind.m_OwnerID).m_Raised)
		{
			unitManager.dismissUnit(unitManager.getUnitOfCharacter(warmind.m_OwnerID).m_UnitID);
		}

		return;
	}

	else if (warmind.m_Active && warmind.m_PrioritizedWarHandle == -1)
	{
		warmind.m_PrioritizedWarHandle = considerPrioritizedWar(warmind);

		if (warmind.m_PrioritizedWarHandle == -1)
		{
			if (unitManager.getUnitOfCharacter(warmind.m_OwnerID).m_Raised)
			{
				unitManager.dismissUnit(unitManager.getUnitOfCharacter(warmind.m_OwnerID).m_UnitID);
				warmind.m_Active = false;
			}

			return;
		}
	}

	if (warManager.getWarHandlesOfCharacter(warmind.m_OwnerID).empty())
	{
		unitManager.dismissUnit(unitManager.getUnitOfCharacter(warmind.m_OwnerID).m_UnitID);
		warmind.m_Active = false;
	}

	else
	{
		if (!warManager.isValidWar(warmind.m_PrioritizedWarHandle))
		{
			considerPrioritizedWar(warmind);
		}

		if (m_UnitManager->getUnitOfCharacter(warmind.m_OwnerID).m_Raised)
		{
			CharacterID opposingForce = warManager.getOpposingForce(warmind.m_PrioritizedWarHandle, warmind.m_OwnerID);
			considerOrders(warmind, m_UnitManager->getUnitOfCharacter(warmind.m_OwnerID), opposingForce);
		}

		else
		{
			Character& character = characterManager.getCharacter(warmind.m_OwnerID);
			if (m_UnitManager->getUnitOfCharacter(warmind.m_OwnerID).m_RepresentedForce >= character.m_MaxArmySize * 0.5f && character.m_MaxArmySize > 0)
			{
				for (auto& region : character.m_OwnedRegionIDs)
				{
					if (Map::get().getRegionById(region).m_OccupiedBy == INVALID_CHARACTER_ID)
					{
						unitManager.raiseUnit(character.m_UnitEntity, Map::get().getRegionCapitalLocation(region));
						character.m_LeadingArmy = character.m_Age < CharacterConstants::m_DeadlyAge ? true : false;
					}
				}

				if (warmind.m_PrioritizedWarHandle != -1 && warManager.isValidWar(warmind.m_PrioritizedWarHandle))
				{
					considerOrders(warmind, m_UnitManager->getUnitOfCharacter(warmind.m_OwnerID), warManager.getOpposingForce(warmind.m_PrioritizedWarHandle, warmind.m_OwnerID));
				}

				else
				{
					warmind.m_Active = false;
					unitManager.dismissUnit(unitManager.getUnitOfCharacter(warmind.m_OwnerID).m_Owner);
				}
			}
		}
	}
}


void AIManager::UpdateAIData(CharacterManager& characterManager, AIData& data, DiplomacyManager& warManager)
{
	if (characterManager.getCharacter(data.m_OwnerID).m_CharacterTitle == Title::Unlanded)
	{
		return;
	}

	m_AIWarManager->update(data);
	m_SettlementManager->update(data);
	m_RelationshipManager->update(data);

	UpdateWarmind(getWarmindOfCharacter(data.m_OwnerID), characterManager, UnitManager::get(), warManager);
}


void AIManager::giveAttackerOrders(WarmindComponent& warmind, CharacterID target, Unit& unit, Unit& enemyUnit)
{
	DiplomacyManager* diplomacyManager = &DiplomacyManager::get();

	if (diplomacyManager->isAllyOf(warmind.m_PrioritizedWarHandle, warmind.m_OwnerID, diplomacyManager->getWar(warmind.m_PrioritizedWarHandle)->m_Attackers[0]))
	{
		m_Orders.orderFollowMainAlly(warmind, unit, diplomacyManager->getWar(warmind.m_PrioritizedWarHandle)->m_Attackers[0]);
		return;
	}

	FightEnemyArmyConsideration fightConsideration;
	float fightEval = fightConsideration.evaluate(warmind.m_OwnerID, target);

	if (fightEval > .7)
	{
		Vector2D unitPosition = unit.m_Position; //NEEDS TO CHANGE
		Vector2D enemyUnitPosition = enemyUnit.m_Position;

		float distance = (unitPosition - enemyUnitPosition).getLength();
		if (distance < 50.0f)
		{
			//Hunt enemy army
			m_Orders.orderFightEnemyArmy(warmind, unit);
			return;
		}
	}

	if (Map::get().regionOccupiedByFriendlies(CharacterManager::get().getCharacter(unit.m_Owner), warmind.m_WargoalRegionId))
	{
		bool validRegionExists = false;
		for (auto& region : CharacterManager::get().getCharacter(enemyUnit.m_Owner).m_OwnedRegionIDs)
		{
			if (Map::get().getRegionById(region).m_OccupiedBy == INVALID_CHARACTER_ID)
			{
				validRegionExists = true;
			}
		}

		validRegionExists ? m_Orders.orderAttackEnemyRegion(unit, enemyUnit) : m_Orders.orderFightEnemyArmy(warmind, unit);
		return;
	}

	//Siege wargoal region
	m_Orders.orderSiegeCapital(warmind, unit);
}

void AIManager::giveDefenderOrders(WarmindComponent& warmind, CharacterID /*target*/, Unit& unit, Unit& enemyUnit)
{
	DiplomacyManager* diplomacyManager = &DiplomacyManager::get();

	if (diplomacyManager->isAllyOf(warmind.m_PrioritizedWarHandle, warmind.m_OwnerID, diplomacyManager->getWar(warmind.m_PrioritizedWarHandle)->m_Defenders[0]))
	{
		m_Orders.orderFollowMainAlly(warmind, unit, diplomacyManager->getWar(warmind.m_PrioritizedWarHandle)->m_Defenders[0]);
		return;
	}

	if (CharacterManager::get().ownsRegion(unit.m_Owner, enemyUnit.m_SeizingRegionID))
	{
		bool validRegionExists = false;
		for (auto& region : CharacterManager::get().getCharacter(enemyUnit.m_Owner).m_OwnedRegionIDs)
		{
			if (Map::get().getRegionById(region).m_OccupiedBy == INVALID_CHARACTER_ID)
			{
				validRegionExists = true;
			}
		}

		validRegionExists ? m_Orders.orderAttackEnemyRegion(unit, enemyUnit) : m_Orders.orderFightEnemyArmy(warmind, unit);
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

void AIManager::giveAllyOrders(WarmindComponent& warmind, Unit& unit, CharacterID mainAlly)
{
	DiplomacyManager* diplomacyManager = &DiplomacyManager::get();

	if (diplomacyManager->isAllyOf(warmind.m_PrioritizedWarHandle, warmind.m_OwnerID, mainAlly))
	{
		m_Orders.orderFollowMainAlly(warmind, unit, mainAlly);
		return;
	}
}

bool AIManager::weightedRandom(float weight)
{
	float f = rand() * 1.0f / RAND_MAX;
	float vv = weight / 10.0f;
	return f < vv;
}

bool AIManager::isValidWarmind(CharacterID ID)
{
	if (CharacterManager::get().getCharacter(ID).m_Dead)
	{
		return false;
	}

	for (auto& warmind : m_Warminds)
	{
		if (warmind.m_OwnerID == ID)
		{
			return true;
		}
	}

	return false;
}

int AIManager::considerPrioritizedWar(WarmindComponent& warmind)
{
	DiplomacyManager* warManager = &DiplomacyManager::get();
	Character& character = CharacterManager::get().getCharacter(warmind.m_OwnerID);

	if (!warManager->getWarHandlesOfCharacter(character.m_CharacterID).empty())
	{
		warmind.m_PrioritizedWarHandle = warManager->getWarHandlesOfCharacter(warmind.m_OwnerID).front();

		if (warManager->isValidWar(warmind.m_PrioritizedWarHandle))
		{
			warmind.m_Opponent = warManager->getOpposingForce(warmind.m_PrioritizedWarHandle, warmind.m_OwnerID);
			return warmind.m_PrioritizedWarHandle;
		}
	}

	getAIDataofCharacter(warmind.m_OwnerID).m_CurrentAction = Action::NONE;
	return -1;
}

void AIManager::considerOrders(WarmindComponent& warmind, Unit& unit, CharacterID target)
{
	DiplomacyManager* warManager = &DiplomacyManager::get();

	if (warmind.m_PrioritizedWarHandle == -1)
	{
		return;
	}

	Unit& enemyUnit = UnitManager::get().getUnitOfCharacter(target);

	if (warManager->getWar(warmind.m_PrioritizedWarHandle) != nullptr)
	{
		if (warManager->isDefender(warmind.m_PrioritizedWarHandle, warmind.m_OwnerID))
		{
			giveDefenderOrders(warmind, target, unit, enemyUnit);
		}

		else if (warManager->isAttacker(warmind.m_PrioritizedWarHandle, warmind.m_OwnerID))
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