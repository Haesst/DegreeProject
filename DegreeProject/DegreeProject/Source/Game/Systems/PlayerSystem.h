#pragma once

#include "ECS/System.h"
//#include "Game/Components/Player.h"
#include "Game/Components/Unit.h"
#include "Game/Pathfinding.h"
#include "Game/Systems/UnitSystem.h"

//struct PlayerSystem
//{
//	EntityManager* m_EntityManager;
//
//	bool m_MouseDownLastFrame = false;
//	bool m_Draging = false;
//	Vector2D m_MousePosition;
//	sf::RectangleShape m_DragWindow;
//	float m_ClickTolerance = 16.0f;
//
//	Vector2D m_DragDirection = { 0.0f, 0.0f };
//
//	PlayerSystem()
//	{
//		addComponentSignature<Player>();
//		m_EntityManager = &EntityManager::get();
//	}
//
//	virtual void start() override
//	{
//		Player* playerComponents = m_EntityManager->getComponentArray<Player>();
//
//		for (auto& entity : m_Entities)
//		{
//			CharacterComponent& character = m_EntityManager->getComponent<CharacterComponent>(playerComponents[entity].m_OwnedCharacter);
//			character.initUnit();
//		}
//	}
//
//	virtual void update() override
//	{
//		Player* playerComponents = m_EntityManager->getComponentArray<Player>();
//
//		clickDrag();
//
//		for (auto& entity : m_Entities)
//		{
//			selectUnit(entity);
//			moveUnit(playerComponents[entity].m_SelectedUnitID);
//			hoverOverRegion(playerComponents[entity]);
//		}
//	}
//
//	virtual void render() override
//	{
//		if (m_Draging)
//		{
//			Window::getWindow()->draw(m_DragWindow);
//		}
//	}
//
//	void moveUnit(EntityID unitID)
//	{
//		if (InputHandler::getRightMouseReleased() == true && InputHandler::getPlayerUnitSelected() == true)
//		{
//			Vector2DInt mousePosition = InputHandler::getMouseMapPosition();
//			UnitComponent& unit = m_EntityManager->getComponent<UnitComponent>(unitID);
//			Transform& transform = m_EntityManager->getComponent<Transform>(unitID);
//
//			unit.m_Moving = false;
//			transform.m_Position = unit.m_LastPosition;
//			
//			Vector2D unitPosition = transform.m_Position;
//			Vector2DInt startingPosition = Map::convertToMap(unitPosition);
//			std::vector<Vector2DInt> path = Pathfinding::get().findPath(startingPosition, mousePosition);
//
//			if (path.size() > 0)
//			{
//				if (unit.m_CurrentPath.size() > 0)
//				{
//					if (path.back() == unit.m_CurrentPath.back())
//					{
//						unit.m_Moving = true;
//						return;
//					}
//				}
//				
//				unit.setPath(path, Map::convertToScreen(startingPosition));
//#pragma warning(push)
//#pragma warning(disable: 26815)
//				UnitSystem* unitSystem = (UnitSystem*)m_EntityManager->getSystem<UnitSystem>().get();
//#pragma warning(pop)
//				unitSystem->showPath(transform, unit);
//			}
//			else
//			{
//				unit.m_CurrentPath.clear();
//			}
//		}
//	}
//
//	void clickDrag()
//	{
//		if (m_MouseDownLastFrame && !m_Draging)
//		{
//			m_Draging = true;
//			m_DragWindow.setSize(sf::Vector2f(10.0f, 10.0f));
//			m_DragWindow.setPosition(m_MousePosition.x, m_MousePosition.y);
//			m_DragWindow.setFillColor(sf::Color::Transparent);
//			m_DragWindow.setOutlineThickness(1.0f);
//		}
//
//		if (m_Draging)
//		{
//			Vector2D mousePosition = InputHandler::getMousePosition();
//			Vector2D distance = mousePosition - m_MousePosition;
//			m_DragWindow.setSize(sf::Vector2f(distance.x, distance.y));
//
//			Vector2D normDistance = distance.normalized();
//			m_DragDirection.x = normDistance.x;
//			m_DragDirection.y = normDistance.y;
//		}
//
//		if (InputHandler::getLeftMouseClicked())
//		{
//			m_MouseDownLastFrame = true;
//			m_MousePosition = InputHandler::getMousePosition();
//		}
//		else if (InputHandler::getLeftMouseReleased())
//		{
//			m_MouseDownLastFrame = false;
//			m_Draging = false;
//		}
//	}
//
//	void selectUnit(EntityID playerID)
//	{
//		if (InputHandler::getLeftMouseClicked() && !m_Draging)
//		{
//			Player& player = m_EntityManager->getComponent<Player>(playerID);
//			CharacterComponent& character = m_EntityManager->getComponent<CharacterComponent>(player.m_OwnedCharacter);
//			UnitComponent& unit = m_EntityManager->getComponent<UnitComponent>(character.m_UnitEntity);
//			Transform& unitTransform = m_EntityManager->getComponent<Transform>(character.m_UnitEntity);
//			if (Map::convertToMap(unitTransform.m_Position) == InputHandler::getMouseMapPosition())
//			{
//				unit.m_PlayerSelected = true;
//				unit.m_OutlineThickness = 1.0f;
//				player.m_SelectedUnitID = character.m_UnitEntity;
//				InputHandler::setPlayerUnitSelected(true);
//			}
//			else
//			{
//				deselectUnit(playerID);
//			}
//		}
//
//		if (m_Draging && InputHandler::getMouseMoved())
//		{
//			Vector2D startPosition = m_DragWindow.getPosition();
//			float width = m_DragWindow.getGlobalBounds().width;
//			float height = m_DragWindow.getGlobalBounds().height;
//
//			float firstX = startPosition.x;
//			float firstY = startPosition.y;
//
//			if (m_DragDirection.x < 0.0f)
//			{
//				firstX -= width;
//			}
//
//			if (m_DragDirection.y < 0.0f)
//			{
//				firstY -= height;
//			}
//
//			sf::Vector2f topLeftPosition = { firstX, firstY };
//
//			bool foundUnit = false;
//			unsigned int xPositions = (unsigned int)width;
//			unsigned int yPositions = (unsigned int)height;
//			for (unsigned int y = 0; y < yPositions; y += (int)Map::m_TileSize)
//			{
//				for (unsigned int x = 0; x < xPositions; x += (int)Map::m_TileSize)
//				{
//					Vector2D position = Vector2D(topLeftPosition.x + x, topLeftPosition.y + y);
//					Vector2DInt mapPosition = Map::convertToMap(position);
//					Player& player = m_EntityManager->getComponent<Player>(playerID);
//					CharacterComponent& character = m_EntityManager->getComponent<CharacterComponent>(player.m_OwnedCharacter);
//					UnitComponent& unit = m_EntityManager->getComponent<UnitComponent>(character.m_UnitEntity);
//					Transform& unitTransform = m_EntityManager->getComponent<Transform>(character.m_UnitEntity);
//					if (Map::convertToMap(unitTransform.m_Position) == mapPosition)
//					{
//						unit.m_PlayerSelected = true;
//						unit.m_OutlineThickness = 1.0f;
//						player.m_SelectedUnitID = character.m_UnitEntity;
//						InputHandler::setPlayerUnitSelected(true);
//						foundUnit = true;
//						break;
//					}
//					else
//					{
//						deselectUnit(playerID);
//					}
//				}
//				if (foundUnit)
//				{
//					break;
//				}
//				else
//				{
//					deselectUnit(playerID);
//				}
//			}
//		}
//	}
//
//	void deselectUnit(EntityID playerID)
//	{
//		Player& player = m_EntityManager->getComponent<Player>(playerID);
//		UnitComponent& unit = m_EntityManager->getComponent<UnitComponent>(player.m_SelectedUnitID);
//		unit.m_PlayerSelected = false;
//		unit.m_OutlineThickness = 0.0f;
//		player.m_SelectedUnitID = 0;
//		InputHandler::setPlayerUnitSelected(false);
//	}
//
//	void hoverOverRegion(Player& playerComp)
//	{
//		Vector2DInt currentMousePosition = InputHandler::getMouseMapPosition();
//
//		if (Map::get().mapSquareDataContainsKey(currentMousePosition))
//		{
//			EntityID regionID = 0; 
//			
//			for (auto& squareData : Map::get().m_MapSquareData)
//			{
//				if (squareData.m_Position == currentMousePosition)
//				{
//					regionID = squareData.m_RegionID;
//					break;
//				}
//			}
//
//			if ((unsigned int)playerComp.m_HighlightedRegion != regionID)
//			{
//				Map::get().getRegionById(playerComp.m_HighlightedRegion).m_Highlighted = false;
//
//				Map::get().getRegionById(regionID).m_Highlighted = true;
//				playerComp.m_HighlightedRegion = regionID;
//			}
//		}
//		else
//		{
//			if (playerComp.m_HighlightedRegion >= 0)
//			{
//				Map::get().getRegionById(playerComp.m_HighlightedRegion).m_Highlighted = false;
//				playerComp.m_HighlightedRegion = -1;
//			}
//		}
//	}
//};