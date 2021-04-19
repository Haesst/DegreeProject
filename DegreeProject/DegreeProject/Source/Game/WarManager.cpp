#include "WarManager.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/AI/AIManager.h"
#include "Game/Data/AIData.h"
#include "Game/Data/Character.h"
#include "Game/Data/Unit.h"
#include "Game/Map/Map.h"

WarManager* WarManager::m_Instance = nullptr;

int WarManager::createWar(CharacterID attacker, CharacterID defender, int warGoalRegion)
{
	int handle = m_Warhandle++;
	m_Wars.push_back(std::make_pair(handle, War(attacker, defender, warGoalRegion, handle)));
	return handle;
}

void WarManager::endWar(int warHandle, CharacterID winner)
{
	unsigned int index = 0;

	if (getWar(warHandle) == nullptr)
	{
		return;
	}

	if (winner != INVALID_CHARACTER_ID)
	{
		if (!CharacterManager::get()->getCharacter(getWar(warHandle)->getAttacker()).m_IsPlayerControlled)
		{
			AIManager::get().getAIDataofCharacter(getWar(warHandle)->getAttacker()).m_CurrentAction = Action::NONE;
		}

		if (!CharacterManager::get()->getCharacter(getWar(warHandle)->getDefender()).m_IsPlayerControlled)
		{
			AIManager::get().getAIDataofCharacter(getWar(warHandle)->getDefender()).m_CurrentAction = Action::NONE;
		}
	}

	if (getWar(warHandle)->getAttacker() == winner)
	{
		invalidateWarsForRegionOnWonWar(*getWar(warHandle));
	}
	
	getWar(warHandle)->endWar(winner);

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

War* WarManager::getWar(int handle)
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

bool WarManager::atWarWith(CharacterID character, CharacterID enemy)
{
	for (auto& war : m_Wars)
	{
		if ((war.second.isAttacker(character) || war.second.isDefender(character)) && 
			(war.second.isAttacker(enemy) || war.second.isDefender(enemy)))
		{
			return true;
		}
	}

	return false;
}

void WarManager::invalidateWarsForRegionOnWonWar(War& wonWar)
{
	for (auto& war : m_Wars)
	{
		if (war.second.m_WargoalRegion == wonWar.m_WargoalRegion)
		{
			if (war.second.getHandle() == wonWar.getHandle())
			{
				continue;
			}

			endWar(war.second.getHandle(), INVALID_CHARACTER_ID);
		}
	}
}

void WarManager::invalidateWarsForRegion(int regionID)
{
	for (auto& war : m_Wars)
	{
		if (war.second.m_WargoalRegion == regionID)
		{
			endWar(war.second.getHandle(), INVALID_CHARACTER_ID);
		}
	}
}

bool WarManager::isEnemyOfEnemy(Unit& unit, Unit& enemyUnit)
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

bool WarManager::isValidWar(War& war)
{
	for (auto ID : m_Wars)
	{
		if (ID.first == war.getHandle())
		{
			return true;
		}
	}

	return false;
}

void WarManager::update()
{
	for (auto& war : m_Wars)
	{
		if (Map::get().getRegionById(war.second.m_WargoalRegion).m_OccupiedBy == INVALID_CHARACTER_ID)
		{
			war.second.warGoalRegionTimerAccu += Time::deltaTime();

			if (war.second.warGoalRegionTimerAccu >= war.second.warGoalRegionCapturedTimer)
			{
				war.second.addWarscore(war.second.getDefender(), 5);
				war.second.warGoalRegionTimerAccu = 0;
			}
		}

		else if (Map::get().getRegionById(war.second.m_WargoalRegion).m_OccupiedBy != INVALID_CHARACTER_ID)
		{
			war.second.warGoalRegionTimerAccu += Time::deltaTime();

			if (war.second.warGoalRegionTimerAccu >= war.second.warGoalRegionCapturedTimer)
			{
				war.second.addWarscore(war.second.getAttacker(), 5);
				war.second.warGoalRegionTimerAccu = 0;
			}
		}
	}
}

std::vector<War> WarManager::getWarsOfCharacter(CharacterID ID)
{
	std::vector<War> wars;

	for (auto& war : m_Wars)
	{
		if (war.second.isAttacker(ID) || war.second.isDefender(ID))
		{
			wars.push_back(war.second);
		}
	}

	return wars;
}

std::vector<CharacterID> WarManager::getOpposingSide(CharacterID ID)
{
	std::vector<CharacterID> enemies;
	std::vector<War> wars = getWarsOfCharacter(ID);

	for (auto& war : wars)
	{
		if (war.isAttacker(ID))
		{
			for (auto defender : war.m_Defenders)
			{
				enemies.push_back(defender);
			}
		}

		if (war.isDefender(ID))
		{
			for (auto attacker : war.m_Attackers)
			{
				enemies.push_back(attacker);
			}
		}
	}

	return enemies;
}

War* WarManager::getWarAgainst(CharacterID character, CharacterID enemy)
{
	for (auto& pair : m_Wars)
	{
		if (pair.second.isAttacker(character) && pair.second.isDefender(enemy))
		{
			return &pair.second;
		}

		else if (pair.second.isDefender(character) && pair.second.isAttacker(enemy))
		{
			return &pair.second;
		}
	}

	return nullptr;
}

void WarManager::createAlliance(const CharacterID& characterOneID, const CharacterID& characterTwoID)
{
	m_Alliances[characterOneID].push_back(characterTwoID);
	m_Alliances[characterTwoID].push_back(characterOneID);
}

void WarManager::breakAlliance(const CharacterID& characterOneID, const CharacterID& characterTwoID)
{
	removeAlly(characterOneID, characterTwoID);
	removeAlly(characterTwoID, characterOneID);
}

std::vector<CharacterID> WarManager::getAlliances(const CharacterID& character)
{
	return m_Alliances[character];
}

void WarManager::removeAlly(CharacterID character, CharacterID ally)
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
