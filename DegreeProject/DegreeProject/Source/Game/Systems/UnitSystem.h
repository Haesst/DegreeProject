#pragma once
#include "ECS/System.h"
#include <Game\Components\Unit.h>
#include <Engine\AssetHandler.h>
#include "Game/WarManager.h"
#include "Game\Components\Player.h"
#include "Engine/InputHandler.h"
#include "Game/Systems/CharacterSystem.h"

struct UnitSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	UnitComponent* m_UnitComponents = nullptr;
	CharacterComponent* m_Characters = nullptr;
	WarmindComponent* m_Warminds = nullptr;
	SpriteRenderer* m_Renderers = nullptr;
	WarManager* m_WarManager = nullptr;

	float m_MoveTolerance = 0.3f;

	Vector2D m_SeizeMeterOffset = { 0.0f, -20.0f };
	Vector2D m_CombatMeterOffset = { 0.0f, 30.0f };
	Vector2D m_SeizeMeterInnerOffset = { 1.0f, 1.0f };

	float m_ProgressMeterWidth = 32.0f;
	float m_ProgressMeterHeight = 12.0f;
	float m_ProgressMeterBorder = 1.0f;
	float m_ProgressMeterDoubleBorder;

	sf::Color m_ProgressMeterOuterColor = sf::Color(25, 25, 25, 250);
	sf::Color m_SeizeMeterFillColor = sf::Color(40, 70, 170, 250);
	sf::Color m_CombatMeterFillColor = sf::Color(170, 70, 40, 250);

	UnitSystem()
	{
		addComponentSignature<UnitComponent>();
		m_EntityManager = &EntityManager::get();
		m_WarManager = &WarManager::get();
		m_UnitComponents = m_EntityManager->getComponentArray<UnitComponent>();
		m_Warminds = m_EntityManager->getComponentArray<WarmindComponent>();
		m_Characters = m_EntityManager->getComponentArray<CharacterComponent>();
		m_Renderers = m_EntityManager->getComponentArray<SpriteRenderer>();
		m_ProgressMeterDoubleBorder = m_ProgressMeterBorder * 2;
	}

	virtual void start() override
	{
		for (EntityID entity : m_Entities)
		{
			UnitComponent& unit = m_UnitComponents[entity];
			Transform& transform = m_EntityManager->getComponent<Transform>(entity);
			unit.m_HighlightShape.setFillColor(unit.m_FillColor);
			unit.m_HighlightShape.setOutlineColor(unit.m_OutlineColor);
			unit.m_HighlightShape.setOutlineThickness(unit.m_OutlineThickness);
			unit.m_HighlightShape.setSize(unit.m_HighlightShapeSize);
			unit.m_HighlightShape.setPosition(transform.m_Position.x, transform.m_Position.y);
		}
	}

	virtual void update() override
	{
		for (EntityID ent : m_Entities)
		{
			UnitComponent& unit = m_UnitComponents[ent];
			Transform& transform = m_EntityManager->getComponent<Transform>(ent);

			if (unit.m_Raised)
			{
				moveUnit(unit, transform);

				if (m_Characters[unit.m_Owner].m_AtWar)
				{
					seizeRegion(unit);
				}

				unit.m_HighlightShape.setFillColor(unit.m_FillColor);
				unit.m_HighlightShape.setOutlineColor(unit.m_OutlineColor);
				unit.m_HighlightShape.setOutlineThickness(unit.m_OutlineThickness);
				unit.m_HighlightShape.setSize(unit.m_HighlightShapeSize);
				unit.m_HighlightShape.setPosition(transform.m_Position.x, transform.m_Position.y);

				if (unit.m_PlayerSelected)
				{
					InputHandler::setPlayerUnitSelected(true);
				}

				if (unit.m_InCombat)
				{
					unit.m_CombatTimerAccu += Time::deltaTime();
					if (unit.m_CombatTimerAccu > unit.m_CombatTimer)
					{
						if (unit.EnemyArmy != nullptr)
						{
							determineCombat(unit.m_EntityID, unit.EnemyArmy->m_EntityID);
							unit.m_CombatTimerAccu = 0.0f;
						}
					}
				}
			}
		}
	}

	virtual void render() override
	{
		for (EntityID entity : m_Entities)
		{
			UnitComponent& unit = m_UnitComponents[entity];
			Window::getWindow()->draw(unit.m_HighlightShape);

			if (unit.m_Raised)
			{
				if (unit.m_Moving)
				{
					for each (sf::RectangleShape rectangle in unit.m_TargetPath)
					{
						Window::getWindow()->draw(rectangle);
					}
				}

				unit.m_EndPosition.setSize(sf::Vector2f(32.0f, 32.0f));
				unit.m_EndPosition.setFillColor(sf::Color::Red);

				if (unit.m_CurrentPath.size() > 0)
				{
					Vector2D pos = Map::convertToScreen(unit.m_CurrentPath.back());
					unit.m_EndPosition.setPosition(pos.x, pos.y);
					Window::getWindow()->draw(unit.m_EndPosition);
				}

				if (unit.m_SeizingRegionID > 0)
				{
					displayProgressMeter(unit, (float)unit.m_DaysSeizing, (float)Map::get().getRegionById(unit.m_SeizingRegionID).m_DaysToSeize, { m_SeizeMeterOffset.x, m_SeizeMeterOffset.y }, m_SeizeMeterFillColor);
				}

				if (unit.m_InCombat)
				{
					displayProgressMeter(unit, unit.m_CombatTimerAccu, unit.m_CombatTimer, { m_CombatMeterOffset.x, m_CombatMeterOffset.y }, m_CombatMeterFillColor);
				}
			}
		}
	}

	void displayProgressMeter(UnitComponent& unit, float timeElapsed, float totalTime, sf::Vector2f offset, sf::Color fillColor)
	{
		Transform& transform = EntityManager::get().getComponent<Transform>(unit.m_EntityID);

		sf::Vector2 innerOffset(m_SeizeMeterInnerOffset.x, m_SeizeMeterInnerOffset.y);

		float innerWidth = m_ProgressMeterWidth - m_ProgressMeterDoubleBorder;
		innerWidth *= timeElapsed / totalTime;

		unit.m_InnerSeizeMeter.setPosition(offset + innerOffset + sf::Vector2(transform.m_Position.x, transform.m_Position.y));
		unit.m_InnerSeizeMeter.setSize({ innerWidth, m_ProgressMeterHeight - m_ProgressMeterDoubleBorder });
		unit.m_InnerSeizeMeter.setFillColor(fillColor);
		unit.m_OuterSeizeMeter.setSize({ m_ProgressMeterWidth, m_ProgressMeterHeight });
		unit.m_OuterSeizeMeter.setPosition(offset + sf::Vector2(transform.m_Position.x, transform.m_Position.y));
		unit.m_OuterSeizeMeter.setFillColor(m_ProgressMeterOuterColor);

		Window::getWindow()->draw(unit.m_OuterSeizeMeter);
		Window::getWindow()->draw(unit.m_InnerSeizeMeter);
	}

	void moveUnit(UnitComponent& unit, Transform& transform)
	{
		if (!unit.m_Moving)
		{
			return;
		}

		showPath(transform, unit);

		if (!transform.m_Position.nearlyEqual(unit.m_Target, m_MoveTolerance))
		{
			Vector2D movement = unit.m_Direction * unit.m_Speed * Time::deltaTime();
			transform.translate(movement);
		}
		else
		{
			if (Map::get().mapSquareDataContainsKey(Map::convertToMap(unit.m_LastPosition)))
			{
				for (auto& squareData : Map::get().m_MapSquareData)
				{
					if (squareData.m_Position == Map::convertToMap(unit.m_LastPosition))
					{
						squareData.remove(unit.getID());
						break;
					}
				}
			}

			unit.m_LastPosition = unit.m_Target;
			transform.m_Position = unit.m_Target;
			Vector2DInt pos = Map::convertToMap(unit.m_Target);

			for (auto& squareData : Map::get().m_MapSquareData)
			{
				if (squareData.m_Position == pos)
				{
					squareData.addUnique(unit.m_EntityID);
					break;
				}
			}

			if (enemyAtSquare(pos, m_Warminds[unit.m_Owner].m_Opponent))
			{
				EntityID enemyID = m_Warminds[unit.m_Owner].m_Opponent;
				UnitComponent& enemyUnit = m_UnitComponents[m_Characters[enemyID].m_UnitEntity];

				if (enemyUnit.m_Raised)
				{
					startCombatTimer(unit.m_EntityID, enemyUnit.m_EntityID);
				}
			}

			if (unit.m_CurrentPath.size() > 0)
			{
				Vector2D nextPosition = Map::convertToScreen(unit.m_CurrentPath.front());
				unit.m_Target = nextPosition;
				unit.m_Direction = (nextPosition - transform.m_Position).normalized();

				Vector2DInt mapPos = Map::convertToMap(unit.m_LastPosition);

				for (auto& squareData : Map::get().m_MapSquareData)
				{
					if (squareData.m_Position == mapPos)
					{
						squareData.addUnique(unit.m_EntityID);
						break;
					}
				}

				unit.m_CurrentPath.erase(unit.m_CurrentPath.begin());
				if (unit.m_TargetPath.size() > 0)
				{
					unit.m_TargetPath.pop_front();
				}
			}
			else
			{
				startConquerRegion(unit, transform);
				unit.m_Moving = false;
			}
		}
	}

	void seizeRegion(UnitComponent& unit)
	{
		if (unit.m_SeizingRegionID <= 0)
		{
			return;
		}

		if (unit.m_LastSeizeDate < Time::m_GameDate.m_Date)
		{
			if (!unit.m_InCombat)
			{
				unit.m_DaysSeizing++;
				unit.m_LastSeizeDate = Time::m_GameDate.m_Date;

				MapRegion region = Map::get().getRegionById(unit.m_SeizingRegionID);

				if ((unsigned int)unit.m_DaysSeizing >= region.m_DaysToSeize)
				{
					EntityID loosingEntity = region.m_OwnerID;
					conquerRegion(unit.m_Owner, loosingEntity, unit.m_SeizingRegionID);
					unit.m_DaysSeizing = 0;
					unit.m_SeizingRegionID = -1;
				}
			}
		}
	}

	void startConquerRegion(UnitComponent& unit, Transform& transform)
	{
		std::vector<int> regionIDs = Map::get().getRegionIDs();
		Vector2DInt currentMapPosition = Map::convertToMap(transform.m_Position);

		for each (int regionID in regionIDs)
		{
			Vector2DInt capitalPosition = Map::get().getRegionCapitalLocation(regionID);
			if (currentMapPosition == capitalPosition)
			{
				unsigned int ownerID = Map::get().getRegionById(regionID).m_OwnerID;
				if (ownerID == unit.m_Owner)
				{
					continue;
				}

				unit.m_SeizingRegionID = regionID;
				unit.m_LastSeizeDate = Time::m_GameDate.m_Date;
			}
		}
	}

	void conquerRegion(EntityID conqueringCharacterID, EntityID loosingCharacterEntity, size_t regionID)
	{
#pragma warning(push)
#pragma warning(disable: 26815)
		CharacterSystem* characterSystem = (CharacterSystem*)m_EntityManager->getSystem<CharacterSystem>().get();
#pragma warning(pop)
		characterSystem->conquerRegion(regionID, conqueringCharacterID);
		characterSystem->loseRegion(regionID, loosingCharacterEntity);

		War& currentWar = *m_WarManager->getWarAgainst(m_Characters[conqueringCharacterID], m_Characters[loosingCharacterEntity]);
		currentWar.addWarscore(conqueringCharacterID, 50);
	}

	bool enemyAtSquare(Vector2DInt square, EntityID opponent)
	{
		for (auto& squareData : Map::get().m_MapSquareData)
		{
			if (squareData.m_Position == square)
			{
				for (auto& ID : squareData.m_EntitiesInSquare)
				{
					if (m_UnitComponents[ID].m_Owner == opponent)
					{
						return true;
					}
				}

				break;
			}
		}

		return false;
	}

	void startCombatTimer(EntityID unit, EntityID enemyUnit)
	{
		LOG_INFO("Battle began between {0}'s army and {1}'s army", m_Characters[m_UnitComponents[unit].m_Owner].m_Name, m_Characters[m_UnitComponents[enemyUnit].m_Owner].m_Name);

		if (!m_UnitComponents[unit].m_InCombat && !m_UnitComponents[enemyUnit].m_InCombat)
		{
			m_UnitComponents[unit].m_Moving = false;
			m_UnitComponents[unit].EnemyArmy = &m_UnitComponents[enemyUnit];
			m_UnitComponents[enemyUnit].EnemyArmy = &m_UnitComponents[unit];

			m_UnitComponents[enemyUnit].m_Moving = false;
			m_UnitComponents[unit].m_InCombat = true;
			m_UnitComponents[enemyUnit].m_InCombat = true;
		}
	}

	void determineCombat(EntityID unit, EntityID enemyUnit)
	{
		WarManager* warManager = &WarManager::get();

		if (m_UnitComponents[unit].m_CombatTimerAccu > m_UnitComponents[unit].m_CombatTimer)
		{
			War* currentWar = warManager->getWarAgainst(m_Characters[m_UnitComponents[unit].m_Owner], m_Characters[m_UnitComponents[enemyUnit].m_Owner]);

			if (currentWar == nullptr)
			{
				return;
			}

#pragma warning(push)
#pragma warning(disable: 26815)
			CharacterSystem* characterSystem = (CharacterSystem*)m_EntityManager->getSystem<CharacterSystem>().get();
#pragma warning(pop)

			if (currentWar->getAttacker().m_RaisedArmySize > currentWar->getDefender().m_RaisedArmySize)
			{
				currentWar->addWarscore(m_Characters[m_UnitComponents[unit].m_Owner].getID(), 50);
				characterSystem->dismissUnit(currentWar->getDefender().m_EntityID);
				OnCombatEnded(unit, enemyUnit);
			}

			else
			{
				currentWar->addWarscore(m_Characters[m_UnitComponents[enemyUnit].m_Owner].getID(), 100);
				characterSystem->dismissUnit(currentWar->getAttacker().m_EntityID);
				OnCombatEnded(unit, enemyUnit);
			}
		}
	}

	void OnCombatEnded(EntityID unit, EntityID enemyUnit)
	{
		m_UnitComponents[unit].m_Moving = true;
		m_UnitComponents[unit].EnemyArmy = nullptr;
		m_UnitComponents[enemyUnit].EnemyArmy = nullptr;
		m_UnitComponents[unit].m_InCombat = false;
		m_UnitComponents[enemyUnit].m_InCombat = false;
	}

	void showPath(Transform& transform, UnitComponent& unit)
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
				oldPosition = Map::convertToMap(transform.m_Position);
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
};
