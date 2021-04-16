#include "UnitManager.h"

#include "Game/Map/Map.h"
#include "Game/Data/Unit.h"
#include "Engine/AssetHandler.h"
#include "Engine/Log.h"
#include "Engine/Time.h"
#include "Game/WarManager.h"
#include "Game/UI/UIManager.h"
#include "Game/AI/AIManager.h"

UnitManager* UnitManager::m_Instance = nullptr;
UnitID UnitManager::m_UnitIDs = INVALID_UNIT_ID + 1;

void UnitManager::start()
{
	m_UnitTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/soldier unit.png");
	m_UnitSprite.setTexture(m_UnitTexture);

	for (auto& unit : m_Units)
	{
		unit.m_RepresentedForce = CharacterManager::get()->getCharacter(unit.m_Owner).m_MaxArmySize;
	}
}

void UnitManager::update()
{
	Date currentDate = Time::m_GameDate.m_Date;

	for (auto& unit : m_Units)
	{
		if (!unit.m_Raised && (unsigned int)unit.m_RepresentedForce < CharacterManager::get()->getCharacter(unit.m_Owner).m_MaxArmySize)
		{
			if (m_LastDay < currentDate)
			{
				unit.m_RepresentedForce += 1;

				if ((unsigned int)unit.m_RepresentedForce > CharacterManager::get()->getCharacter(unit.m_Owner).m_MaxArmySize)
				{
					unit.m_RepresentedForce = CharacterManager::get()->getCharacter(unit.m_Owner).m_MaxArmySize;
				}
			}
		}

		if (!unit.m_Raised)
		{
			continue;
		}

		// Move unit
		moveUnit(unit);
		showPath(unit);
		// Engage enemy
		unitCombat(unit);
		// Siege
		
		//if (unit.m_Owner == CharacterManager::get()->getPlayerCharacter().m_CharacterID)
		//{
		//	unitSiege(unit);
		//}

		startConquerRegion(unit);

		unitSiege(unit);

		updateSprite(unit);

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


		if (unit.m_Moving)
		{
			for each (sf::RectangleShape rectangle in unit.m_TargetPath)
			{
				Window::getWindow()->draw(rectangle);
			}
		}

		if (unit.m_Selected)
		{
			unit.m_HighlightShape.setPosition({ unit.m_Position.x, unit.m_Position.y });
			unit.m_HighlightShape.setOutlineThickness(1.0f);
			unit.m_HighlightShape.setFillColor(unit.m_FillColor); // Rename fillcolor to highlight fill
			unit.m_HighlightShape.setOutlineColor(unit.m_OutlineColor); // Rename outline color to highlight border
			unit.m_HighlightShape.setSize(unit.m_HighlightShapeSize);
			Window::getWindow()->draw(unit.m_HighlightShape);
		}

		if (unit.m_SeizingRegionID != -1)
		{
			MapRegion& region = Map::get().getRegionById(unit.m_SeizingRegionID);
			int daysToSiegeRegion = region.m_DaysToSeize;

			for (auto& slot : region.m_BuildingSlots)
			{
				if (slot.m_BuildingId == -1)
				{
					continue;
				}

				Building& building = GameData::m_Buildings[slot.m_BuildingId];
				daysToSiegeRegion += building.m_HoldingModifier;
			}

			displayProgressMeter(unit, (float)unit.m_DaysSeizing, (float)daysToSiegeRegion, { m_SeizeMeterOffset.x, m_SeizeMeterOffset.y }, m_SeizeMeterFillColor);
		}

		if(unit.m_InCombat)
		{
			displayProgressMeter(unit, (float)unit.m_CombatTimerAccu, (float)unit.m_CombatTimer, { m_CombatMeterOffset.x, m_CombatMeterOffset.y }, m_CombatMeterFillColor);
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

	newUnit.m_Sprite.setTexture(m_UnitTexture);
	newUnit.m_Sprite.setPosition({ newUnit.m_Position.x, newUnit.m_Position.y });
	updateSprite(newUnit);

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
	if (!Map::get().mapSquareDataContainsKey(location))
	{		
		return;
	}

	//if (unitID == INVALID_UNIT_ID)
	//{
	//	return; // Todo: This is wrong. Find out how an invalid id gets sent in
	//}

	Unit& unit = getUnitWithId(unitID);

	if (unit.m_RepresentedForce == 0)
	{
		return;
	}

	unit.m_Position = Map::get().convertToScreen(location);
	unit.m_Raised = true;
	unit.m_Sprite.setColor(CharacterManager::get()->getCharacter(getUnitWithId(unitID).m_Owner).m_RegionColor);

	for (auto& squareData : Map::get().m_MapSquareData)
	{
		if (squareData.m_Position != location)
		{
			continue;
		}

		squareData.addUnique(unitID);
		break;
	}

	startConquerRegion(unit);

	CharacterManager::get()->getCharacter(getUnitWithId(unitID).m_Owner).m_RaisedArmySize = unit.m_RepresentedForce;
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

	unit.m_Moving = false;
	unit.m_CurrentPath.clear();
	unit.m_InCombat = false;
	unit.m_SeizingRegionID = -1;
	unit.m_Raised = false;
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
		unit.m_SeizingRegionID = -1;
		unit.m_DaysSeizing = 0;
	}
}

void UnitManager::displayProgressMeter(Unit& unit, float timeElapsed, float totalTime, sf::Vector2f offset, sf::Color fillColor)
{
	sf::Vector2 innerOffset(m_SeizeMeterInnerOffset.x, m_SeizeMeterInnerOffset.y);

	float innerWidth = m_ProgressMeterWidth - m_ProgressMeterDoubleBorder;
	innerWidth *= timeElapsed / totalTime;

	unit.m_InnerSeizeMeter.setPosition(offset + innerOffset + sf::Vector2(unit.m_Position.x, unit.m_Position.y));
	unit.m_InnerSeizeMeter.setSize({ innerWidth, m_ProgressMeterHeight - m_ProgressMeterDoubleBorder });
	unit.m_InnerSeizeMeter.setFillColor(fillColor);
	unit.m_OuterSeizeMeter.setSize({ m_ProgressMeterWidth, m_ProgressMeterHeight });
	unit.m_OuterSeizeMeter.setPosition(offset + sf::Vector2(unit.m_Position.x, unit.m_Position.y));
	unit.m_OuterSeizeMeter.setFillColor(m_ProgressMeterOuterColor);

	Window::getWindow()->draw(unit.m_OuterSeizeMeter);
	Window::getWindow()->draw(unit.m_InnerSeizeMeter);
}

void UnitManager::moveUnit(Unit& unit)
{
	if (!unit.m_Moving)
	{
		return;
	}

	// showPath(transform, unit); <- debug. add functionality

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
	int id = unitAtSquare(Map::get().convertToMap(unit.m_Position), unit.m_UnitID);

	if (id == INVALID_UNIT_ID)
	{
		return;
	}

	if (WarManager::get().atWarWith(unit.m_Owner, getUnitWithId(id).m_Owner) || WarManager::get().isEnemyOfEnemy(unit, getUnitWithId(id)))
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
				for (const CharacterID& ally : WarManager::get().getAlliances(character.m_CharacterID))
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
			WarManager* warManager = &WarManager::get();

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
	WarManager* warManager = &WarManager::get();
	War* war = warManager->getWarAgainst(getUnitWithId(unitID).m_Owner, getUnitWithId(enemyID).m_Owner);

	float armyWeight;

	if (getUnitWithId(unitID).m_RepresentedForce > 0 && getUnitWithId(enemyID).m_RepresentedForce > 0)
	{
		armyWeight = (float)getUnitWithId(unitID).m_RepresentedForce / getUnitWithId(enemyID).m_RepresentedForce;
	}

	else
	{
		armyWeight = 0.5f;
	}
	
	LOG_INFO("ARMY WEIGHT: {0}", armyWeight);
	bool win = weightedRandomCombat(armyWeight);

	if (win)
	{
		dismissUnit(enemyID);
		getUnitWithId(enemyID).m_RepresentedForce = 0;

		if (war != nullptr)
		{
			if (war->isDefender(unitID))
			{
				war->addWarscore(getUnitWithId(unitID).m_Owner, 100);
			}

			else
			{
				war->addWarscore(getUnitWithId(unitID).m_Owner, 50);
			}

			LOG_INFO("{0} won the battle against {1}", CharacterManager::get()->getCharacter(getUnitWithId(unitID).m_Owner).m_Name, CharacterManager::get()->getCharacter(getUnitWithId(enemyID).m_Owner).m_Name);
		}
	}

	else
	{
		LOG_INFO("{0} won the battle against {1}", CharacterManager::get()->getCharacter(getUnitWithId(unitID).m_Owner).m_Name, CharacterManager::get()->getCharacter(getUnitWithId(enemyID).m_Owner).m_Name);
		dismissUnit(unitID);
		getUnitWithId(unitID).m_RepresentedForce = 0;
		
		if (war != nullptr)
		{
			if (war->isDefender(enemyID))
			{
				war->addWarscore(getUnitWithId(enemyID).m_Owner, 100);
			}

			else
			{
				war->addWarscore(getUnitWithId(enemyID).m_Owner, 50);
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
	if (unit.m_SeizingRegionID <= 0)
	{
		return;
	}

	if (unit.m_LastSeizeDate < Time::m_GameDate.m_Date)
	{
		MapRegion& region = Map::get().getRegionById(unit.m_SeizingRegionID);

		if (!unit.m_InCombat || neutralUnitAtSquare(CharacterManager::get()->getCharacter(unit.m_Owner), region.m_RegionCapital))
		{
			unit.m_DaysSeizing++;
			unit.m_LastSeizeDate = Time::m_GameDate.m_Date;

			if (Map::get().mapSquareDataContainsKey(region.m_RegionCapital))
			{
				if (neutralUnitAtSquare(CharacterManager::get()->getCharacter(unit.m_Owner), region.m_RegionCapital))
				{
					unit.m_SeizingRegionID = -1;
					unit.m_DaysSeizing = 0;
					return;
				}
			}
			War* war = WarManager::get().getWarAgainst(unit.m_Owner, region.m_OwnerID);

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
				if (slot.m_BuildingId == -1)
				{
					continue;
				}

				Building& building = GameData::m_Buildings[slot.m_BuildingId];
				daysToSiegeRegion += building.m_HoldingModifier;
			}

			if ((unsigned int)unit.m_DaysSeizing >= region.m_DaysToSeize)
			{	
				CharacterManager* characterManager = CharacterManager::get();
				Character& attacker = characterManager->getCharacter(unit.m_Owner);
				Character& defender = characterManager->getCharacter(region.m_OwnerID);
				War* currentWar = WarManager::get().getWarAgainst(attacker.m_CharacterID, defender.m_CharacterID);

				if (currentWar == nullptr && !skipWarCheck)
				{
					return;
				}

				if (unit.m_Owner == region.m_OwnerID)
				{
					War* siegedWar = WarManager::get().getWarAgainst(unit.m_Owner, region.m_OccupiedBy);

					if (siegedWar != nullptr)
					{
						siegedWar->addWarscore(region.m_OccupiedBy, -50);
					}
					
					region.m_OccupiedBy = INVALID_CHARACTER_ID;

					attacker.m_MaxArmySize += region.m_ManPower;

					unit.m_DaysSeizing = 0;
					unit.m_SeizingRegionID = -1;

					return;
				}
				
				if (defender.m_CharacterID == region.m_OwnerID)
				{
					currentWar->m_AttackerOccupiedRegions.push_back(region.m_RegionId);

					if (currentWar->m_AttackerWarscore < 100)
					{
						currentWar->addWarscore(currentWar->getAttacker(), 50);
					}

					if (!WarManager::get().isValidWar(*currentWar))
					{
						return;
					}

					region.m_OccupiedBy = attacker.m_CharacterID;

					//Loot
					defender.m_CurrentGold -= region.m_RegionTax;
					attacker.m_CurrentGold += region.m_RegionTax;

					defender.m_MaxArmySize -= region.m_ManPower;

					bool allRegionsSiezed = true;

					for (auto ID : CharacterManager::get()->getCharacter(currentWar->getDefender()).m_OwnedRegionIDs)
					{
						if (Map::get().getRegionById(ID).m_OccupiedBy == INVALID_CHARACTER_ID)
						{
							allRegionsSiezed = false;
						}
					}

					if (allRegionsSiezed && currentWar->m_AttackerWarscore < 100)
					{
						currentWar->addWarscore(currentWar->getAttacker(), 100);
					}
				}

				else if (attacker.m_CharacterID == region.m_OwnerID)
				{
					currentWar->m_DefenderOccupiedRegions.push_back(region.m_RegionId);
					currentWar->addWarscore(currentWar->getDefender(), 50);
					region.m_OccupiedBy = defender.m_CharacterID;

					//Loot
					attacker.m_CurrentGold -= region.m_RegionTax;
					defender.m_CurrentGold += region.m_RegionTax;

					attacker.m_MaxArmySize -= region.m_ManPower;
				}

				unit.m_DaysSeizing = 0;
				unit.m_SeizingRegionID = -1;
			}
		}
	}
}

void UnitManager::startConquerRegion(Unit& unit)
{
	if (unit.m_Moving || unit.m_SeizingRegionID != -1)
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

			if (region.m_OccupiedBy != INVALID_CHARACTER_ID && CharacterManager::get()->isAlliedWith(region.m_OccupiedBy, unit.m_Owner))
			{
				continue;
			}

			if (region.m_OccupiedBy == unit.m_Owner)
			{
				continue;
			}

			bool allyFoundSieging = false;
			Character& unitOwner = CharacterManager::get()->getCharacter(unit.m_Owner);

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

			War* war = WarManager::get().getWarAgainst(unit.m_Owner, ownerID);

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
	float vv = weight / 5.0f;
	return f < vv;
}

void UnitManager::updateSprite(Unit& unit)
{
	unit.m_Sprite.setTexture(m_UnitTexture, true);
	unit.m_Sprite.setPosition({ unit.m_Position.x, unit.m_Position.y });

	sf::FloatRect localSize = unit.m_Sprite.getLocalBounds();

	unit.m_Sprite.setScale(
		32 / localSize.width, // Todo: Remove magic number
		32 / localSize.height
	);
}


void UnitManager::showPath(Unit& unit)
{
	unit.m_TargetPath.clear();
	std::vector<Vector2DInt> path;
	for each (Vector2DInt position in unit.m_CurrentPath)
	{
		path.push_back(position);
	}
	unsigned int index = 0;
	for each (Vector2DInt position in path)
	{
		Vector2D screenPosition = Map::convertToScreen(position);
		Vector2D rectangleSize = Vector2D(unit.m_HighlightShape.getSize().x * 1.45f, unit.m_HighlightShape.getSize().y * 0.25f);
		float rotation = 0.0f;
		Vector2DInt oldPosition = Vector2DInt();
		if (unit.m_TargetPath.size() > 0)
		{
			oldPosition = path[index - 1];
		}
		else
		{
			oldPosition = Map::convertToMap(unit.m_Position);
		}
		Vector2DInt change = position - oldPosition;
		if (abs(change.y) == 1 && change.x == 0)
		{
			rotation = 90.0f;
			screenPosition.x += unit.m_HighlightShape.getSize().x * 0.625f;
			screenPosition.y -= unit.m_HighlightShape.getSize().y * 0.375f;
			if (change.y < 0)
			{
				screenPosition.y += unit.m_HighlightShape.getSize().y * 0.5f;
			}
			else
			{
				screenPosition.y -= unit.m_HighlightShape.getSize().y * 0.5f;
			}
		}
		else if (change.x > 0 && change.y > 0 || change.x < 0 && change.y < 0)
		{
			rotation = 45.0f;
			screenPosition.x += unit.m_HighlightShape.getSize().x * 0.28125f;
			screenPosition.y -= unit.m_HighlightShape.getSize().y * 0.28125f;
			if (change.x > 0 && change.y > 0)
			{
				screenPosition.x -= unit.m_HighlightShape.getSize().x;
				screenPosition.y -= unit.m_HighlightShape.getSize().y;
			}
			if (change.x < 0 && change.y < 0)
			{
				screenPosition.x += unit.m_HighlightShape.getSize().x * 0.5f;
				screenPosition.y += unit.m_HighlightShape.getSize().y * 0.5f;
			}
		}
		else if (change.x > 0 && change.y < 0 || change.x < 0 && change.y > 0)
		{
			rotation = -45.0f;
			screenPosition.x += unit.m_HighlightShape.getSize().x * 0.21875f;
			screenPosition.y += unit.m_HighlightShape.getSize().y * 0.21875f;
			if (change.x > 0 && change.y < 0)
			{
				screenPosition.x -= unit.m_HighlightShape.getSize().x;
				screenPosition.y += unit.m_HighlightShape.getSize().y;
			}
		}

		sf::RectangleShape rectangle = sf::RectangleShape(sf::Vector2f(rectangleSize.x, rectangleSize.y));
		rectangle.setFillColor(sf::Color::White);
		rectangle.setRotation(rotation);
		rectangle.setPosition(screenPosition.x, screenPosition.y + unit.m_HighlightShape.getSize().y * 0.375f);
		unit.m_TargetPath.push_back(rectangle);
		index++;
	}
}
