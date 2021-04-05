#pragma once

#include <SFML/Graphics.hpp>
#include "Engine/Window.h"
#include "Engine/InputHandler.h"
#include "Game/Data/Unit.h"
#include "Game/Systems/UnitManager.h"

const static unsigned int NUMBER_OF_BUILDING_SLOTS = 4;

struct RegionWindow
{
	sf::RectangleShape m_WindowShape;
	sf::RectangleShape m_BuildingSlotShapes[NUMBER_OF_BUILDING_SLOTS];
	sf::RectangleShape m_RaiseArmyShape;
	sf::Color m_BuildingSlotColors[NUMBER_OF_BUILDING_SLOTS];
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Red;
	sf::Color m_RaiseArmyColor = sf::Color::Transparent;
	float m_SizeX = 600.0f;
	float m_SizeY = 600.0f;
	float m_OutlineThickness = 10.0f;
	sf::Text m_RegionNameText;
	sf::Text m_RegionTaxText;
	sf::Text m_KingdomNameText;
	sf::Font m_Font;
	std::string m_RegionName = "";
	std::string m_RegionTax = "";
	std::string m_KingdomName = "";
	int m_CharacterSize = 50;
	bool m_Visible = false;
	bool m_Open = false;
	unsigned int m_CurrentRegionID = 0;
	sf::RectangleShape m_BuildingProgressShape[NUMBER_OF_BUILDING_SLOTS];
	sf::Sprite m_BuildingSlotSprites[NUMBER_OF_BUILDING_SLOTS];
	sf::Texture m_BuildingSlotTextures[NUMBER_OF_BUILDING_SLOTS];
	sf::Texture m_RaiseArmyTexture;
	sf::Sprite m_RaiseArmySprite;
	sf::Texture m_CharacterTexture;
	sf::Sprite m_CharacterSprite;
	int m_SpriteSize = 64;
	float m_IconSlotPositionX = 0.0f;
	float m_IconSlotPositionOffset = 0.0f;
	float m_IconSlotPositionY = 0.0f;

#pragma warning(push)
#pragma warning(disable: 26812)
	sf::Text::Style m_Style = sf::Text::Regular;
#pragma warning(pop)

	sf::RenderWindow* m_Window = nullptr;
	Character* m_PlayerCharacter = nullptr;
	MapRegion* m_CurrentMapRegion = nullptr;
	bool m_PlayerRegion = false;

	const std::string m_TaxString = "Tax: ";

	float m_ProgressMeterWidth = 64.0f;
	float m_ProgressMeterBorder = 1.0f;
	float m_ProgressMeterDoubleBorder;
	sf::Color m_OuterBuildingProgressColor = sf::Color(25, 25, 25, 250);
	UIID m_OwnedUIWindow = INVALID_UI_ID;

	sf::Vector2f m_CharacterPosition = sf::Vector2f();

	RegionWindow(UIID id, sf::Font font)
	{
		m_OwnedUIWindow = id;
		m_Font = font;
		m_Window = Window::getWindow();
		m_ProgressMeterDoubleBorder = m_ProgressMeterBorder * 2;

		m_BuildingSlotTextures[0] = AssetHandler::get().getTextureAtPath("Assets/Graphics/FortIcon.png");
		m_BuildingSlotTextures[1] = AssetHandler::get().getTextureAtPath("Assets/Graphics/BarackIcon.png");
		m_BuildingSlotTextures[2] = AssetHandler::get().getTextureAtPath("Assets/Graphics/MarketIcon.png");
		m_BuildingSlotTextures[3] = AssetHandler::get().getTextureAtPath("Assets/Graphics/WallIcon.png");

		m_RaiseArmyTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/soldier unit.png");
		m_CharacterTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/harold.jpg");

		m_IconSlotPositionX = m_SizeX - m_SpriteSize - m_OutlineThickness;
		m_IconSlotPositionOffset = m_SpriteSize + m_OutlineThickness * 2;
		m_IconSlotPositionY = m_Window->getSize().y - m_SpriteSize - m_OutlineThickness * 3;

		m_CharacterPosition = sf::Vector2f(m_OutlineThickness + m_SpriteSize, (float)(m_Window->getSize().y - m_SpriteSize * 8));
	}

	void start()
	{
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

		m_KingdomNameText.setFont(m_Font);
		m_KingdomNameText.setCharacterSize((int)(m_CharacterSize * 0.5f));
		m_KingdomNameText.setStyle(m_Style);
	}

	void update()
	{
		updateInfo();
		handleWindow();

		if (m_Visible)
		{
			clickButton();

			int positionX = (int)m_OutlineThickness;
			int positionY = m_Window->getSize().y - (int)(m_SizeY + m_OutlineThickness);

			m_WindowShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX, positionY)));
			m_WindowShape.setOutlineColor(m_OwnerColor);

			int iconSlotPositionX = (int)(m_SizeX - m_SpriteSize - m_OutlineThickness);
			int iconSlotPositionOffset = (int)(m_SpriteSize + m_OutlineThickness * 2);
			int iconSlotPositionY = (int)(Window::getWindow()->getSize().y - m_SpriteSize - m_OutlineThickness * 3);

			for (unsigned int index = 0; index < NUMBER_OF_BUILDING_SLOTS; index++)
			{
				m_BuildingSlotShapes[index].setFillColor(m_BuildingSlotColors[index]);
				m_BuildingSlotShapes[index].setOutlineColor(m_OwnerColor);
				m_BuildingSlotShapes[index].setPosition(m_Window->mapPixelToCoords(sf::Vector2i(iconSlotPositionX - iconSlotPositionOffset * index, iconSlotPositionY)));
			}

			m_RaiseArmyShape.setFillColor(m_RaiseArmyColor);
			m_RaiseArmyShape.setOutlineColor(m_OwnerColor);
			m_RaiseArmyShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(iconSlotPositionX, iconSlotPositionY - iconSlotPositionOffset)));

			m_RegionNameText.setString(m_RegionName);
			m_RegionNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_SizeX * 0.1f), positionY)));
			m_RegionNameText.setFillColor(m_OwnerColor);

			m_RegionTaxText.setString(m_TaxString + m_RegionTax);
			m_RegionTaxText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_SizeX * 0.7f), positionY + (int)(m_SizeY * 0.4f))));
			m_RegionTaxText.setFillColor(m_OwnerColor);

			m_KingdomNameText.setString(m_KingdomName);
			m_KingdomNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_SizeX * 0.1f), positionY + (int)(m_SizeY * 0.1f))));
			m_KingdomNameText.setFillColor(m_OwnerColor);
		}
	}

	void render()
	{
		if (m_Visible)
		{
			m_Window->draw(m_WindowShape);
			m_Window->draw(m_RegionNameText);
			m_Window->draw(m_RegionTaxText);
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

	void displayProgressMeter(unsigned int index)
	{
		int buildingProgress = m_CurrentMapRegion->m_BuildingSlots[index].m_DaysBuilt;
		if (!m_CurrentMapRegion->m_BuildingSlots[index].m_Finished && buildingProgress != 0)
		{
			float progressWidth = m_ProgressMeterWidth - m_ProgressMeterDoubleBorder;
			progressWidth *= (float)buildingProgress / (float)GameData::m_Buildings[index + 1].m_DaysToConstruct;

			m_BuildingProgressShape[index].setPosition(m_BuildingSlotShapes[index].getPosition());
			m_BuildingProgressShape[index].setSize({ progressWidth, m_BuildingSlotShapes[index].getSize().y });
			m_BuildingProgressShape[index].setFillColor(m_OwnerColor);

			Window::getWindow()->draw(m_BuildingProgressShape[index]);
		}
		else if(m_CurrentMapRegion->m_BuildingSlots[index].m_Finished)
		{
			m_BuildingSlotColors[index] = m_OwnerColor;
		}
	}

	void updateInfo()
	{
		if (InputHandler::getLeftMouseClicked() && !InputHandler::getPlayerUnitSelected() && !InputHandler::getCharacterWindowOpen())
		{
			Vector2D mousePosition = InputHandler::getMousePosition();
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

					if (m_CurrentRegionID != regionID)
					{
						m_CurrentRegionID = regionID;
						checkIfPlayerRegion(m_CurrentRegionID);
						m_CurrentMapRegion = &Map::get().getRegionById(m_CurrentRegionID);
						m_RegionTax = std::to_string(m_CurrentMapRegion->m_RegionTax);
						m_RegionName = m_CurrentMapRegion->m_RegionName;
						Character character = CharacterManager::get()->getCharacter(m_CurrentMapRegion->m_OwnerID);
						m_KingdomName = character.m_KingdomName;
						m_OwnerColor = character.m_RegionColor;
						if (character.m_RaisedArmySize > 0)
						{
							m_RaiseArmyColor = m_OwnerColor;
						}
						else
						{
							m_RaiseArmyColor = sf::Color::Transparent;
						}
						for (unsigned int index = 0; index < NUMBER_OF_BUILDING_SLOTS; index++)
						{
							if (m_CurrentMapRegion->m_BuildingSlots[index].m_Finished)
							{
								m_BuildingSlotColors[index] = m_OwnerColor;
								m_BuildingSlotSprites[index].setColor(m_OwnerColor);
							}
							else
							{
								m_BuildingSlotColors[index] = sf::Color::Transparent;
								m_BuildingSlotSprites[index].setColor(sf::Color::White);
							}
						}
					}
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
			Vector2D mousePosition = InputHandler::getMousePosition();
			if (!m_WindowShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				closeWindow();
			}
		}
	}

	void handleWindow()
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

	void openWindow()
	{
		m_Visible = true;
		m_WindowShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));
	}

	void closeWindow()
	{
		m_Open = false;
		m_Visible = false;
		m_WindowShape.setSize(sf::Vector2f());
	}

	void clickButton()
	{
		if (InputHandler::getLeftMouseReleased() && m_PlayerRegion)
		{
			Vector2D mousePosition = InputHandler::getMousePosition();
			if (m_RaiseArmyShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				Unit& unit = UnitManager::get().getUnitWithId(m_PlayerCharacter->m_UnitEntity);
				if (unit.m_Raised)
				{
					UnitManager::get().dismissUnit(unit.m_UnitID);
					m_PlayerCharacter->m_RaisedArmySize = 0;
					m_RaiseArmyColor = sf::Color::Transparent;
				}
				else
				{
					Vector2DInt capitalPosition = Map::get().getRegionCapitalLocation(m_CurrentRegionID);
					UnitManager::get().raiseUnit(unit.m_UnitID, capitalPosition);
					m_PlayerCharacter->m_RaisedArmySize = m_PlayerCharacter->m_MaxArmySize;
					m_RaiseArmyColor = m_OwnerColor;
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

	bool checkIfPlayerRegion(size_t currentRegionID)
	{
		for (unsigned int ownedRegionID : m_PlayerCharacter->m_OwnedRegionIDs)
		{
			if (currentRegionID == ownedRegionID)
			{
				m_PlayerRegion = true;
				break;
			}
			else
			{
				m_PlayerRegion = false;
			}
		}
		return m_PlayerRegion;
	}

	void updateSprites()
	{
		for (unsigned int index = 0; index < NUMBER_OF_BUILDING_SLOTS; index++)
		{
			updateSprite(m_BuildingSlotSprites[index], m_BuildingSlotTextures[index], sf::Vector2f(m_IconSlotPositionX - m_IconSlotPositionOffset * index, m_IconSlotPositionY));
		}

		updateSprite(m_RaiseArmySprite, m_RaiseArmyTexture, sf::Vector2f(m_IconSlotPositionX, m_IconSlotPositionY - m_IconSlotPositionOffset));
		updateSprite(m_CharacterSprite, m_CharacterTexture, m_CharacterPosition);
	}

	void updateSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position)
	{
		sprite.setTexture(texture, true);
		sprite.setPosition(Window::getWindow()->mapPixelToCoords(sf::Vector2i((int)position.x, (int)position.y)));

		sf::FloatRect localSize = sprite.getLocalBounds();

		sprite.setScale(m_SpriteSize / localSize.width, m_SpriteSize / localSize.height);
		
		m_Window->draw(sprite);
	}
};