#include "DiplomacyManager.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/AI/AIManager.h"
#include "Game/Data/AIData.h"
#include "Game/Data/Character.h"
#include "Game/Data/Unit.h"
#include "Game/Map/Map.h"
#include "Game/UI/UIManager.h"
#include "Game/Systems/UnitManager.h"

DiplomacyManager* DiplomacyManager::m_Instance = nullptr;

int DiplomacyManager::createWar(CharacterID attacker, CharacterID defender, int warGoalRegion)
{
	int handle = m_Warhandle++;
	m_Wars.push_back(std::make_pair(handle, War(attacker, defender, warGoalRegion, handle)));

	CharacterManager& characterManager = CharacterManager::get();
	AIManager& aiManager = AIManager::get();

	if (!characterManager.getCharacter(attacker).m_IsPlayerControlled)
	{
		aiManager.getWarmindOfCharacter(attacker).m_Active = true;
	}

	if (!characterManager.getCharacter(defender).m_IsPlayerControlled)
	{
		aiManager.getWarmindOfCharacter(defender).m_Active = true;
	}

	return handle;
}

void DiplomacyManager::endWar(int warHandle, CharacterID winner)
{
	unsigned int index = 0;

	if (getWar(warHandle) == nullptr)
	{
		return;
	}
	
	makeTruce(getAttacker(warHandle), getDefender(warHandle));

	if (winner != INVALID_CHARACTER_ID)
	{
		if (!CharacterManager::get().getCharacter(getAttacker(warHandle)).m_IsPlayerControlled)
		{
			AIManager::get().getAIDataofCharacter(getAttacker(warHandle)).m_CurrentAction = Action::NONE;
			AIManager::get().getWarmindOfCharacter(getAttacker(warHandle)).m_PrioritizedWarHandle = -1;

		}

		if (!CharacterManager::get().getCharacter(getDefender(warHandle)).m_IsPlayerControlled)
		{
			AIManager::get().getAIDataofCharacter(getDefender(warHandle)).m_CurrentAction = Action::NONE;
			AIManager::get().getWarmindOfCharacter(getDefender(warHandle)).m_PrioritizedWarHandle = -1;
		}
	}

	if (getAttacker(warHandle) == winner)
	{
		invalidateWarsForRegionOnWonWar(*getWar(warHandle));
	}

	handleOccupiedRegions(warHandle, winner);
	
	for (auto& pair : m_Wars)
	{
		if (pair.first == warHandle)
		{
			m_Wars.erase(m_Wars.begin() + index);
			break;
		}

		index++;
	}
}

void DiplomacyManager::start()
{
	Time::m_GameDate.subscribeToMonthChange(std::bind(&DiplomacyManager::onMonthChange, this, std::placeholders::_1));
}

War* DiplomacyManager::getWar(int handle)
{
	for (auto& pair : m_Wars)
	{
		if (pair.first == handle)
		{
			return &pair.second;
		}
	}

	return nullptr;
}

bool DiplomacyManager::atWarWith(CharacterID character, CharacterID enemy)
{
	for (auto& war : m_Wars)
	{
		if (isAttacker(war.second.getHandle(), character))
		{
			if (isDefender(war.second.getHandle(), enemy))
			{
				return true;
			}
		}

		else if (isDefender(war.second.getHandle(), character))
		{
			if (isAttacker(war.second.getHandle(), enemy))
			{
				return true;
			}
		}
	}

	return false;
}

void DiplomacyManager::invalidateWarsForRegionOnWonWar(War& wonWar)
{
	UnitManager& unitManager = UnitManager::get();

	for (auto& war : m_Wars)
	{
		if (war.second.m_WargoalRegion == wonWar.m_WargoalRegion)
		{
			if (war.second.getHandle() == wonWar.getHandle())
			{
				continue;
			}

			for (auto& attacker : war.second.m_Attackers)
			{
				unitManager.dismissUnit(unitManager.getUnitOfCharacter(attacker).m_UnitID);
			}

			for (auto& defender : war.second.m_Defenders)
			{
				unitManager.dismissUnit(unitManager.getUnitOfCharacter(defender).m_UnitID);
			}

			endWar(war.second.getHandle(), INVALID_CHARACTER_ID);
		}
	}
}

void DiplomacyManager::invalidateWarsForRegion(int regionID)
{
	for (auto& war : m_Wars)
	{
		if (war.second.m_WargoalRegion == regionID)
		{
			endWar(war.second.getHandle(), INVALID_CHARACTER_ID);
		}
	}
}

void DiplomacyManager::invalidateWar(int warHandle)
{
	endWar(warHandle, INVALID_CHARACTER_ID);
}

void DiplomacyManager::InvalidateWarsOfCharacter(CharacterID ID)
{
	for (auto& war : getWarHandlesOfCharacter(ID))
	{
		endWar(war, INVALID_CHARACTER_ID);
	}
}

bool DiplomacyManager::isEnemyOfEnemy(Unit& unit, Unit& enemyUnit)
{
	std::vector<CharacterID> enemies = getOpposingSide(unit.m_Owner);
	std::vector<CharacterID> enemyEnemies = getOpposingSide(enemyUnit.m_Owner);

	for (auto& character : enemies)
	{
		for (auto& enemy : enemyEnemies)
		{
			if (character == enemy)
			{
				return true;
			}
		}
	}

	return false;
}

bool DiplomacyManager::isValidWar(int warHandle)
{
	for (auto ID : m_Wars)
	{
		if (ID.first == warHandle)
		{
			if (getWar(warHandle)->m_Attackers.size() > 0 && getWar(warHandle)->m_Defenders.size() > 0)
			{
				return true;
			}

			return false;
		}
	}

	return false;
}

void DiplomacyManager::update()
{
	std::vector<int> invalidHandles;

	for (auto& war : m_Wars)
	{
		War* currentWar = getWar(war.second.getHandle());
		
		if (currentWar == nullptr)
		{
			continue;
		}

		if (!isValidWar(currentWar->getHandle()))
		{
			invalidHandles.push_back(currentWar->getHandle());
			continue;
		}

		if (Map::get().getRegionById(war.second.m_WargoalRegion).m_OccupiedBy == INVALID_CHARACTER_ID)
		{
			if (isValidWar(war.second.getHandle()))
			{
				war.second.warGoalRegionTimerAccu += Time::deltaTime();
			}

			if (war.second.warGoalRegionTimerAccu >= war.second.warGoalRegionCapturedTimer)
			{
				addWarscore(currentWar->getHandle(), getDefender(currentWar->getHandle()), 5);

				if (war.second.m_DefenderOccupiedRegions.size() > war.second.m_AttackerOccupiedRegions.size())
				{
					for (auto ID : war.second.m_DefenderOccupiedRegions)
					{
						addWarscore(currentWar->getHandle(), getDefender(currentWar->getHandle()), 1);
					}
				}

				war.second.warGoalRegionTimerAccu = 0;
			}
		}

		else if (Map::get().getRegionById(war.second.m_WargoalRegion).m_OccupiedBy != INVALID_CHARACTER_ID)
		{
			currentWar->warGoalRegionTimerAccu += Time::deltaTime();

			if (currentWar->warGoalRegionTimerAccu >= currentWar->warGoalRegionCapturedTimer)
			{
				addWarscore(currentWar->getHandle(), getAttacker(currentWar->getHandle()), 5);

				if (war.second.m_AttackerOccupiedRegions.size() > war.second.m_DefenderOccupiedRegions.size())
				{
					for (auto ID : war.second.m_AttackerOccupiedRegions)
					{
						addWarscore(currentWar->getHandle(), getAttacker(currentWar->getHandle()), 1);
					}
				}

				war.second.warGoalRegionTimerAccu = 0;
			}
		}
	}

	for (int handle : invalidHandles)
	{
		eraseWar(handle);
	}
}

bool DiplomacyManager::isWinning(int warHandle, CharacterID ID, CharacterID enemyID)
{
	War* war = getWar(warHandle);

	if (getWarscore(war->getHandle(), ID) > getWarscore(war->getHandle(), enemyID))
	{
		return true;
	}

	return false;
}

int DiplomacyManager::getWarscore(int warHandle, CharacterID ID)
{
	War* war = getWar(warHandle);

	if (war == nullptr)
	{
		return -1;
	}

	if (!war->m_Attackers.empty() && !war->m_Defenders.empty()) //Todo: real solution


		if (getAttacker(war->getHandle()) == ID)
		{
			return war->m_AttackerWarscore;
		}

		else if (getDefender(war->getHandle()) == ID)
		{
			return war->m_DefenderWarscore;
		}

	return -1;
}

void DiplomacyManager::addWarscore(int warHandle, CharacterID ID, int warScore)
{
	War* war = getWar(warHandle);

	if (war == nullptr)
	{
		return;
	}

	if (isAttacker(war->getHandle(), ID))
	{
		war->m_AttackerWarscore += warScore;
		war->m_DefenderWarscore -= warScore;

		if (war->m_AttackerWarscore >= 100)
		{
			if (!CharacterManager::get().getCharacter(getAttacker(war->getHandle())).m_IsPlayerControlled)
			{
				CharacterManager::get().sendPeaceOffer(getAttacker(war->getHandle()), getDefender(war->getHandle()), PeaceType::Enforce_Demands);
				return;
			}
		}
	}

	else if (isDefender(war->getHandle(), ID))
	{
		war->m_DefenderWarscore += warScore;
		war->m_AttackerWarscore -= warScore;

		if (war->m_DefenderWarscore >= 100)
		{
			if (!CharacterManager::get().getCharacter(getDefender(war->getHandle())).m_IsPlayerControlled)
			{
				CharacterManager::get().sendPeaceOffer(getDefender(war->getHandle()), getAttacker(war->getHandle()), PeaceType::Enforce_Demands);
				return;
			}
		}
	}
}

void DiplomacyManager::addAttacker(int warHandle, CharacterID character)
{
	War* war = getWar(warHandle);

	if (war == nullptr)
	{
		return;
	}

	for (auto ID : war->m_Attackers)
	{
		if (ID == character)
		{
			return;
		}
	}

	war->m_Attackers.push_back(character);
}

void DiplomacyManager::addDefender(int warHandle, CharacterID character)
{
	War* war = getWar(warHandle);

	if (war == nullptr)
	{
		return;
	}

	for (auto ID : war->m_Defenders)
	{
		if (ID == character)
		{
			return;
		}
	}

	war->m_Defenders.push_back(character);
}

void DiplomacyManager::handleOccupiedRegions(int warHandle, CharacterID winningCharacter)
{
	War* war = getWar(warHandle);

	if (war == nullptr)
	{
		return;
	}

	Character& attacker = CharacterManager::get().getCharacter(war->m_Attackers[0]);
	Character& defender = CharacterManager::get().getCharacter(war->m_Defenders[0]);

	for (auto& region : war->m_AttackerOccupiedRegions)
	{
		Map::get().getRegionById(region).m_OccupiedBy = INVALID_CHARACTER_ID;
		defender.m_MaxArmySize += Map::get().getRegionById(region).m_ManPower;
	}

	if (getAttacker(war->getHandle()) == winningCharacter)
	{
		if (winningCharacter != INVALID_CHARACTER_ID)
		{
			CharacterID loserCharacter = Map::get().getRegionById(war->m_WargoalRegion).m_OwnerID;
			CharacterManager::get().removeRegion(loserCharacter, war->m_WargoalRegion);
			CharacterManager::get().addRegion(winningCharacter, war->m_WargoalRegion);
			UIManager::get().AdjustOwnership(winningCharacter, loserCharacter, Map::get().getRegionById(war->m_WargoalRegion).m_RegionId);
			Map::get().setRegionColor(Map::get().getRegionById(war->m_WargoalRegion).m_RegionId, CharacterManager::get().getCharacter(winningCharacter).m_RegionColor);
			if (CharacterManager::get().getCharacter(loserCharacter).m_OwnedRegionIDs.size() == 0)
			{
				UIManager::get().SetRealmTextAsConquered(loserCharacter);
			}
		}
	}

	for (auto& region : war->m_DefenderOccupiedRegions)
	{
		Map::get().getRegionById(region).m_OccupiedBy = INVALID_CHARACTER_ID;
		attacker.m_MaxArmySize += Map::get().getRegionById(region).m_ManPower;
	}
}

bool DiplomacyManager::alliesInWar(int warHandle, CharacterID ID)
{
	War* war = getWar(warHandle);

	if (war == nullptr)
	{
		return false;
	}

	if (ID == getAttacker(war->getHandle()))
	{
		if (DiplomacyManager::get().getAlliances(ID).size() + 1 == war->m_Attackers.size())
		{
			return true;
		}
	}

	if (ID == getAttacker(war->getHandle()))
	{
		if (DiplomacyManager::get().getAlliances(ID).size() + 1 == war->m_Defenders.size())
		{
			return true;
		}
	}

	return false;
}

bool DiplomacyManager::isAllyOf(int warHandle, CharacterID potentialAlly, CharacterID allyOf)
{
	War* war = getWar(warHandle);

	if (war == nullptr)
	{
		return false;
	}

	if (isAttacker(war->getHandle(), allyOf))
	{
		for (auto& attacker : war->m_Attackers)
		{
			for (auto& attackerAlly : getAlliances(allyOf))
			{
				if (attackerAlly == potentialAlly)
				{
					return true;
				}
			}
		}
	}

	else if (isDefender(war->getHandle(), allyOf))
	{
		for (auto& defender : war->m_Defenders)
		{
			for (auto& defenderAlly : getAlliances(allyOf))
			{
				if (defenderAlly == potentialAlly)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool DiplomacyManager::isAttacker(int warHandle, CharacterID ent)
{
	War* war = getWar(warHandle);

	if (war == nullptr)
	{
		return false;
	}

	for (auto ID : war->m_Attackers)
	{
		if (ent == ID)
		{
			return true;
		}
	}

	return false;
}

bool DiplomacyManager::isDefender(int warHandle, CharacterID ent)
{
	War* war = getWar(warHandle);

	if (war == nullptr)
	{
		return false;
	}

	for (auto ID : war->m_Defenders)
	{
		if (ent == ID)
		{
			return true;
		}
	}

	return false;
}

bool DiplomacyManager::hasTruce(CharacterID char1, CharacterID char2)
{
	for (auto& truce : m_ActiveTruces)
	{
		if (truce.m_Char1 == char1 && truce.m_Char2 == char2 || truce.m_Char1 == char2 && truce.m_Char2 == char1)
		{
			return true;
		}
	}

	return false;
}

CharacterID DiplomacyManager::getOpposingForce(int warHandle, CharacterID ID)
{
	if (isAttacker(warHandle, ID))
	{
		return getDefender(warHandle);
	}

	else if (isDefender(warHandle, ID))
	{
		return getAttacker(warHandle);
	}

 	return INVALID_CHARACTER_ID;
}

CharacterID DiplomacyManager::getAttacker(int warHandle)
{
	War* war = getWar(warHandle);

	if (war == nullptr)
	{
		return INVALID_CHARACTER_ID;
	}

	return war->m_Attackers[0];
}

CharacterID DiplomacyManager::getDefender(int warHandle)
{
	War* war = getWar(warHandle);

	if (war == nullptr)
	{
		return INVALID_CHARACTER_ID;
	}

	return war->m_Defenders[0];
}

std::vector<int> DiplomacyManager::getWarsForRegion(int regionID)
{
	std::vector<int> wars;

	for (auto& war : m_Wars)
	{
		if (war.second.m_WargoalRegion == regionID)
		{
			wars.push_back(war.second.getHandle());
		}
	}

	return wars;
}

std::vector<int> DiplomacyManager::getWarHandlesOfCharacter(CharacterID ID)
{
	std::vector<int> wars;

	for (auto& war : m_Wars)
	{
		if(isAttacker(war.second.getHandle(), ID) || isDefender(war.second.getHandle(), ID))
		{
			wars.push_back(war.second.getHandle());
		}
	}

	return wars;
}

std::vector<CharacterID> DiplomacyManager::getOpposingSide(CharacterID ID)
{
	std::vector<CharacterID> enemies;
	std::vector<int> wars = getWarHandlesOfCharacter(ID);

	for (auto& war : wars)
	{
		if (isAttacker(war, ID))
		{
			for (auto defender : getWar(war)->m_Defenders)
			{
				enemies.push_back(defender);
			}
		}

		if (isDefender(war, ID))
		{
			for (auto attacker : getWar(war)->m_Attackers)
			{
				enemies.push_back(attacker);
			}
		}
	}

	return enemies;
}

void DiplomacyManager::removeAllyFromWar(CharacterID ally, int warHandle)
{
	War* war = getWar(warHandle);

	if (isAttacker(war->getHandle(), ally))
	{
		unsigned int index = 0;

		for (auto& allied : war->m_Attackers)
		{
			if (allied == ally)
			{
				war->m_Attackers.erase(war->m_Attackers.begin() + index);
				return;
			}

			index++;
		}
	}

	else if (isDefender(war->getHandle(), ally))
	{
		unsigned int index = 0;

		for (auto& allied : war->m_Defenders)
		{
			if (allied == ally)
			{
				war->m_Defenders.erase(war->m_Defenders.begin() + index);
				return;
			}

			index++;
		}
	}
}

War* DiplomacyManager::getWarAgainst(CharacterID character, CharacterID enemy)
{
	for (auto& pair : m_Wars)
	{
		if (isAttacker(pair.second.getHandle(), character) && isDefender(pair.second.getHandle(), enemy))
		{
			return &pair.second;
		}

		else if (isDefender(pair.second.getHandle(), character) && isAttacker(pair.second.getHandle(), enemy))
		{
			return &pair.second;
		}
	}

	return nullptr;
}

int DiplomacyManager::getWarHandleAgainst(CharacterID character, CharacterID enemy)
{
	for (auto& pair : m_Wars)
	{
		if (isAttacker(pair.second.getHandle(), character) && isDefender(pair.second.getHandle(), enemy))
		{
			return pair.second.getHandle();
		}

		else if (isDefender(pair.second.getHandle(), character) && isAttacker(pair.second.getHandle(), enemy))
		{
			return pair.second.getHandle();
		}
	}

	return -1;
}

void DiplomacyManager::createAlliance(const CharacterID& characterOneID, const CharacterID& characterTwoID)
{
	unsigned int size = m_Alliances[characterOneID].size();
	for (unsigned int index = 0; index < size; index++)
	{
		if (m_Alliances[characterOneID][index] == characterTwoID)
		{
			if (CharacterManager::get().getCharacter(characterOneID).m_IsPlayerControlled)
			{
				UIManager::get().createUIEventElement(characterTwoID, characterOneID, UIType::AllianceDeclined);
			}
			return;
		}
	}
	m_Alliances[characterOneID].push_back(characterTwoID);
	m_Alliances[characterTwoID].push_back(characterOneID);
}

void DiplomacyManager::breakAlliance(const CharacterID& characterOneID, const CharacterID& characterTwoID)
{
	removeAlly(characterOneID, characterTwoID);
	removeAlly(characterTwoID, characterOneID);
}

std::vector<CharacterID> DiplomacyManager::getAlliances(const CharacterID& character)
{
	return m_Alliances[character];
}

void DiplomacyManager::makeTruce(CharacterID char1, CharacterID char2)
{
	Truce truce(Time::m_GameDate.m_Date, char1, char2, m_TruceHandle += 1);
	m_ActiveTruces.push_back(truce);
}

void DiplomacyManager::endTruce(int truceHandle)
{
	unsigned int index = 0;
	for (auto& truce : m_ActiveTruces)
	{
		if (truce.m_Handle == truceHandle)
		{
			break;
		}

		index++;
	}

	m_ActiveTruces.erase(m_ActiveTruces.begin() + index);
}

void DiplomacyManager::endTruces(std::vector<Truce> trucesToEnd)
{
	for (auto& truce : trucesToEnd)
	{
		endTruce(truce.m_Handle);
	}
}

void DiplomacyManager::eraseWar(int handle)
{
	int index = 0;

	for (auto& war : m_Wars)
	{
		if (war.second.getHandle() == handle)
		{
			break;
		}

		index++;
	}

	m_Wars.erase(m_Wars.begin() + index);
}

void DiplomacyManager::onMonthChange(Date currentDate)
{
	std::vector<Truce> trucesToEnd;

	for (auto& truce : m_ActiveTruces)
	{
		if (currentDate.m_Month - truce.m_StartDate.m_Month >= 2) 
		{
			trucesToEnd.push_back(truce);
		}
	}

	if (!trucesToEnd.empty())
	{
		endTruces(trucesToEnd);
	}
}

void DiplomacyManager::removeAlly(CharacterID character, CharacterID ally)
{
	int index = 0;
	bool foundAlly = false;
	for (const CharacterID& c : m_Alliances[character])
	{
		if (c == ally)
		{
			foundAlly = true;
			break;
		}
		index++;
	}

	ASSERT(foundAlly, "Given alliance doesn't exist");

	m_Alliances[character].erase(m_Alliances[character].begin() + index);
}
