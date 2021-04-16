#include "Game/UI/RegionWindow.h"
#include "Engine/Window.h"
#include "Engine/InputHandler.h"
#include "Game/Data/Unit.h"
#include "Game/Systems/UnitManager.h"
#include "Game/Map/Map.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/Data/Character.h"

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

	m_WindowShape.setFillColor(m_FillColor);
	m_WindowShape.setOutlineThickness(m_OutlineThickness);

	for (unsigned int index = 0; index < NUMBER_OF_BUILDING_SLOTS; index++)
	{
		m_BuildingSlotShapes[index].setOutlineThickness(m_OutlineThickness * 0.5f);
		m_BuildingSlotShapes[index].setSize(sf::Vector2f((float)m_SpriteSize, (float)m_SpriteSize));
	}

	m_RaiseArmyShape.setOutlineThickness(m_OutlineThickness * 0.5f);
	m_RaiseArmyShape.setSize(sf::Vector2f((float)m_SpriteSize, (float)m_SpriteSize));

	m_RegionNameText.setFont(m_Font);
	m_RegionNameText.setCharacterSize(m_CharacterSize);
	m_RegionNameText.setStyle(m_Style);

	m_RegionTaxText.setFont(m_Font);
	m_RegionTaxText.setCharacterSize((int)(m_CharacterSize * 0.5f));
	m_RegionTaxText.setStyle(m_Style);

	m_RegionManpowerText.setFont(m_Font);
	m_RegionManpowerText.setCharacterSize((int)(m_CharacterSize * 0.5f));
	m_RegionManpowerText.setStyle(m_Style);

	m_KingdomNameText.setFont(m_Font);
	m_KingdomNameText.setCharacterSize((int)(m_CharacterSize * 0.5f));
	m_KingdomNameText.setStyle(m_Style);
}

void RegionWindow::update()
{
	clickOnMap();
	handleWindow();

	if (m_Visible)
	{
		clickButton();

		int positionX = (int)m_OutlineThickness;
		int positionY = m_Window->getSize().y - (int)(m_SizeY + m_OutlineThickness);

		m_WindowShape.setPosition(sf::Vector2f(positionX, positionY));

		int iconSlotPositionX = (int)(m_SizeX - m_SpriteSize - m_OutlineThickness);
		int iconSlotPositionOffset = (int)(m_SpriteSize + m_OutlineThickness * 2);
		int iconSlotPositionY = (int)(m_Window->getSize().y - m_SpriteSize - m_OutlineThickness * 3);

		for (unsigned int index = 0; index < NUMBER_OF_BUILDING_SLOTS; index++)
		{
			m_BuildingSlotShapes[index].setPosition(sf::Vector2f(iconSlotPositionX - iconSlotPositionOffset * index, iconSlotPositionY));
		}

		m_RaiseArmyShape.setPosition(sf::Vector2f(iconSlotPositionX, iconSlotPositionY - iconSlotPositionOffset));

		m_RegionNameText.setPosition(sf::Vector2f(positionX + (int)(m_SizeX * 0.1f), positionY));

		m_RegionTaxText.setPosition(sf::Vector2f(positionX + (int)(m_SizeX * 0.1f), positionY + (int)(m_SizeY * 0.5f)));

		m_RegionManpowerText.setPosition(sf::Vector2f(positionX + (int)(m_SizeX * 0.1f), positionY + (int)(m_SizeY * 0.6f)));

		m_KingdomNameText.setPosition(sf::Vector2f(positionX + (int)(m_SizeX * 0.1f), positionY + (int)(m_SizeY * 0.1f)));
	}
}

void RegionWindow::render()
{
	if (m_Visible)
	{
		m_Window->draw(m_WindowShape);
		m_Window->draw(m_RegionNameText);
		m_Window->draw(m_RegionTaxText);
		m_Window->draw(m_RegionManpowerText);
		m_Window->draw(m_KingdomNameText);
		m_Window->draw(m_RaiseArmyShape);
		for (unsigned int index = 0; index < NUMBER_OF_BUILDING_SLOTS; index++)
		{
			m_Window->draw(m_BuildingSlotShapes[index]);
			displayProgressMeter(index);
		}
		updateSprites();
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
			if (Map::get().mapSquareDataContainsKey(mouseMapPosition))
			{
				size_t regionID = 0;

				for (auto& squareData : Map::get().m_MapSquareData)
				{
					if (squareData.m_Position == mouseMapPosition)
					{
						regionID = squareData.m_RegionID;
					}
				}
				m_CurrentRegionID = regionID;
				m_CurrentMapRegion = &Map::get().getRegionById(m_CurrentRegionID);
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
		Character character = CharacterManager::get()->getCharacter(m_CurrentMapRegion->m_OwnerID);
		m_OwnerColor = character.m_RegionColor;

		m_WindowShape.setOutlineColor(m_OwnerColor);

		m_RegionTaxText.setString(m_TaxString + std::to_string(m_CurrentMapRegion->m_RegionTax));
		m_RegionTaxText.setFillColor(m_OwnerColor);

		m_RegionManpowerText.setString(m_ManPowerString + std::to_string(m_CurrentMapRegion->m_ManPower));
		m_RegionManpowerText.setFillColor(m_OwnerColor);

		m_RegionNameText.setString(m_CurrentMapRegion->m_RegionName);
		m_RegionNameText.setFillColor(m_OwnerColor);

		m_KingdomNameText.setString(character.m_KingdomName);
		m_KingdomNameText.setFillColor(m_OwnerColor);

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
	else if (InputHandler::getCharacterWindowOpen())
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
		for (unsigned int index = 0; index < NUMBER_OF_BUILDING_SLOTS; index++)
		{
			if (m_BuildingSlotShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				CharacterManager::get()->constructBuilding(m_PlayerCharacter->m_CharacterID, index + 1, m_CurrentRegionID, index);
				break;
			}
		}
	}
}

bool RegionWindow::checkIfPlayerRegion()
{
	return m_PlayerRegion = m_CurrentMapRegion->m_OwnerID == m_PlayerCharacter->m_CharacterID;
}

void RegionWindow::updateSprites()
{
	for (unsigned int index = 0; index < NUMBER_OF_BUILDING_SLOTS; index++)
	{
		updateSprite(m_BuildingSlotSprites[index], m_BuildingSlotTextures[index], sf::Vector2f(m_IconSlotPositionX - m_IconSlotPositionOffset * index, m_IconSlotPositionY));
	}

	updateSprite(m_RaiseArmySprite, m_RaiseArmyTexture, sf::Vector2f(m_IconSlotPositionX, m_IconSlotPositionY - m_IconSlotPositionOffset));

	if (CharacterManager::get()->getCharacter(m_CurrentMapRegion->m_OwnerID).m_Gender == Gender::Male)
	{
		updateSprite(m_CharacterSprite, m_MaleCharacterTexture, m_CharacterPosition);
	}
	else
	{
		updateSprite(m_CharacterSprite, m_FemaleCharacterTexture, m_CharacterPosition);
	}
}

void RegionWindow::updateSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, int spriteSize)
{
	sprite.setTexture(texture, true);
	sprite.setPosition(position);

	sf::FloatRect localSize = sprite.getLocalBounds();

	sprite.setScale(spriteSize / localSize.width, spriteSize / localSize.height);

	m_Window->draw(sprite);
}
