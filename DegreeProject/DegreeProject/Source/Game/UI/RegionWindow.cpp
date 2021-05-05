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
#include <sstream>

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

	AssetHandler& assetHandler = AssetHandler::get();

	m_BuildingSlotTextures[0] = assetHandler.getTextureAtPath("Assets/Graphics/FortIcon.png");
	m_BuildingSlotTextures[1] = assetHandler.getTextureAtPath("Assets/Graphics/BarackIcon.png");
	m_BuildingSlotTextures[2] = assetHandler.getTextureAtPath("Assets/Graphics/MarketIcon.png");
	m_BuildingSlotTextures[3] = assetHandler.getTextureAtPath("Assets/Graphics/WallIcon.png");

	m_RaiseArmyTexture = assetHandler.getTextureAtPath("Assets/Graphics/soldier unit.png");
	m_RegionTaxTexture = assetHandler.getTextureAtPath("Assets/Graphics/Coins.png");

	m_IconSlotPositionX = m_SizeX - m_SpriteSize - m_OutlineThickness;
	m_IconSlotPositionOffset = m_SpriteSize + m_OutlineThickness * 2;
	m_IconSlotPositionY = m_Window->getSize().y - m_SpriteSize - m_OutlineThickness * 3;

	m_CharacterPosition = { m_OutlineThickness + m_SpriteSize, (float)(m_Window->getSize().y - m_SpriteSize * 8) };

	m_PlayerCharacter = &CharacterManager::get().getPlayerCharacter();

	float positionX = m_OutlineThickness;
	float positionY = m_Window->getSize().y - m_SizeY - m_OutlineThickness;

	setShape(m_WindowShape, m_FillColor, m_OwnerColor, m_OutlineThickness, {  }, { positionX, positionY });

	for (unsigned int index = 0; index < NUMBER_OF_BUILDING_SLOTS; index++)
	{
		setShape(m_BuildingSlotShapes[index], m_OwnerColor, m_OwnerColor, m_OutlineThickness * 0.5f, { (float)m_SpriteSize, (float)m_SpriteSize }, { m_IconSlotPositionX - m_IconSlotPositionOffset * index, m_IconSlotPositionY });
		setSprite(m_BuildingSlotSprites[index], m_BuildingSlotTextures[index], m_BuildingSlotShapes[index].getPosition());
	}

	setShape(m_RaiseArmyShape, m_FillColor, m_OwnerColor, m_OutlineThickness * 0.5f, { (float)m_SpriteSize, (float)m_SpriteSize }, { m_IconSlotPositionX, m_IconSlotPositionY - m_IconSlotPositionOffset });
	setSprite(m_RaiseArmySprite, m_RaiseArmyTexture, m_RaiseArmyShape.getPosition());

	setText(m_RegionNameText, m_Font, m_CharacterSize * 2, m_OwnerColor, { positionX + m_SizeX * 0.1f, positionY });
	setText(m_KingdomNameText, m_Font, m_CharacterSize, m_OwnerColor, { positionX + m_SizeX * 0.1f, positionY + m_SizeY * 0.1f });

	setText(m_RegionTaxText, m_Font, m_CharacterSize, m_OwnerColor, { positionX + m_SpriteSize * 1.5f, positionY + m_SizeY * 0.5f });
	setText(m_RegionTaxInfoText, m_Font, m_CharacterSize, m_OwnerColor, { m_RegionTaxText.getPosition().x - m_SpriteSize * 0.5f, m_RegionTaxText.getPosition().y - m_SpriteSize * 0.5f }, m_TaxString);
	setSprite(m_RegionTaxSprite, m_RegionTaxTexture, { m_RegionTaxText.getPosition().x - m_SpriteSize * 0.5f, m_RegionTaxText.getPosition().y }, (unsigned int)(m_SpriteSize * 0.5f));

	setText(m_RegionManpowerText, m_Font, m_CharacterSize, m_OwnerColor, { positionX + m_SpriteSize * 1.5f, positionY + m_SizeY * 0.6f });
	setText(m_RegionManpowerInfoText, m_Font, m_CharacterSize, m_OwnerColor, { m_RegionManpowerText.getPosition().x - m_SpriteSize * 0.5f, m_RegionManpowerText.getPosition().y - m_SpriteSize * 0.5f }, m_ManPowerString);
	setSprite(m_RegionManpowerSprite, m_RaiseArmyTexture, { m_RegionManpowerText.getPosition().x - m_SpriteSize * 0.5f, m_RegionManpowerText.getPosition().y }, (unsigned int)(m_SpriteSize * 0.5f));
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
		m_Window->draw(m_KingdomNameText);
		m_Window->draw(m_RaiseArmyShape);
		m_Window->draw(m_RaiseArmySprite);
		m_Window->draw(m_RegionTaxSprite);
		m_Window->draw(m_RegionTaxText);
		if (m_ShowTaxInfo)
		{
			m_Window->draw(m_RegionTaxInfoText);
		}
		m_Window->draw(m_RegionManpowerSprite);
		m_Window->draw(m_RegionManpowerText);
		if (m_ShowManpowerInfo)
		{
			m_Window->draw(m_RegionManpowerInfoText);
		}
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
			InputHandler::setLeftMouseClicked(false);
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
			InputHandler::setRightMouseClicked(false);
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
		setShape(m_BuildingProgressShape[index], m_OwnerColor, m_OwnerColor, 0.0f, { progressWidth, m_BuildingSlotShapes[index].getSize().y }, m_BuildingSlotShapes[index].getPosition());
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
		m_PlayerCharacter = &CharacterManager::get().getPlayerCharacter();

		Character& character = CharacterManager::get().getCharacter(m_CurrentMapRegion->m_OwnerID);

		m_OwnerColor = character.m_RegionColor;

		m_WindowShape.setOutlineColor(m_OwnerColor);

		const char* portraitPath = character.m_PortraitPath.c_str();
		m_CharacterTexture = AssetHandler::get().getTextureAtPath(portraitPath);
		setSprite(m_CharacterSprite, m_CharacterTexture, m_CharacterPosition, m_SpriteSize);

		std::stringstream stream;
		stream << m_CurrentMapRegion->m_RegionTax;
		m_RegionTaxText.setString(stream.str());
		m_RegionTaxText.setFillColor(m_OwnerColor);
		m_RegionTaxInfoText.setFillColor(m_OwnerColor);
		stream.str(std::string());
		stream.clear();

		stream << m_CurrentMapRegion->m_ManPower;
		m_RegionManpowerText.setString(stream.str());
		m_RegionManpowerText.setFillColor(m_OwnerColor);
		m_RegionManpowerInfoText.setFillColor(m_OwnerColor);
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
		if (UnitManager::get().getUnitOfCharacter(character.m_CharacterID).m_Raised)
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
		InputHandler::setLeftMouseReleased(false);
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
		m_WindowShape.setSize({ m_SizeX, m_SizeY });
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
		m_WindowShape.setSize({  });
		Time::m_GameDate.unsubscribeToDayChange(m_DaySubscriptionHandle);
		m_DaySubscriptionHandle = -1;
		InputHandler::setRegionWindowOpen(false);
	}
}

void RegionWindow::clickButton()
{
	if (InputHandler::getLeftMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (m_RegionTaxSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
			m_ShowTaxInfo = true;
		}
		else
		{
			m_ShowTaxInfo = false;
		}
		if (m_RegionManpowerSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
			m_ShowManpowerInfo = true;
		}
		else
		{
			m_ShowManpowerInfo = false;
		}
	}
	if (InputHandler::getLeftMouseReleased() && m_PlayerRegion)
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (m_RaiseArmyShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
			Unit& unit = UnitManager::get().getUnitWithId(m_PlayerCharacter->m_UnitEntity);
			if (unit.m_Raised)
			{
				UnitManager::get().dismissUnit(unit.m_UnitID);
				m_RaiseArmyShape.setFillColor(sf::Color::Transparent);
				m_RaiseArmySprite.setColor(sf::Color::White);
			}
			else
			{
				Vector2DInt capitalPosition = Map::get().getRegionCapitalLocation(m_CurrentRegionID);
				UnitManager::get().raiseUnit(unit.m_UnitID, capitalPosition);
				m_RaiseArmyShape.setFillColor(m_OwnerColor);
				m_RaiseArmySprite.setColor(m_OwnerColor);
			}
			return;
		}
		CharacterManager& characterManager = CharacterManager::get();
		for (unsigned int index = 0; index < NUMBER_OF_BUILDING_SLOTS; index++)
		{
			if (m_BuildingSlotShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				InputHandler::setLeftMouseReleased(false);
				characterManager.constructBuilding(m_PlayerCharacter->m_CharacterID, index + 1, m_CurrentRegionID, index);
				break;
			}
		}
	}
	if (InputHandler::getRightMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		CharacterWindow& characterWindow = *UIManager::get().m_CharacterWindow;
		if (m_CharacterSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setRightMouseReleased(false);
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

void RegionWindow::setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position)
{
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThickness);
	shape.setSize(size);
	shape.setPosition(position);
}

void RegionWindow::setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string)
{
	text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setFillColor(fillColor);
	text.setPosition(position);
	text.setString(string);
}

void RegionWindow::setSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, unsigned int spriteSize)
{
	sprite.setTexture(texture, true);
	sprite.setScale(spriteSize / sprite.getLocalBounds().width, spriteSize / sprite.getLocalBounds().height);
	sprite.setPosition(position);
}
