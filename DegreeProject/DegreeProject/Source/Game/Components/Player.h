#pragma once

#include <SFML/Graphics.hpp>

#include "Game/Map/Map.h"
#include "Game/Data/Types.h"
#include "Game/Data/Unit.h"
#include "Game/Pathfinding.h"

#include "Engine/Vector2D.h"
#include "Engine/InputHandler.h"
#include "Engine/Window.h"
#include "Engine/UnitManager.h"

class Player
{
public:
	Player(CharacterID ownedCharacter)
	{
		m_OwnedCharacter = ownedCharacter;
	}

	void update()
	{
		clickDrag();

		tryToSelectUnit();
		moveUnit();
		hoverOverRegion();
	}

	void render()
	{
		if (m_Draging)
		{
			Window::getWindow()->draw(m_DragWindow);
		}
	}

private:
	void hoverOverRegion()
	{
		Vector2DInt currentMousePosition = InputHandler::getMouseMapPosition();
		
		if (Map::get().mapSquareDataContainsKey(currentMousePosition))
		{
			size_t regionID = 0;

			for (auto& squareData : Map::get().m_MapSquareData)
			{
				if (squareData.m_Position == currentMousePosition)
				{
					regionID = squareData.m_RegionID;
					break;
				}
			}

			if ((unsigned int)m_HighlightedRegion != regionID)
			{
				Map::get().getRegionById(m_HighlightedRegion).m_Highlighted = false;

				Map::get().getRegionById(regionID).m_Highlighted = true;
				m_HighlightedRegion = regionID;
			}
		}
		else
		{
			if (m_HighlightedRegion >= 0)
			{
				Map::get().getRegionById(m_HighlightedRegion).m_Highlighted = false;
				m_HighlightedRegion = -1;
			}
		}
	}
	
	void clickDrag()
	{
		if (m_MouseDownLastFrame && !m_Draging)
		{
			m_Draging = true;
			m_DragWindow.setSize(sf::Vector2f(10.0f, 10.0f));
			m_DragWindow.setPosition(m_MousePosition.x, m_MousePosition.y);
			m_DragWindow.setFillColor(sf::Color::Transparent);
			m_DragWindow.setOutlineThickness(1.0f);
		}

		if (m_Draging)
		{
			Vector2D mousePosition = InputHandler::getMousePosition();
			Vector2D distance = mousePosition - m_MousePosition;
			m_DragWindow.setSize(sf::Vector2f(distance.x, distance.y));

			Vector2D normDistance = distance.normalized();
			m_DragDirection.x = normDistance.x;
			m_DragDirection.y = normDistance.y;
		}

		if (InputHandler::getLeftMouseClicked())
		{
			m_MouseDownLastFrame = true;
			m_MousePosition = InputHandler::getMousePosition();
		}
		else if (InputHandler::getLeftMouseReleased())
		{
			m_MouseDownLastFrame = false;
			m_Draging = false;
		}
	}

	void selectUnit(UnitID id)
	{
		Unit& unit = UnitManager::get().getUnitWithId(id);

		unit.m_Selected = true;
		InputHandler::setPlayerUnitSelected(true);
		m_SelectedUnitID = id;
	}

	void tryToSelectUnit()
	{
		if (InputHandler::getLeftMouseClicked() && !m_Draging)
		{
			Vector2DInt mousePosition = InputHandler::getMouseMapPosition();

			bool foundUnit = false;

			if (Map::get().mapSquareDataContainsKey(mousePosition))
			{
				for (auto& squareData : Map::get().m_MapSquareData)
				{
					if (squareData.m_Position != mousePosition)
					{
						continue;
					}

					CharacterID playerCharacter = CharacterManager::get()->getPlayerCharacterID();

					for (auto& unitID : squareData.m_EntitiesInSquare)
					{
						if (UnitManager::get().getUnitWithId(unitID).m_Owner == playerCharacter)
						{
							selectUnit(unitID);
							foundUnit = true;
						}
					}
				}
			}

			if (!foundUnit)
			{
				deselectUnits();
			}

			//CharacterComponent& character = m_EntityManager->getComponent<CharacterComponent>(player.m_OwnedCharacter);
			//UnitComponent& unit = m_EntityManager->getComponent<UnitComponent>(character.m_UnitEntity);
			//Transform& unitTransform = m_EntityManager->getComponent<Transform>(character.m_UnitEntity);
			//if (Map::convertToMap(unitTransform.m_Position) == InputHandler::getMouseMapPosition())
			//{
			//	unit.m_PlayerSelected = true;
			//	unit.m_OutlineThickness = 1.0f;
			//	player.m_SelectedUnitID = character.m_UnitEntity;
			//	InputHandler::setPlayerUnitSelected(true);
			//}
			//else
			//{
			//	//deselectUnit(playerID);
			//}
		}
		
		if (m_Draging && InputHandler::getMouseMoved())
		{
			Vector2D startPosition = m_DragWindow.getPosition();
			float width = m_DragWindow.getGlobalBounds().width;
			float height = m_DragWindow.getGlobalBounds().height;
		
			float firstX = startPosition.x;
			float firstY = startPosition.y;
		
			if (m_DragDirection.x < 0.0f)
			{
				firstX -= width;
			}
		
			if (m_DragDirection.y < 0.0f)
			{
				firstY -= height;
			}
		
			sf::Vector2f topLeftPosition = { firstX, firstY };
		
			bool foundUnit = false;

			unsigned int xPositions = (unsigned int)width;
			unsigned int yPositions = (unsigned int)height;
			for (unsigned int y = 0; y < yPositions; y += (int)Map::m_TileSize)
			{
				for (unsigned int x = 0; x < xPositions; x += (int)Map::m_TileSize)
				{
					Vector2D position = Vector2D(topLeftPosition.x + x, topLeftPosition.y + y);
					Vector2DInt mapPosition = Map::convertToMap(position);
					//Player& player = m_EntityManager->getComponent<Player>(playerID);
					//CharacterComponent& character = m_EntityManager->getComponent<CharacterComponent>(player.m_OwnedCharacter);
					//UnitComponent& unit = m_EntityManager->getComponent<UnitComponent>(character.m_UnitEntity);
					//Transform& unitTransform = m_EntityManager->getComponent<Transform>(character.m_UnitEntity);

					// check if it contains key

					if (Map::get().mapSquareDataContainsKey(mapPosition))
					{
						for (auto& squareData : Map::get().m_MapSquareData)
						{
							if (squareData.m_Position != mapPosition)
							{
								continue;
							}

							CharacterID playerCharacter = CharacterManager::get()->getPlayerCharacterID();

							for (auto& unitID : squareData.m_EntitiesInSquare)
							{
								if (UnitManager::get().getUnitWithId(unitID).m_Owner == playerCharacter)
								{
									selectUnit(unitID);
									foundUnit = true;
									break;
								}
							}
						}
					}
				}
			}

			if (!foundUnit)
			{
				deselectUnits();
			}
		}
	}

	void moveUnit()
	{
		if (InputHandler::getRightMouseReleased() == true && m_SelectedUnitID != INVALID_UNIT_ID)
		{
			Vector2DInt mousePosition = InputHandler::getMouseMapPosition();
			Vector2DInt unitPosition = Map::get().convertToMap(UnitManager::get().getUnitWithId(m_SelectedUnitID).m_Position);

			UnitManager::get().giveUnitPath(m_SelectedUnitID, Pathfinding::get().findPath(unitPosition, mousePosition));

			/*Vector2DInt mousePosition = InputHandler::getMouseMapPosition();
			UnitComponent& unit = m_EntityManager->getComponent<UnitComponent>(unitID);
			Transform& transform = m_EntityManager->getComponent<Transform>(unitID);

			unit.m_Moving = false;
			transform.m_Position = unit.m_LastPosition;

			Vector2D unitPosition = transform.m_Position;
			Vector2DInt startingPosition = Map::convertToMap(unitPosition);
			std::vector<Vector2DInt> path = Pathfinding::get().findPath(startingPosition, mousePosition);

			if (path.size() > 0)
			{
				if (unit.m_CurrentPath.size() > 0)
				{
					if (path.back() == unit.m_CurrentPath.back())
					{
						unit.m_Moving = true;
						return;
					}
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
			}*/
		}
	}

	void deselectUnits()
	{
		if (m_SelectedUnitID == INVALID_UNIT_ID)
		{
			return;
		}

		Unit& unit = UnitManager::get().getUnitWithId(m_SelectedUnitID);
		unit.m_Selected = false;
		InputHandler::setPlayerUnitSelected(false);
		m_SelectedUnitID = INVALID_UNIT_ID;
		/*Player& player = m_EntityManager->getComponent<Player>(playerID);
		UnitComponent& unit = m_EntityManager->getComponent<UnitComponent>(player.m_SelectedUnitID);
		unit.m_PlayerSelected = false;
		unit.m_OutlineThickness = 0.0f;
		player.m_SelectedUnitID = 0;
		InputHandler::setPlayerUnitSelected(false);*/
	}

private:
	bool m_Draging = false;
	sf::RectangleShape m_DragWindow;

	bool m_MouseDownLastFrame = false;
	Vector2D m_MousePosition;
	Vector2D m_DragDirection = { 0.0f, 0.0f };

	int m_HighlightedRegion = -1;
	unsigned int m_SelectedUnitID = INVALID_UNIT_ID;
	unsigned int m_OwnedCharacter = INVALID_CHARACTER_ID;
};