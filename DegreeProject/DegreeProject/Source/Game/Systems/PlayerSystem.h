#pragma once

#include "ECS/System.h"
#include "Game/Components/Player.h"
#include "Game/Components/Unit.h"

struct PlayerSystem : System
{
	EntityManager* m_EntityManager;

	bool m_Draging = false;
	Vector2D m_MousePosition;
	sf::RectangleShape m_DragWindow;
	float m_ClickTolerance = 16.0f;

	//Debug
	std::vector<sf::RectangleShape> m_DragPositions;
	bool m_Debug = false;

	PlayerSystem()
	{
		addComponentSignature<Player>();
		m_EntityManager = &EntityManager::get();
	}

	virtual void start() override
	{
		Player* playerComponents = m_EntityManager->getComponentArray<Player>();

		for (auto& entity : m_Entities)
		{
			CharacterComponent& character = m_EntityManager->getComponent<CharacterComponent>(playerComponents[entity].m_OwnedCharacter);
			character.initUnit();
		}
	}

	virtual void update() override
	{
		Player* playerComponents = m_EntityManager->getComponentArray<Player>();

		clickDrag();

		for (auto& entity : m_Entities)
		{
			selectUnit(entity, playerComponents[entity].m_OwnedCharacter);
			moveUnit(playerComponents[entity].m_SelectedUnitID);
			hoverOverRegion(playerComponents[entity]);
		}
	}

	virtual void render() override
	{
		if (m_Draging)
		{
			Window::getWindow()->draw(m_DragWindow);

			//Debug
			if (m_Debug)
			{
				for each (sf::RectangleShape rectangle in m_DragPositions)
				{
					Window::getWindow()->draw(rectangle);
				}
			}
		}
	}

	void moveUnit(EntityID unitID)
	{
		if (InputHandler::getRightMouseReleased() == true && InputHandler::getPlayerUnitSelected() == true)
		{
			Vector2DInt mousePosition = InputHandler::getMouseMapPosition();
			UnitComponent& unit = m_EntityManager->getComponent<UnitComponent>(unitID);
			Transform& transform = m_EntityManager->getComponent<Transform>(unitID);

			unit.m_Moving = false;
			transform.m_Position = unit.m_LastPosition;
			
			Vector2D unitPosition = transform.m_Position;
			Vector2DInt startingPosition = Map::convertToMap(unitPosition);
			std::list<Vector2DInt> path = Pathfinding::findPath(startingPosition, mousePosition);

			if (path.size() > 0)
			{
				if (path.back() == unit.m_CurrentPath.back())
				{
					unit.m_Moving = true;
					return;
				}

				unit.setPath(path, Map::convertToScreen(startingPosition));
#pragma warning(push)
#pragma warning(disable: 26815)
				UnitSystem* unitSystem = (UnitSystem*)m_EntityManager->getSystem<UnitSystem>().get();
#pragma warning(pop)
				unitSystem->showPath(transform, unit);
			}
			else
			{
				unit.m_CurrentPath.clear();
			}
		}
	}

	void clickDrag()
	{
		if (InputHandler::getLeftMouseClicked() == true && m_Draging == false)
		{
			m_MousePosition = InputHandler::getMousePosition();
			if (InputHandler::getMouseMoved() == true)
			{
				m_Draging = true;
				m_DragWindow.setSize(sf::Vector2f(10.0f, 10.0f));
				m_DragWindow.setPosition(m_MousePosition.x, m_MousePosition.y);
				m_DragWindow.setFillColor(sf::Color::Transparent);
				m_DragWindow.setOutlineThickness(1.0f);
			}
		}
		if (m_Draging == true)
		{
			Vector2D mousePosition = InputHandler::getMousePosition();
			Vector2D distance = mousePosition - m_MousePosition;
			m_DragWindow.setSize(sf::Vector2f(distance.x, distance.y));
			if (InputHandler::getLeftMouseReleased() == true)
			{
				m_Draging = false;
			}
		}
	}

	void selectUnit(EntityID playerID, EntityID ownerID)
	{
		if (InputHandler::getLeftMouseClicked() == true && m_Draging == false)
		{
			if (Map::get().mapSquareDataContainsKey(InputHandler::getMouseMapPosition()))
			{
				std::list<EntityID> unitIDList = {};
				
				for (auto& squareData : Map::get().m_MapSquareData)
				{
					if (squareData.m_Position == InputHandler::getMouseMapPosition())
					{
						unitIDList = squareData.m_EntitiesInSquare;
					}
				}

				if (unitIDList.size() > 0)
				{
					for (EntityID unitID : unitIDList)
					{
						UnitComponent& unit = m_EntityManager->getComponent<UnitComponent>(unitID);
						Player& player = m_EntityManager->getComponent<Player>(playerID);
						if (unit.m_Owner == ownerID)
						{
							unit.m_PlayerSelected = true;
							unit.m_OutlineThickness = 1.0f;
							player.m_SelectedUnitID = unitID;
							InputHandler::setPlayerUnitSelected(true);
							break;
						}
					}
				}
				else
				{
					deselectUnit(playerID);
				}
			}
			else
			{
				deselectUnit(playerID);
			}
		}
		if (m_Draging == true)
		{
			Vector2D firstPosition = m_DragWindow.getPosition();
			float width = m_DragWindow.getGlobalBounds().width;
			float height = m_DragWindow.getGlobalBounds().height;
			unsigned int xPositions = (unsigned int)width / 32;
			unsigned int yPositions = (unsigned int)height / 32;
			for (unsigned int y = 0; y < yPositions; y++)
			{
				for (unsigned int x = 0; x < xPositions; x++)
				{
					Vector2D position = Vector2D(firstPosition.x + x * 32, firstPosition.y + y * 32);
					Vector2DInt mapPosition = Map::convertToMap(position);
					if (Map::get().mapSquareDataContainsKey(mapPosition))
					{
						std::list<EntityID> unitIDList = {};

						for (auto& squareData : Map::get().m_MapSquareData)
						{
							if (squareData.m_Position == mapPosition)
							{
								unitIDList = squareData.m_EntitiesInSquare;
							}
						}

						for (EntityID unitID : unitIDList)
						{
							UnitComponent& unit = m_EntityManager->getComponent<UnitComponent>(unitID);
							if (unit.m_Owner == ownerID)
							{
								unit.m_PlayerSelected = true;
								unit.m_OutlineThickness = 1.0f;
								break;
							}
						}
					}
				}
			}
		}
	}

	void deselectUnit(EntityID playerID)
	{
		Player& player = m_EntityManager->getComponent<Player>(playerID);
		UnitComponent& unit = m_EntityManager->getComponent<UnitComponent>(player.m_SelectedUnitID);
		unit.m_PlayerSelected = false;
		unit.m_OutlineThickness = 0.0f;
		player.m_SelectedUnitID = 0;
		InputHandler::setPlayerUnitSelected(false);
	}

	void setDebugPositions(std::vector<Vector2DInt> dragPositions)
	{
		m_DragPositions.clear();
		for (Vector2DInt mapPosition : dragPositions)
		{
			sf::RectangleShape rectangle(sf::Vector2f(30.0f, 30.0f));
			Vector2D screenPosition = Map::convertToScreen(mapPosition);
			rectangle.setPosition(screenPosition.x + 2, screenPosition.y + 2);
			rectangle.setFillColor(sf::Color::Magenta);
			m_DragPositions.push_back(rectangle);
		}
	}

	void hoverOverRegion(Player& playerComp)
	{
		Vector2DInt currentMousePosition = InputHandler::getMouseMapPosition();

		if (Map::get().mapSquareDataContainsKey(currentMousePosition))
		{
			EntityID regionID = 0; 
			
			for (auto& squareData : Map::get().m_MapSquareData)
			{
				if (squareData.m_Position == currentMousePosition)
				{
					regionID = squareData.m_RegionID;
				}
			}

			if ((unsigned int)playerComp.m_HighlightedRegion != regionID)
			{
				Map::get().getRegionById(playerComp.m_HighlightedRegion).m_Highlighted = false;

				Map::get().getRegionById(regionID).m_Highlighted = true;
				playerComp.m_HighlightedRegion = regionID;
			}
		}
		else
		{
			if (playerComp.m_HighlightedRegion >= 0)
			{
				Map::get().getRegionById(playerComp.m_HighlightedRegion).m_Highlighted = false;
				playerComp.m_HighlightedRegion = -1;
			}
		}
	}
};