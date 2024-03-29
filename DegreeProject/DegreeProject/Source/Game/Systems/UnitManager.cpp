#include "UnitManager.h"

#include "Game/Map/Map.h"
#include "Game/Data/Unit.h"
#include "Engine/AssetHandler.h"
#include "Engine/Log.h"
#include "Engine/Time.h"
#include "Game/DiplomacyManager.h"
#include "Game/UI/UIManager.h"
#include "Game/AI/AIManager.h"
#include "Game/Game.h"
#include "Engine/InputHandler.h"
#include "Game/Player.h"

#include <sstream>

UnitManager* UnitManager::m_Instance = nullptr;
UnitID UnitManager::m_UnitIDs = INVALID_UNIT_ID + 1;

void UnitManager::start()
{
	m_UnitTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/soldier unit.png");
	m_BoatTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Boat.png");
	m_UnitSprite.setTexture(m_UnitTexture);
	m_UnitLinePath.setPrimitiveType(sf::LineStrip);
}

void UnitManager::update()
{
	Date currentDate = Time::m_GameDate.m_Date;

	for (auto& unit : m_Units)
	{
		Character& character = CharacterManager::get().getCharacter(unit.m_Owner);

		if (character.m_CharacterID == INVALID_UNIT_ID)
		{
			dismissUnit(unit.m_UnitID);
			continue;
		}

		if (m_LastDay < currentDate)
		{
			if (character.m_CurrentGold < 0)
			{
				unit.m_RepresentedForce -= 1;
			}
			else if (!unit.m_Raised)
			{
				unit.m_RepresentedForce += 1;
			}

			if (unit.m_RepresentedForce > (int)character.m_MaxArmySize)
			{
				unit.m_RepresentedForce = character.m_MaxArmySize;
			}
			else if (unit.m_RepresentedForce < 0)
			{
				unit.m_RepresentedForce = 0;
			}
		}

		if (!unit.m_Raised)
		{
			continue;
		}

		// Move unit
		moveUnit(unit);

		if (unit.m_Owner == CharacterManager::get().getPlayerCharacterID())
		{
			showLinePath(unit);
		}
		// Engage enemy
		unitCombat(unit);

		// Siege
		startConquerRegion(unit);

		unitSiege(unit);

		updateSprite(unit);

		updateUnitUI(unit);

		if (unit.m_InCombat)
		{
			unit.m_CombatTimerAccu += Time::deltaTime();
			if (unit.m_CombatTimerAccu > unit.m_CombatTimer)
			{
				determineCombat(unit.m_UnitID, unit.m_FightingArmyID);
				unit.m_CombatTimerAccu = 0;
			}
		}
	}

	m_LastDay = currentDate;

}

#pragma warning(push)
#pragma warning(disable: 26812)
void UnitManager::render()
{
	for (auto& unit : m_Units)
	{
		if (!unit.m_Raised)
		{
			continue;
		}

		Window::getWindow()->draw(unit.m_Sprite);

		if (unit.m_Selected)
		{
			Window::getWindow()->draw(unit.m_HighlightShape);
		}

		Window::getWindow()->draw(unit.m_StrengthShape);
		Window::getWindow()->draw(unit.m_StrengthText);

		if (unit.m_Moving && unit.m_Owner == CharacterManager::get().getPlayerCharacterID())
		{
			Window::getWindow()->draw(m_UnitLinePath);
		}

		if (unit.m_SeizingRegionID != INVALID_REGION_ID)
		{
			MapRegion& region = Map::get().getRegionById(unit.m_SeizingRegionID);
			int daysToSiegeRegion = region.m_DaysToSeize;

			for (auto& slot : region.m_BuildingSlots)
			{
				if (slot.m_BuildingId == INVALID_BUILDING_ID)
				{
					continue;
				}

				Building& building = GameData::m_Buildings[slot.m_BuildingId];
				
				if (!building.m_Finished)
				{
					continue;
				}

				daysToSiegeRegion += building.m_HoldingModifier;
			}

			displayProgressMeter(unit, (float)unit.m_DaysSeizing, (float)daysToSiegeRegion, { m_SeizeMeterOffset.x, m_SeizeMeterOffset.y }, 0.0f, m_SeizeMeterFillColor);
		}

		if(unit.m_InCombat)
		{
			displayProgressMeter(unit, (float)unit.m_CombatTimerAccu, (float)unit.m_CombatTimer, { m_CombatMeterOffset.x, m_CombatMeterOffset.y }, -90.0f, m_CombatMeterFillColor);
		}
	}
}
#pragma warning(pop)

UnitManager& UnitManager::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new UnitManager();
	}

	return *m_Instance;
}

UnitID UnitManager::addUnit(CharacterID owner, int size)
{
	Unit newUnit;
	newUnit.m_Owner = owner;
	newUnit.m_UnitID = m_UnitIDs++;
	newUnit.m_RepresentedForce = size;

	updateSprite(newUnit);
	setupUnitUI(newUnit);
	m_Units.push_back(newUnit);

	return newUnit.m_UnitID;
}

#pragma warning(push)
#pragma warning(disable: 4239 4172)
Unit& UnitManager::getUnitOfCharacter(CharacterID character)
{
	for (auto& unit : m_Units)
	{
		if (unit.m_Owner == character)
		{
			return unit;
		}
	}

	ASSERT(false, "Given character doesn't have any units.");
	return Unit();
}
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable: 4239 4172)
Unit& UnitManager::getUnitWithId(UnitID id)
{
	for (auto& unit : m_Units)
	{
		if (unit.m_UnitID == id)
		{
			return unit;
		}
	}

	ASSERT(false, "No unit with given ID exists");

	return Unit();
}
#pragma warning(pop)

void UnitManager::raiseUnit(UnitID unitID, Vector2DInt location)
{
	if (getUnitWithId(unitID).m_Raised)
	{
		return;
	}

	if (!Map::get().mapSquareDataContainsKey(location))
	{		
		return;
	}

	Unit& unit = getUnitWithId(unitID);

	if (unit.m_RepresentedForce == 0)
	{
		return;
	}

	unit.m_Position = Map::get().convertToScreen(location);
	unit.m_LastPosition = unit.m_Position;
	unit.m_Raised = true;
	unit.m_Sprite.setColor(CharacterManager::get().getCharacter(getUnitWithId(unitID).m_Owner).m_RegionColor);

	for (auto& squareData : Map::get().m_MapSquareData)
	{
		if (squareData.m_Position != location)
		{
			continue;
		}

		squareData.addUnique(unitID);
		break;
	}

	if (unit.m_Owner == CharacterManager::get().getPlayerCharacterID())
	{
		CharacterManager::get().m_Player->selectUnit(unitID);
	}

	startConquerRegion(unit);
}

void UnitManager::dismissUnit(UnitID unitID)
{
	Unit& unit = getUnitWithId(unitID);

	for (auto& squareData : Map::get().m_MapSquareData)
	{
		if (squareData.m_Position != Map::convertToMap(unit.m_Position))
		{
			continue;
		}

		LOG_WARN("Removing dismissed unit");
		squareData.remove(unitID);
		break;
	}
	unit.m_Position = unit.m_Position.zero();
	unit.m_Position = unit.m_LastPosition.zero();
	unit.m_Moving = false;
	unit.m_CurrentPath.clear();
	unit.m_InCombat = false;
	unit.m_SeizingRegionID = INVALID_REGION_ID;
	unit.m_Raised = false;
	unit.m_DaysSeizing = 0;
}

void UnitManager::giveUnitPath(UnitID unitID, std::vector<Vector2DInt> path)
{
	if (path.size() <= 0)
	{
		return;
	}

	Unit& unit = getUnitWithId(unitID);

	if (unit.m_CurrentPath.size() > 0)
	{
		if (unit.m_CurrentPath.back() == path.back())
		{
			return;
		}
	}

	if (path.back() == Map::convertToMap(unit.m_Position))
	{
		if (unit.m_Moving)
		{
			unit.m_Moving = false;
			unit.m_CurrentPath.clear();
		}

		return;
	}

	Vector2D nextPosition = Map::convertToScreen(path.front());
	unit.m_Target = nextPosition;
	unit.m_Direction = (nextPosition - unit.m_Position).normalized();

	unit.m_CurrentPath = path;
	unit.m_Moving = true;

	if (unit.m_SeizingRegionID > 0)
	{
		unit.m_SeizingRegionID = INVALID_REGION_ID;
		unit.m_DaysSeizing = 0;
	}
}

void UnitManager::displayProgressMeter(Unit& unit, float timeElapsed, float totalTime, sf::Vector2f offset, float rotation, sf::Color fillColor)
{
	sf::Vector2 innerOffset(m_SeizeMeterInnerOffset.x, m_SeizeMeterInnerOffset.y);

	float innerWidth = m_ProgressMeterWidth - m_ProgressMeterDoubleBorder;
	innerWidth *= timeElapsed / totalTime;

	unit.m_InnerSeizeMeter.setRotation(rotation);
	unit.m_InnerSeizeMeter.setPosition(offset + innerOffset + sf::Vector2(unit.m_Position.x, unit.m_Position.y));
	unit.m_InnerSeizeMeter.setSize({ innerWidth, m_ProgressMeterHeight - m_ProgressMeterDoubleBorder });
	unit.m_InnerSeizeMeter.setFillColor(fillColor);
	unit.m_OuterSeizeMeter.setRotation(rotation);
	unit.m_OuterSeizeMeter.setSize({ m_ProgressMeterWidth, m_ProgressMeterHeight });
	unit.m_OuterSeizeMeter.setPosition(offset + sf::Vector2(unit.m_Position.x, unit.m_Position.y));
	unit.m_OuterSeizeMeter.setFillColor(m_ProgressMeterOuterColor);

	Window::getWindow()->draw(unit.m_OuterSeizeMeter);
	Window::getWindow()->draw(unit.m_InnerSeizeMeter);
}

void UnitManager::moveUnit(Unit& unit)
{
	if (!unit.m_Moving || unit.m_InCombat)
	{
		return;
	}

	if (!unit.m_Position.nearlyEqual(unit.m_Target, m_MoveTolerance))
	{
		unit.m_Position += unit.m_Direction * unit.m_Speed * Time::deltaTime();
	}

	else
	{
		if (Map::get().mapSquareDataContainsKey(Map::convertToMap(unit.m_LastPosition)))
		{
			for (auto& squareData : Map::get().m_MapSquareData)
			{
				if (squareData.m_Position == Map::convertToMap(unit.m_LastPosition))
				{
					squareData.remove(unit.m_UnitID);
					break;
				}
			}
		}

		unit.m_LastPosition = unit.m_Target;
		unit.m_Position = unit.m_Target;
		Vector2DInt pos = Map::convertToMap(unit.m_Target);

		if (Map::get().isWater(pos))
		{
			unit.m_OnWater = true;
		}

		else
		{
			unit.m_OnWater = false;
		}

		for (auto& squareData : Map::get().m_MapSquareData)
		{
			if (squareData.m_Position == pos)
			{
				squareData.addUnique(unit.m_UnitID);
				break;
			}
		}

		if (unit.m_CurrentPath.size() > 0)
		{
			Vector2D nextPosition = Map::convertToScreen(unit.m_CurrentPath.front());
			unit.m_Target = nextPosition;
			unit.m_Direction = (nextPosition - unit.m_Position).normalized();

			Vector2DInt mapPos = Map::convertToMap(unit.m_LastPosition);

			for (auto& squareData : Map::get().m_MapSquareData)
			{
				if (squareData.m_Position == mapPos)
				{
					squareData.addUnique(unit.m_UnitID);
					break;
				}
			}

			unit.m_CurrentPath.erase(unit.m_CurrentPath.begin());
		}
		else
		{
			unit.m_Moving = false;
		}
	}
}

void UnitManager::unitCombat(Unit& unit)
{
	unsigned int id = unitAtSquare(Map::get().convertToMap(unit.m_Position), unit.m_UnitID);

	if (DiplomacyManager::get().isAllied(unit.m_Owner, id))
	{
		return;
	}

	if (unit.m_FightingArmyID == id)
	{
		return;
	}

	if (id == INVALID_UNIT_ID)
	{
		return;
	}

	if (getUnitWithId(id).m_OnWater || unit.m_OnWater)
	{
		return;
	}

	if (DiplomacyManager::get().atWarWith(unit.m_Owner, getUnitWithId(id).m_Owner) || DiplomacyManager::get().isEnemyOfEnemy(unit, getUnitWithId(id)))
	{
		unit.m_FightingArmyID = id;
		getUnitWithId(id).m_FightingArmyID = unit.m_UnitID;
		startCombatTimer(unit.m_UnitID, id);
		return;
	}
}

void UnitManager::startCombatTimer(UnitID unitID, UnitID enemyUnitID)
{
	if (!getUnitWithId(unitID).m_InCombat && !getUnitWithId(enemyUnitID).m_InCombat)
	{
		Unit& unit = getUnitWithId(unitID);
		Unit& enemyUnit = getUnitWithId(enemyUnitID);

		unit.m_Moving = false;
		unit.m_InCombat = true;
		enemyUnit.m_Moving = false;
		enemyUnit.m_InCombat = true;
	}
}

UnitID UnitManager::unitAtSquare(Vector2DInt square, UnitID unitID)
{
	for (auto& squareData : Map::get().m_MapSquareData)
	{
		if (squareData.m_Position == square)
		{
			for (auto& ID : squareData.m_EntitiesInSquare)
			{
				if (unitID != ID)
				{
					return ID;
				}
			}

			break;
		}
	}

	return INVALID_UNIT_ID;
}

std::vector<UnitID> UnitManager::getAlliesAtSquare(const Character& character, Vector2DInt square)
{
	std::vector<UnitID> alliesInSquare;

	for (const auto& squareData : Map::get().m_MapSquareData)
	{
		if (squareData.m_Position == square)
		{
			for (const auto& ID : squareData.m_EntitiesInSquare)
			{
				for (const CharacterID& ally : DiplomacyManager::get().getAlliances(character.m_CharacterID))
				{
					if (ally == getUnitWithId(ID).m_Owner)
					{
						alliesInSquare.push_back(ID);
					}
				}
			}

			break;
		}
	}

	return alliesInSquare;
}

bool UnitManager::neutralUnitAtSquare(Character& character, Vector2DInt square)
{
	for (auto& squareData : Map::get().m_MapSquareData)
	{
		if (squareData.m_Position == square)
		{
			DiplomacyManager* warManager = &DiplomacyManager::get();

			for (auto& ID : squareData.m_EntitiesInSquare)
			{
				Unit& unit = getUnitWithId(ID);
				bool allyInSquare = false;

				for (const CharacterID& ally : warManager->getAlliances(character.m_CharacterID))
				{
					if (unit.m_Owner == ally)
					{
						allyInSquare = true;
						break;
					}
				}

				if (unit.m_Owner == character.m_CharacterID || allyInSquare)
				{
					continue;
				}

				War* potentialWar = warManager->getWarAgainst(character.m_CharacterID, unit.m_Owner);

				if (potentialWar == nullptr)
				{
					return true;
				}

				else
				{
					return false;
				}
			}
		}
	}

	return false;
}

void UnitManager::determineCombat(UnitID unitID, UnitID enemyID)
{
	DiplomacyManager* warManager = &DiplomacyManager::get();
	War* war = warManager->getWarAgainst(getUnitWithId(unitID).m_Owner, getUnitWithId(enemyID).m_Owner);

	float armyWeight;

	std::vector<UnitID> unitAllies = getAlliesAtSquare(CharacterManager::get().getCharacter(getUnitWithId(unitID).m_Owner), Map::get().convertToMap(getUnitWithId(unitID).m_Position));
	std::vector<UnitID> enemyAllies = getAlliesAtSquare(CharacterManager::get().getCharacter(getUnitWithId(enemyID).m_Owner), Map::get().convertToMap(getUnitWithId(enemyID).m_Position));

	float unitTotalForce = (float)getUnitWithId(unitID).m_RepresentedForce;
	float enemyTotalForce = (float)getUnitWithId(enemyID).m_RepresentedForce;

	bool leader = CharacterManager::get().getCharacter(getUnitWithId(unitID).m_Owner).m_LeadingArmy;
	bool enemyLeader = CharacterManager::get().getCharacter(getUnitWithId(enemyID).m_Owner).m_LeadingArmy;

	for (auto& unit : unitAllies)
	{
		unitTotalForce += getUnitWithId(unit).m_RepresentedForce;
		leader = CharacterManager::get().getCharacter(getUnitWithId(unit).m_Owner).m_LeadingArmy ? true : leader;
	}

	for (auto& unit : enemyAllies)
	{
		enemyTotalForce += getUnitWithId(unit).m_RepresentedForce;
		enemyLeader = CharacterManager::get().getCharacter(getUnitWithId(unit).m_Owner).m_LeadingArmy ? true : enemyLeader;
	}

	unitTotalForce = leader ? unitTotalForce * m_LeaderCombatMultiplier : unitTotalForce;
	enemyTotalForce = enemyLeader ? enemyTotalForce * m_LeaderCombatMultiplier : enemyTotalForce;

	if (getUnitWithId(unitID).m_RepresentedForce > 0 && getUnitWithId(enemyID).m_RepresentedForce > 0)
	{
		armyWeight = (unitTotalForce / enemyTotalForce) * (unitTotalForce / enemyTotalForce) * 0.5f;
	}

	else
	{
		armyWeight = 0.5f;
	}
	
	LOG_INFO("ARMY WEIGHT: {0}", armyWeight);
	bool win = weightedRandomCombat(armyWeight);

	//unitTotalForce > enemyTotalForce ? win = true : win = false;

	if (win)
	{
		dismissUnit(enemyID);
		getUnitWithId(enemyID).m_RepresentedForce = (int)(getUnitWithId(enemyID).m_RepresentedForce * 0.5f);

		if (war != nullptr)
		{
			if (warManager->isDefender(war->getHandle(), unitID))
			{
				warManager->addWarscore(war->getHandle(), getUnitWithId(unitID).m_Owner, 100);
			}

			else
			{
				warManager->addWarscore(war->getHandle(), getUnitWithId(unitID).m_Owner, 50);
			}

			LOG_INFO("{0} won the battle against {1}", CharacterManager::get().getCharacter(getUnitWithId(unitID).m_Owner).m_Name, CharacterManager::get().getCharacter(getUnitWithId(enemyID).m_Owner).m_Name);
		}
	}

	else
	{
		LOG_INFO("{0} won the battle against {1}", CharacterManager::get().getCharacter(getUnitWithId(enemyID).m_Owner).m_Name, CharacterManager::get().getCharacter(getUnitWithId(unitID).m_Owner).m_Name);
		dismissUnit(unitID);
		getUnitWithId(unitID).m_RepresentedForce = (int)(getUnitWithId(unitID).m_RepresentedForce * 0.5f);
		
		if (war != nullptr)
		{
			if (warManager->isDefender(war->getHandle(), enemyID))
			{
				warManager->addWarscore(war->getHandle(), getUnitWithId(enemyID).m_Owner, 100);
			}

			else
			{
				warManager->addWarscore(war->getHandle(), getUnitWithId(enemyID).m_Owner, 50);
			}
		}
	}

	getUnitWithId(unitID).m_FightingArmyID = INVALID_UNIT_ID;
	getUnitWithId(enemyID).m_FightingArmyID = INVALID_UNIT_ID;
	getUnitWithId(unitID).m_CombatTimerAccu = 0.0f;
	getUnitWithId(enemyID).m_CombatTimerAccu = 0.0f;
	getUnitWithId(unitID).m_InCombat = false;
	getUnitWithId(enemyID).m_InCombat = false;
}

void UnitManager::unitSiege(Unit& unit)
{
	if (unit.m_SeizingRegionID <= 0 || unit.m_InCombat)
	{
		return;
	}

	if (unit.m_LastSeizeDate < Time::m_GameDate.m_Date)
	{
		MapRegion& region = Map::get().getRegionById(unit.m_SeizingRegionID);

		unit.m_DaysSeizing++;
		unit.m_LastSeizeDate = Time::m_GameDate.m_Date;

		War* war = DiplomacyManager::get().getWarAgainst(unit.m_Owner, region.m_OwnerID);

		bool skipWarCheck = region.m_OccupiedBy != INVALID_CHARACTER_ID && region.m_OwnerID == unit.m_Owner;

		if (war == nullptr && !skipWarCheck)
		{
			unit.m_DaysSeizing = 0;
			unit.m_LastSeizeDate = Time::m_GameDate.m_Date;
			dismissUnit(unit.m_UnitID);
			return;
		}

		int daysToSiegeRegion = region.m_DaysToSeize;

		for (auto& slot : region.m_BuildingSlots)
		{
			if (slot.m_BuildingId == INVALID_BUILDING_ID)
			{
				continue;
			}

			Building& building = GameData::m_Buildings[slot.m_BuildingId];

			if (!building.m_Finished)
			{
				continue;
			}

			daysToSiegeRegion += building.m_HoldingModifier;
		}

		if (unit.m_DaysSeizing >= daysToSiegeRegion)
		{
			CharacterManager& characterManager = CharacterManager::get();
			Character& attacker = characterManager.getCharacter(unit.m_Owner);
			Character& defender = characterManager.getCharacter(region.m_OwnerID);
			int currentWarHandle = DiplomacyManager::get().getWarHandleAgainst(attacker.m_CharacterID, defender.m_CharacterID);
			DiplomacyManager& warManager = DiplomacyManager::get();

			if (currentWarHandle == -1 && !skipWarCheck)
			{
				return;
			}

			if (unit.m_Owner == region.m_OwnerID)
			{
				War* siegedWar = DiplomacyManager::get().getWarAgainst(unit.m_Owner, region.m_OccupiedBy);

				if (siegedWar != nullptr)
				{
					warManager.addWarscore(siegedWar->getHandle(), region.m_OccupiedBy, -50);
				}

				region.m_OccupiedBy = INVALID_CHARACTER_ID;

				attacker.m_MaxArmySize += region.m_ManPower;

				unit.m_DaysSeizing = 0;
				unit.m_SeizingRegionID = INVALID_REGION_ID;

				return;
			}

			if (defender.m_CharacterID == region.m_OwnerID)
			{
				war = warManager.getWar(currentWarHandle);

				war->m_AttackerOccupiedRegions.push_back(region.m_RegionId);

				if (war->m_AttackerWarscore < 100)
				{
					warManager.addWarscore(war->getHandle(), attacker.m_CharacterID, 50);
					warManager.addWarscore(war->getHandle(), defender.m_CharacterID, -50);
				}

				if (!DiplomacyManager::get().isValidWar(currentWarHandle))
				{
					return;
				}

				region.m_OccupiedBy = attacker.m_CharacterID;

				//Loot
				defender.m_CurrentGold -= region.m_RegionTax;
				attacker.m_CurrentGold += region.m_RegionTax;

				//defender.m_MaxArmySize -= region.m_ManPower;

				bool allRegionsSiezed = true;

				for (auto ID : CharacterManager::get().getCharacter(warManager.getDefender(war->getHandle())).m_OwnedRegionIDs)
				{
					if (Map::get().getRegionById(ID).m_OccupiedBy == INVALID_CHARACTER_ID)
					{
						allRegionsSiezed = false;
					}
				}

				if (allRegionsSiezed)
				{
					warManager.addWarscore(war->getHandle(), warManager.getAttacker(war->getHandle()), 100);
					warManager.addWarscore(war->getHandle(), warManager.getDefender(war->getHandle()), -100);
				}
			}

			else if (attacker.m_CharacterID == region.m_OwnerID)
			{
				war = warManager.getWar(currentWarHandle);

				war->m_DefenderOccupiedRegions.push_back(region.m_RegionId);
				warManager.addWarscore(currentWarHandle, warManager.getDefender(currentWarHandle), 50);
				region.m_OccupiedBy = defender.m_CharacterID;

				//Loot
				attacker.m_CurrentGold -= region.m_RegionTax;
				defender.m_CurrentGold += region.m_RegionTax;

				//attacker.m_MaxArmySize -= region.m_ManPower;

				bool allRegionsSiezed = true;

				for (auto ID : CharacterManager::get().getCharacter(warManager.getDefender(war->getHandle())).m_OwnedRegionIDs)
				{
					if (Map::get().getRegionById(ID).m_OccupiedBy == INVALID_CHARACTER_ID)
					{
						allRegionsSiezed = false;
					}
				}

				if (allRegionsSiezed)
				{
					warManager.addWarscore(war->getHandle(), warManager.getDefender(war->getHandle()), 100);
					warManager.addWarscore(war->getHandle(), warManager.getAttacker(war->getHandle()), -100);
				}
			}

			unit.m_DaysSeizing = 0;
			unit.m_SeizingRegionID = INVALID_REGION_ID;
		}
	}
}

void UnitManager::startConquerRegion(Unit& unit)
{
	if (unit.m_Moving || unit.m_SeizingRegionID != INVALID_REGION_ID || unit.m_InCombat)
	{
		return;
	}

	std::vector<int> regionIDs = Map::get().getRegionIDs();
	Vector2DInt currentMapPosition = Map::convertToMap(unit.m_Position);

	for each (int regionID in regionIDs)
	{
		Vector2DInt capitalPosition = Map::get().getRegionCapitalLocation(regionID);
		if (currentMapPosition == capitalPosition)
		{
			MapRegion& region = Map::get().getRegionById(regionID);
			unsigned int ownerID = region.m_OwnerID;

			if (ownerID == unit.m_Owner && region.m_OccupiedBy == INVALID_CHARACTER_ID)
			{
				continue;
			}

			if (region.m_OccupiedBy != INVALID_CHARACTER_ID && CharacterManager::get().isAlliedWith(region.m_OccupiedBy, unit.m_Owner))
			{
				continue;
			}

			if (region.m_OccupiedBy == unit.m_Owner)
			{
				continue;
			}

			bool allyFoundSieging = false;
			Character& unitOwner = CharacterManager::get().getCharacter(unit.m_Owner);

			std::vector alliesInSquare = getAlliesAtSquare(unitOwner, currentMapPosition);

			if (alliesInSquare.size() > 0)
			{
				for (const UnitID& ally : alliesInSquare)
				{
					if (getUnitWithId(ally).m_SeizingRegionID == regionID)
					{
						allyFoundSieging = true;
						break;
					}
				}
			}

			if (allyFoundSieging)
			{
				continue;
			}

			bool skipWarCheck = region.m_OccupiedBy != INVALID_CHARACTER_ID;

			War* war = DiplomacyManager::get().getWarAgainst(unit.m_Owner, ownerID);

			if (war == nullptr && !skipWarCheck)
			{
				continue;
			}

			unit.m_SeizingRegionID = regionID;
			unit.m_LastSeizeDate = Time::m_GameDate.m_Date;
		}
	}
}

bool UnitManager::weightedRandomCombat(float weight)
{
	float f = rand() * 1.0f / RAND_MAX;
	return f < weight;
}

void UnitManager::updateSprite(Unit& unit)
{
	unit.m_Sprite.setTexture(unit.m_OnWater ? m_BoatTexture : m_UnitTexture, true);
	unit.m_Sprite.setPosition({ unit.m_Position.x, unit.m_Position.y });

	sf::FloatRect localSize = unit.m_Sprite.getLocalBounds();

	unit.m_Sprite.setTextureRect(unit.m_Direction.x > 0 ? sf::IntRect((int)localSize.width, 0, -(int)localSize.width, (int)localSize.width) : sf::IntRect(0, 0, (int)localSize.width, (int)localSize.width));

	unit.m_Sprite.setScale(
		m_SpriteSize / localSize.width,
		m_SpriteSize / localSize.height
	);
}

void UnitManager::showLinePath(Unit& unit)
{
	m_UnitLinePath.clear();
	for each (Vector2DInt position in unit.m_CurrentPath)
	{
		Vector2D screenPosition = Map::convertToScreen(position);
		m_UnitLinePath.append(sf::Vertex({ screenPosition.x + m_SpriteSize * 0.5f, screenPosition.y + m_SpriteSize * 0.5f }));
	}
}

void UnitManager::setupUnitUI(Unit& unit)
{
	unit.m_StrengthShape.setSize({ unit.m_HighlightShapeSize.x, unit.m_HighlightShapeSize.y * 0.5f });
	unit.m_StrengthShape.setOutlineColor(unit.m_StrengthOutlineColor);

	unit.m_StrengthText.setFont(Game::m_UIFont);
	unit.m_StrengthText.setCharacterSize(unit.m_CharacterSize);
	unit.m_StrengthText.setFillColor(unit.m_OutlineColor);
	unit.m_StrengthText.setOutlineColor(unit.m_StrengthOutlineColor);

	unit.m_HighlightShape.setFillColor(unit.m_TransparentColor);
	unit.m_HighlightShape.setOutlineColor(unit.m_OutlineColor);
	unit.m_HighlightShape.setSize(unit.m_HighlightShapeSize);
}

void UnitManager::updateUnitUI(Unit& unit)
{
	unit.m_StrengthShape.setFillColor(CharacterManager::get().getCharacter(unit.m_Owner).m_RegionColor);
	unit.m_StrengthShape.setOutlineThickness(unit.m_OutlineThickness * InputHandler::m_TotalZoom);
	unit.m_StrengthShape.setPosition({ unit.m_Position.x, unit.m_Position.y + unit.m_HighlightShapeSize.y });

	std::stringstream stream;
	stream << unit.m_RepresentedForce;
	unit.m_StrengthText.setString(stream.str());
	stream.str(std::string());
	stream.clear();
	unit.m_StrengthText.setOrigin(unit.m_StrengthText.getLocalBounds().width * 0.5f, unit.m_StrengthShape.getSize().y * 0.9f);
	unit.m_StrengthText.setOutlineThickness(unit.m_OutlineThickness * InputHandler::m_TotalZoom);
	unit.m_StrengthText.setPosition(unit.m_StrengthShape.getPosition() + unit.m_StrengthShape.getSize() * 0.5f);

	unit.m_HighlightShape.setPosition({ unit.m_Position.x, unit.m_Position.y });
	unit.m_HighlightShape.setOutlineThickness(unit.m_OutlineThickness * InputHandler::m_TotalZoom);
}
