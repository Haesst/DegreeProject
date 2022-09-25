#include "Player.h"
#include "Game/UI/UIManager.h"
#include "Game/UI/EndWindow.h"
#include "UI/ArmyWindow.h"

Player* Player::m_Instance = nullptr;

Player::Player(CharacterID ownedCharacter)
{
	m_OwnedCharacter = ownedCharacter;
}

Player::Player()
{
	m_DragWindow.setFillColor(sf::Color::Transparent);
}

Player& Player::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new Player();
	}

	return *m_Instance;
}

void Player::update()
{
	clickDrag();
	tryToSelectUnit();
	moveUnit();
	hoverOverRegion();
}

void Player::render()
{
	if (m_Draging)
	{
		Window::getWindow()->draw(m_DragWindow);
	}
}

void Player::hoverOverRegion()
{
	if (UIManager::get().IsDraggingWindow())
	{
		if (m_HighlightedRegion >= 0)
		{
			Map::get().getRegionById(m_HighlightedRegion).m_Highlighted = false;
			m_HighlightedRegion = -1;
		}
		return;
	}

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

void Player::clickDrag()
{
	if (UIManager::get().IsDraggingWindow())
	{
		m_MouseDownLastFrame = false;
		m_Draging = false;
		return;
	}

	if (m_MouseDownLastFrame && !m_Draging)
	{
		m_Draging = true;
		m_DragWindow.setPosition(m_MousePosition.x, m_MousePosition.y);
	}

	if (m_Draging)
	{
		Vector2D mousePosition = InputHandler::getMousePosition();
		Vector2D distance = mousePosition - m_MousePosition;
		m_DragWindow.setSize(sf::Vector2f(distance.x, distance.y));
		m_DragWindow.setOutlineThickness(m_DragThickness * InputHandler::m_TotalZoom);

		Vector2D direction = distance.normalized();
		m_DragDirection.x = direction.x;
		m_DragDirection.y = direction.y;
	}

	if (InputHandler::getLeftMouseClicked())
	{
		m_MouseDownLastFrame = true;
		m_MousePosition = InputHandler::getMousePosition();
	}
	else if (InputHandler::getLeftMouseReleased() && m_Draging)
	{
		m_MouseDownLastFrame = false;
		m_Draging = false;
	}
}

void Player::tryToSelectUnit()
{
	if (InputHandler::getLeftMouseClicked() && !m_Draging)
	{
		ArmyWindow& armyWindow = *UIManager::get().m_ArmyWindow;
		Vector2D mousePositionUI = InputHandler::getUIMousePosition();
		Vector2D mousePosition = InputHandler::getMousePosition();
		CharacterID playerCharacterID = CharacterManager::get().getPlayerCharacterID();
		Unit& unit = UnitManager::get().getUnitOfCharacter(playerCharacterID);
		if (unit.m_Sprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			selectUnit(unit.m_UnitID);
		}
		else if (m_SelectedUnitID != INVALID_UNIT_ID && !armyWindow.m_WindowShape.getGlobalBounds().contains(mousePositionUI.x, mousePositionUI.y))
		{
			deselectUnit();
		}
	}

	if (m_Draging && InputHandler::getMouseMoved())
	{
		ArmyWindow& armyWindow = *UIManager::get().m_ArmyWindow;
		Vector2D mousePositionUI = InputHandler::getUIMousePosition();
		CharacterID playerCharacterID = CharacterManager::get().getPlayerCharacterID();
		Unit& unit = UnitManager::get().getUnitOfCharacter(playerCharacterID);
		Vector2D unitPosition = { unit.m_Position.x + unit.m_HighlightShapeSize.x * 0.5f, unit.m_Position.y + unit.m_HighlightShapeSize.y * 0.5f };
		if (m_DragWindow.getGlobalBounds().contains(unitPosition.x, unitPosition.y))
		{
			selectUnit(unit.m_UnitID);
		}
		else if (m_SelectedUnitID != INVALID_UNIT_ID && !armyWindow.m_WindowShape.getGlobalBounds().contains(mousePositionUI.x, mousePositionUI.y))
		{
			deselectUnit();
		}
	}
}

void Player::selectUnit(UnitID id)
{
	Unit& unit = UnitManager::get().getUnitWithId(id);
	unit.m_Selected = true;
	InputHandler::setPlayerUnitSelected(true);
	ArmyWindow& armyWindow = *UIManager::get().m_ArmyWindow;
	armyWindow.openWindow();
	m_SelectedUnitID = id;
}

void Player::deselectUnit()
{
	Unit& unit = UnitManager::get().getUnitWithId(m_SelectedUnitID);
	unit.m_Selected = false;
	InputHandler::setPlayerUnitSelected(false);
	ArmyWindow& armyWindow = *UIManager::get().m_ArmyWindow;
	armyWindow.closeWindow();
	m_SelectedUnitID = INVALID_UNIT_ID;
}

void Player::moveUnit()
{
	if (InputHandler::getRightMouseReleased() && m_SelectedUnitID != INVALID_UNIT_ID)
	{
		InputHandler::setRightMouseReleased(false);
		Vector2DInt mousePosition = InputHandler::getMouseMapPosition();
		Vector2DInt unitPosition = Map::get().convertToMap(UnitManager::get().getUnitWithId(m_SelectedUnitID).m_Position);
		UnitManager::get().giveUnitPath(m_SelectedUnitID, Pathfinding::get().findPath(unitPosition, mousePosition));
	}
}

void Player::loseGame(LoseCause cause)
{
	UIManager::get().m_EndWindow->openWindow(unsigned int(cause));
}

void Player::winGame()
{
	UIManager::get().m_EndWindow->openWindow(unsigned int(LoseCause::Unlanded) + 1);
}
