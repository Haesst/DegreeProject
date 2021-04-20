#include "Game/UI/RegionWindow.h"
#include "Engine/Window.h"
#include "Engine/InputHandler.h"
#include "Game/Data/Unit.h"
#include "Game/Systems/UnitManager.h"
#include "Game/Map/Map.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/Data/Character.h"
#include "Game/UI/CharacterWindow.h"
#include "Game/UI/UIManager.h"

RegionWindow::RegionWindow(UIID id, sf::Font font, Vector2D, Vector2D size)
{
	m_OwnedUIWindow = id;
	m_Font = font;
	m_SizeX = size.x;
	m_SizeY = size.y;
	m_ProgressMeterDoubleBorder = m_ProgressMeterBorder * 2;
}

void RegionWindow::start()
{
	m_Window = Window::getWindow();

	m_BuildingSlotTextures[0] = AssetHandler::get().getTextureAtPath("Assets/Graphics/FortIcon.png");
	m_BuildingSlotTextures[1] = AssetHandler::get().getTextureAtPath("Assets/Graphics/BarackIcon.png");
	m_BuildingSlotTextures[2] = AssetHandler::get().getTextureAtPath("Assets/Graphics/MarketIcon.png");
	m_BuildingSlotTextures[3] = AssetHandler::get().getTextureAtPath("Assets/Graphics/WallIcon.png");

	m_RaiseArmyTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/soldier unit.png");

	m_MaleCharacterTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Harold.jpg");
	m_FemaleCharacterTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Harriet.jpg");

	m_IconSlotPositionX = m_SizeX - m_SpriteSize - m_OutlineThickness;
	m_IconSlotPositionOffset = m_SpriteSize + m_OutlineThickness * 2;
	m_IconSlotPositionY = m_Window->getSize().y - m_SpriteSize - m_OutlineThickness * 3;

	m_CharacterPosition = sf::Vector2f(m_OutlineThickness + m_SpriteSize, (float)(m_Window->getSize().y - m_SpriteSize * 8));

	m_PlayerCharacter = &CharacterManager::get()->getPlayerCharacter();

	float positionX = m_OutlineThickness;
	float positionY = m_Window->getSize().y - m_SizeY - m_OutlineThickness;

	m_WindowShape.setPosition(sf::Vector2f(positionX, positionY));
	m_WindowShape.setFillColor(m_FillColor);
	m_WindowShape.setOutlineThickness(m_OutlineThickness);

	for (unsigned int index = 0; index < NUMBER_OF_BUILDING_SLOTS; index++)
	{
		m_BuildingSlotShapes[index].setOutlineThickness(m_OutlineThickness * 0.5f);
		m_BuildingSlotShapes[index].setSize(sf::Vector2f((float)m_SpriteSize, (float)m_SpriteSize));
		m_BuildingSlotShapes[index].setPosition(sf::Vector2f(m_IconSlotPositionX - m_IconSlotPositionOffset * index, m_IconSlotPositionY));
		m_BuildingSlotSprites[index].setTexture(m_BuildingSlotTextures[index]);
		m_BuildingSlotSprites[index].setPosition(m_BuildingSlotShapes[index].getPosition());
		m_BuildingSlotSprites[index].setScale(m_SpriteSize / m_BuildingSlotSprites[index].getLocalBounds().width, m_SpriteSize / m_BuildingSlotSprites[index].getLocalBounds().height);
	}

	m_RaiseArmyShape.setPosition(sf::Vector2f(m_IconSlotPositionX, m_IconSlotPositionY - m_IconSlotPositionOffset));
	m_RaiseArmyShape.setOutlineThickness(m_OutlineThickness * 0.5f);
	m_RaiseArmyShape.setSize(sf::Vector2f((float)m_SpriteSize, (float)m_SpriteSize));
	m_RaiseArmySprite.setTexture(m_RaiseArmyTexture);
	m_RaiseArmySprite.setPosition(m_RaiseArmyShape.getPosition());
	m_RaiseArmySprite.setScale(m_SpriteSize / m_RaiseArmySprite.getLocalBounds().width, m_SpriteSize / m_RaiseArmySprite.getLocalBounds().height);

	m_RegionNameText.setPosition(sf::Vector2f(positionX + m_SizeX * 0.1f, positionY));
	m_RegionNameText.setFont(m_Font);
	m_RegionNameText.setCharacterSize(m_CharacterSize);

	m_RegionTaxText.setPosition(sf::Vector2f(positionX + m_SizeX * 0.1f, positionY + m_SizeY * 0.5f));
	m_RegionTaxText.setFont(m_Font);
	m_RegionTaxText.setCharacterSize((int)(m_CharacterSize * 0.5f));

	m_RegionManpowerText.setPosition(sf::Vector2f(positionX + m_SizeX * 0.1f, positionY + m_SizeY * 0.6f));
	m_RegionManpowerText.setFont(m_Font);
	m_RegionManpowerText.setCharacterSize((int)(m_CharacterSize * 0.5f));

	m_KingdomNameText.setFont(m_Font);
	m_KingdomNameText.setCharacterSize((int)(m_CharacterSize * 0.5f));
	m_KingdomNameText.setPosition(sf::Vector2f(positionX + m_SizeX * 0.1f, positionY + m_SizeY * 0.1f));
}

void RegionWindow::update()
{
	clickOnMap();
	handleWindow();

	if (m_Visible)
	{
		clickButton();
	}
}

void RegionWindow::render()
{
	if (m_Visible)
	{
		m_Window->draw(m_WindowShape);
		m_Window->draw(m_CharacterSprite);
		m_Window->draw(m_RegionNameText);
		m_Window->draw(m_RegionTaxText);
		m_Window->draw(m_RegionManpowerText);
		m_Window->draw(m_KingdomNameText);
		m_Window->draw(m_RaiseArmyShape);
		m_Window->draw(m_RaiseArmySprite);
		for (unsigned int index = 0; index < NUMBER_OF_BUILDING_SLOTS; index++)
		{
			m_Window->draw(m_BuildingSlotShapes[index]);
			displayProgressMeter(index);
			m_Window->draw(m_BuildingSlotSprites[index]);
		}
	}
}

void RegionWindow::clickOnMap()
{
	if (InputHandler::getLeftMouseClicked() && !InputHandler::getPlayerUnitSelected() && !InputHandler::getCharacterWindowOpen())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (!m_WindowShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			Vector2DInt mouseMapPosition = InputHandler::getMouseMapPosition();
			Map& map = Map::get();
			if (map.mapSquareDataContainsKey(mouseMapPosition))
			{
				size_t regionID = 0;

				for (auto& squareData : map.m_MapSquareData)
				{
					if (squareData.m_Position == mouseMapPosition)
					{
						regionID = squareData.m_RegionID;
					}
				}
				m_CurrentRegionID = regionID;
				m_CurrentMapRegion = &map.getRegionById(m_CurrentRegionID);
				checkIfPlayerRegion();
				updateInfo();
				m_Open = true;
			}
			else
			{
				closeWindow();
			}
		}
		else
		{
			m_Open = false;
		}
	}
	else if (InputHandler::getRightMouseClicked() && !InputHandler::getPlayerUnitSelected() && m_Visible)
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (!m_WindowShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			closeWindow();
		}
	}
}

void RegionWindow::onDayChange()
{
	if (m_Visible)
	{
		updateInfo();
	}
}

void RegionWindow::displayProgressMeter(unsigned int index)
{
	int buildingProgress = m_CurrentMapRegion->m_BuildingSlots[index].m_DaysBuilt;
	if (!m_CurrentMapRegion->m_BuildingSlots[index].m_Finished && buildingProgress != 0)
	{
		float progressWidth = m_ProgressMeterWidth - m_ProgressMeterDoubleBorder;
		progressWidth *= (float)buildingProgress / (float)GameData::m_Buildings[index + 1].m_DaysToConstruct;

		m_BuildingProgressShape[index].setPosition(m_BuildingSlotShapes[index].getPosition());
		m_BuildingProgressShape[index].setSize({ progressWidth, m_BuildingSlotShapes[index].getSize().y });
		m_BuildingProgressShape[index].setFillColor(m_OwnerColor);

		m_Window->draw(m_BuildingProgressShape[index]);
	}
	else if (m_CurrentMapRegion->m_BuildingSlots[index].m_Finished)
	{
		m_BuildingSlotShapes[index].setFillColor(m_OwnerColor);
	}
}

void RegionWindow::updateInfo()
{
	if (m_CurrentMapRegion != nullptr)
	{
		m_PlayerCharacter = &CharacterManager::get()->getPlayerCharacter();

		Character& character = CharacterManager::get()->getCharacter(m_CurrentMapRegion->m_OwnerID);

		m_OwnerColor = character.m_RegionColor;

		m_WindowShape.setOutlineColor(m_OwnerColor);

		if (character.m_Gender == Gender::Male)
		{
			m_CharacterSprite.setTexture(m_MaleCharacterTexture);
		}
		else
		{
			m_CharacterSprite.setTexture(m_FemaleCharacterTexture);
		}
		m_CharacterSprite.setPosition(m_CharacterPosition);
		m_CharacterSprite.setScale(m_SpriteSize / m_CharacterSprite.getLocalBounds().width, m_SpriteSize / m_CharacterSprite.getLocalBounds().height);

		std::stringstream stream;
		stream << m_TaxString << m_CurrentMapRegion->m_RegionTax;
		m_RegionTaxText.setString(stream.str());
		m_RegionTaxText.setFillColor(m_OwnerColor);
		stream.str(std::string());
		stream.clear();

		stream << m_ManPowerString << m_CurrentMapRegion->m_ManPower;
		m_RegionManpowerText.setString(stream.str());
		m_RegionManpowerText.setFillColor(m_OwnerColor);
		stream.str(std::string());
		stream.clear();

		stream << m_CurrentMapRegion->m_RegionName;
		m_RegionNameText.setString(stream.str());
		m_RegionNameText.setFillColor(m_OwnerColor);
		stream.str(std::string());
		stream.clear();

		stream << character.m_KingdomName;
		m_KingdomNameText.setString(stream.str());
		m_KingdomNameText.setFillColor(m_OwnerColor);
		stream.str(std::string());
		stream.clear();

		m_RaiseArmyShape.setOutlineColor(m_OwnerColor);
		if (character.m_RaisedArmySize > 0)
		{
			m_RaiseArmyShape.setFillColor(m_OwnerColor);
			m_RaiseArmySprite.setColor(m_OwnerColor);
		}
		else
		{
			m_RaiseArmyShape.setFillColor(sf::Color::Transparent);
			m_RaiseArmySprite.setColor(sf::Color::White);
		}

		for (unsigned int index = 0; index < NUMBER_OF_BUILDING_SLOTS; index++)
		{
			m_BuildingSlotShapes[index].setOutlineColor(m_OwnerColor);
			if (m_CurrentMapRegion->m_BuildingSlots[index].m_Finished)
			{
				m_BuildingSlotSprites[index].setColor(m_OwnerColor);
			}
			else
			{
				m_BuildingSlotShapes[index].setFillColor(sf::Color::Transparent);
			}
		}
	}
}

void RegionWindow::handleWindow()
{
	InputHandler::setRegionWindowOpen(m_Visible);
	if (InputHandler::getLeftMouseReleased() && !InputHandler::getPlayerUnitSelected() && m_Open && !m_Visible)
	{
		openWindow();
	}
	else if (m_Visible && (InputHandler::getPlayerUnitSelected() || InputHandler::getEscapePressed()))
	{
		closeWindow();
	}
	else if (InputHandler::getCharacterWindowOpen() || InputHandler::getWarWindowOpen())
	{
		closeWindow();
	}
}

void RegionWindow::openWindow()
{
	if (!m_Visible)
	{
		m_Visible = true;
		m_WindowShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));
		m_DaySubscriptionHandle = Time::m_GameDate.subscribeToDayChange([](void* data) { RegionWindow& regionWindow = *static_cast<RegionWindow*>(data); regionWindow.onDayChange(); }, static_cast<void*>(this));
		InputHandler::setRegionWindowOpen(true);
	}
}

void RegionWindow::closeWindow()
{
	if (m_Visible)
	{
		m_Open = false;
		m_Visible = false;
		m_WindowShape.setSize(sf::Vector2f());
		Time::m_GameDate.unsubscribeToDayChange(m_DaySubscriptionHandle);
		m_DaySubscriptionHandle = -1;
		InputHandler::setRegionWindowOpen(false);
	}
}

void RegionWindow::clickButton()
{
	if (InputHandler::getLeftMouseReleased() && m_PlayerRegion)
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (m_RaiseArmyShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			Unit& unit = UnitManager::get().getUnitWithId(m_PlayerCharacter->m_UnitEntity);
			if (unit.m_Raised)
			{
				UnitManager::get().dismissUnit(unit.m_UnitID);
				m_PlayerCharacter->m_RaisedArmySize = 0;
				m_RaiseArmyShape.setFillColor(sf::Color::Transparent);
				m_RaiseArmySprite.setColor(sf::Color::White);
			}
			else
			{
				Vector2DInt capitalPosition = Map::get().getRegionCapitalLocation(m_CurrentRegionID);
				UnitManager::get().raiseUnit(unit.m_UnitID, capitalPosition);
				m_PlayerCharacter->m_RaisedArmySize = unit.m_RepresentedForce;
				m_RaiseArmyShape.setFillColor(m_OwnerColor);
				m_RaiseArmySprite.setColor(m_OwnerColor);
			}
			return;
		}
		CharacterManager& characterManager = *CharacterManager::get();
		for (unsigned int index = 0; index < NUMBER_OF_BUILDING_SLOTS; index++)
		{
			if (m_BuildingSlotShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				characterManager.constructBuilding(m_PlayerCharacter->m_CharacterID, index + 1, m_CurrentRegionID, index);
				break;
			}
		}
	}
	if (InputHandler::getRightMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		CharacterWindow& characterWindow = *UIManager::get()->m_CharacterWindow;
		if (m_CharacterSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			closeWindow();
			characterWindow.m_CurrentCharacterID = m_CurrentMapRegion->m_OwnerID;
			characterWindow.checkIfPlayerCharacter();
			characterWindow.updateInfo();
			characterWindow.openWindow();
		}
	}
}

bool RegionWindow::checkIfPlayerRegion()
{
	return m_PlayerRegion = m_CurrentMapRegion->m_OwnerID == m_PlayerCharacter->m_CharacterID;
}
