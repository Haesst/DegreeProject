#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Window.h"
#include "Game/Components/RegionWindow.h"
#include "Engine/InputHandler.h"

struct RegionWindowSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	sf::RenderWindow* m_Window = nullptr;
	RegionWindow* m_RegionWindows = nullptr;
	CharacterComponent* playerCharacter = nullptr;
	MapRegion* m_CurrentMapRegion = nullptr;
	bool m_PlayerRegion = false;

	float m_ProgressMeterWidth = 64.0f;
	float m_ProgressMeterBorder = 1.0f;
	float m_ProgressMeterDoubleBorder;
	sf::Color m_OuterBuildingProgressColor = sf::Color(25, 25, 25, 250);

	RegionWindowSystem()
	{
		addComponentSignature<RegionWindow>();
		m_EntityManager = &EntityManager::get();
		m_Window = Window::getWindow();
		m_ProgressMeterDoubleBorder = m_ProgressMeterBorder * 2;
	}

	~RegionWindowSystem(){}

	virtual void start() override
	{
		m_RegionWindows = m_EntityManager->getComponentArray<RegionWindow>();
#pragma warning(push)
#pragma warning(disable: 26815)
		CharacterSystem* characterSystem = (CharacterSystem*)m_EntityManager->getSystem<CharacterSystem>().get();
#pragma warning(pop)
		playerCharacter = &m_EntityManager->getComponent<CharacterComponent>(characterSystem->getPlayerID());
		for (auto entity : m_Entities)
		{
			m_RegionWindows[entity].m_WindowShape.setFillColor(m_RegionWindows[entity].m_FillColor);
			m_RegionWindows[entity].m_WindowShape.setOutlineThickness(m_RegionWindows[entity].m_OutlineThickness);

			for (unsigned int index = 0; index < m_RegionWindows[entity].m_NumberOfBuildingSlots; index++)
			{
				m_RegionWindows[entity].m_BuildingSlotShapes[index].setOutlineThickness(m_RegionWindows[entity].m_OutlineThickness * 0.5f);
				m_RegionWindows[entity].m_BuildingSlotShapes[index].setSize(sf::Vector2f(64.0f, 64.0f));
			}

			m_RegionWindows[entity].m_RaiseArmyShape.setOutlineThickness(m_RegionWindows[entity].m_OutlineThickness * 0.5f);
			m_RegionWindows[entity].m_RaiseArmyShape.setSize(sf::Vector2f(64.0f, 64.0f));

			m_RegionWindows[entity].m_RegionNameText.setFont(m_RegionWindows[entity].m_Font);
			m_RegionWindows[entity].m_RegionNameText.setCharacterSize(m_RegionWindows[entity].m_CharacterSize);
			m_RegionWindows[entity].m_RegionNameText.setStyle(m_RegionWindows[entity].m_Style);

			m_RegionWindows[entity].m_RegionTaxText.setFont(m_RegionWindows[entity].m_Font);
			m_RegionWindows[entity].m_RegionTaxText.setCharacterSize((int)(m_RegionWindows[entity].m_CharacterSize * 0.5f));
			m_RegionWindows[entity].m_RegionTaxText.setStyle(m_RegionWindows[entity].m_Style);

			m_RegionWindows[entity].m_KingdomNameText.setFont(m_RegionWindows[entity].m_Font);
			m_RegionWindows[entity].m_KingdomNameText.setCharacterSize((int)(m_RegionWindows[entity].m_CharacterSize * 0.5f));
			m_RegionWindows[entity].m_KingdomNameText.setStyle(m_RegionWindows[entity].m_Style);
		}
	}

	virtual void update() override
	{
		for (auto entity : m_Entities)
		{
			updateInfo(m_RegionWindows[entity]);
			handleWindow(m_RegionWindows[entity]);

			if (m_RegionWindows[entity].m_Visible)
			{
				clickButton(m_RegionWindows[entity]);

				int positionX = (int)m_RegionWindows[entity].m_OutlineThickness;
				int positionY = m_Window->getSize().y - (int)(m_RegionWindows[entity].m_SizeY + m_RegionWindows[entity].m_OutlineThickness);

				m_RegionWindows[entity].m_WindowShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX, positionY)));
				m_RegionWindows[entity].m_WindowShape.setOutlineColor(m_RegionWindows[entity].m_OwnerColor);

				int iconSlotPositionX = (int)(m_RegionWindows[entity].m_SizeX - 64 - m_RegionWindows[entity].m_OutlineThickness);
				int iconSlotPositionOffset = (int)(64 + m_RegionWindows[entity].m_OutlineThickness * 2);
				int iconSlotPositionY = (int)(Window::getWindow()->getSize().y - 64 - m_RegionWindows[entity].m_OutlineThickness * 3);

				for (unsigned int index = 0; index < m_RegionWindows[entity].m_NumberOfBuildingSlots; index++)
				{
					m_RegionWindows[entity].m_BuildingSlotShapes[index].setFillColor(m_RegionWindows[entity].m_BuildingSlotColors[index]);
					m_RegionWindows[entity].m_BuildingSlotShapes[index].setOutlineColor(m_RegionWindows[entity].m_OwnerColor);
					m_RegionWindows[entity].m_BuildingSlotShapes[index].setPosition(m_Window->mapPixelToCoords(sf::Vector2i(iconSlotPositionX - iconSlotPositionOffset * index, iconSlotPositionY)));
				}

				m_RegionWindows[entity].m_RaiseArmyShape.setFillColor(m_RegionWindows[entity].m_RaiseArmyColor);
				m_RegionWindows[entity].m_RaiseArmyShape.setOutlineColor(m_RegionWindows[entity].m_OwnerColor);
				m_RegionWindows[entity].m_RaiseArmyShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(iconSlotPositionX, iconSlotPositionY - iconSlotPositionOffset)));

				m_RegionWindows[entity].m_RegionNameText.setString(m_RegionWindows[entity].m_RegionName);
				m_RegionWindows[entity].m_RegionNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_RegionWindows[entity].m_SizeX * 0.1f), positionY)));
				m_RegionWindows[entity].m_RegionNameText.setFillColor(m_RegionWindows[entity].m_OwnerColor);

				m_RegionWindows[entity].m_RegionTaxText.setString("Tax: " + m_RegionWindows[entity].m_RegionTax);
				m_RegionWindows[entity].m_RegionTaxText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_RegionWindows[entity].m_SizeX * 0.7f), positionY + (int)(m_RegionWindows[entity].m_SizeY * 0.4f))));
				m_RegionWindows[entity].m_RegionTaxText.setFillColor(m_RegionWindows[entity].m_OwnerColor);

				m_RegionWindows[entity].m_KingdomNameText.setString(m_RegionWindows[entity].m_KingdomName);
				m_RegionWindows[entity].m_KingdomNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_RegionWindows[entity].m_SizeX * 0.1f), positionY + (int)(m_RegionWindows[entity].m_SizeY * 0.1f))));
				m_RegionWindows[entity].m_KingdomNameText.setFillColor(m_RegionWindows[entity].m_OwnerColor);
			}
		}
	}

	virtual void render() override
	{
		for (auto entity : m_Entities)
		{
			if (m_RegionWindows[entity].m_Visible)
			{
				m_Window->draw(m_RegionWindows[entity].m_WindowShape);
				m_Window->draw(m_RegionWindows[entity].m_RegionNameText);
				m_Window->draw(m_RegionWindows[entity].m_RegionTaxText);
				m_Window->draw(m_RegionWindows[entity].m_KingdomNameText);
				m_Window->draw(m_RegionWindows[entity].m_RaiseArmyShape);
				for (unsigned int index = 0; index < m_RegionWindows[entity].m_NumberOfBuildingSlots; index++)
				{
					m_Window->draw(m_RegionWindows[entity].m_BuildingSlotShapes[index]);
					displayProgressMeter(m_RegionWindows[entity], index);
				}
			}
		}
	}

	void displayProgressMeter(RegionWindow& regionWindow, unsigned int index)
	{
		int buildingProgress = m_CurrentMapRegion->m_BuildingSlots[index].m_DaysBuilt;
		if (!m_CurrentMapRegion->m_BuildingSlots[index].m_Finished && buildingProgress != 0)
		{
			float progressWidth = m_ProgressMeterWidth - m_ProgressMeterDoubleBorder;
			progressWidth *= (float)buildingProgress / (float)GameData::m_Buildings[index + 1].m_DaysToConstruct;

			regionWindow.m_BuildingProgressShape[index].setPosition(regionWindow.m_BuildingSlotShapes[index].getPosition());
			regionWindow.m_BuildingProgressShape[index].setSize({ progressWidth, regionWindow.m_BuildingSlotShapes[index].getSize().y });
			regionWindow.m_BuildingProgressShape[index].setFillColor(regionWindow.m_OwnerColor);

			Window::getWindow()->draw(regionWindow.m_BuildingProgressShape[index]);
		}
	}

	void updateInfo(RegionWindow& regionWindow)
	{
		if (InputHandler::getLeftMouseClicked() && !InputHandler::getPlayerUnitSelected() && !InputHandler::getCharacterWindowOpen())
		{
			Vector2D mousePosition = InputHandler::getMousePosition();
			if (!regionWindow.m_WindowShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				Vector2DInt mouseMapPosition = InputHandler::getMouseMapPosition();
				if (Map::mapSquareDataContainsKey(mouseMapPosition))
				{
					EntityID regionID = 0;
					
					for (auto& squareData : Map::m_MapSquareData)
					{
						if (squareData.m_Position == mouseMapPosition)
						{
							regionID = squareData.m_RegionID;
						}
					}

					if (regionWindow.m_CurrentRegionID != regionID)
					{
						regionWindow.m_CurrentRegionID = regionID;
						checkIfPlayerRegion(regionWindow.m_CurrentRegionID);
						m_CurrentMapRegion = &Map::getRegionById(regionWindow.m_CurrentRegionID);
						regionWindow.m_RegionTax = std::to_string(m_CurrentMapRegion->m_RegionTax);
						regionWindow.m_RegionName = m_CurrentMapRegion->m_RegionName;
						CharacterComponent& character = m_EntityManager->getComponent<CharacterComponent>(m_CurrentMapRegion->m_OwnerID);
						regionWindow.m_KingdomName = character.m_KingdomName;
						regionWindow.m_OwnerColor = character.m_RegionColor;
						if (character.m_RaisedArmySize > 0)
						{
							regionWindow.m_RaiseArmyColor = regionWindow.m_OwnerColor;
						}
						else
						{
							regionWindow.m_RaiseArmyColor = sf::Color::Transparent;
						}
						for (unsigned int index = 0; index < regionWindow.m_NumberOfBuildingSlots; index++)
						{
							if (m_CurrentMapRegion->m_BuildingSlots[index].m_Finished)
							{
								regionWindow.m_BuildingSlotColors[index] = regionWindow.m_OwnerColor;
								m_EntityManager->getComponent<UISpriteRenderer>(regionWindow.m_BuildingIconIDs[index]).m_Sprite.setColor(regionWindow.m_OwnerColor);
							}
							else
							{
								regionWindow.m_BuildingSlotColors[index] = sf::Color::Transparent;
								m_EntityManager->getComponent<UISpriteRenderer>(regionWindow.m_BuildingIconIDs[index]).m_Sprite.setColor(sf::Color::White);
							}
						}
					}
					regionWindow.m_Open = true;
				}
				else
				{
					regionWindow.m_Open = false;
				}
			}
			else
			{
				regionWindow.m_Open = false;
			}
		}
	}

	void handleWindow(RegionWindow& regionWindow)
	{
		InputHandler::setRegionWindowOpen(regionWindow.m_Visible);
		if (InputHandler::getLeftMouseReleased() && !InputHandler::getPlayerUnitSelected() && regionWindow.m_Open && !regionWindow.m_Visible)
		{
			openWindow(regionWindow);
		}
		else if (regionWindow.m_Visible && (InputHandler::getPlayerUnitSelected() || InputHandler::getEscapePressed()))
		{
			closeWindow(regionWindow);
			m_EntityManager->setEntityActive(regionWindow.m_BottomPortraitID, true);
		}
		else if(InputHandler::getCharacterWindowOpen())
		{
			closeWindow(regionWindow);
		}
	}

	void openWindow(RegionWindow& regionWindow)
	{
		regionWindow.m_Visible = true;
		regionWindow.m_WindowShape.setSize(sf::Vector2f(regionWindow.m_SizeX, regionWindow.m_SizeY));
		m_EntityManager->setEntityActive(regionWindow.m_RegionPortraitID, true);
		m_EntityManager->setEntityActive(regionWindow.m_RaiseArmyID, true);
		for (unsigned int index = 0; index < regionWindow.m_NumberOfBuildingSlots; index++)
		{
			m_EntityManager->setEntityActive(regionWindow.m_BuildingIconIDs[index], true);
		}
		m_EntityManager->setEntityActive(regionWindow.m_BottomPortraitID, false);
	}

	void closeWindow(RegionWindow& regionWindow)
	{
		regionWindow.m_Open = false;
		regionWindow.m_Visible = false;
		regionWindow.m_WindowShape.setSize(sf::Vector2f());
		m_EntityManager->setEntityActive(regionWindow.m_RegionPortraitID, false);
		m_EntityManager->setEntityActive(regionWindow.m_RaiseArmyID, false);
		for (unsigned int index = 0; index < regionWindow.m_NumberOfBuildingSlots; index++)
		{
			m_EntityManager->setEntityActive(regionWindow.m_BuildingIconIDs[index], false);
		}
	}

	void clickButton(RegionWindow& regionWindow)
	{
		if (InputHandler::getLeftMouseReleased() && m_PlayerRegion)
		{
			Vector2D mousePosition = InputHandler::getMousePosition();
			if (regionWindow.m_RaiseArmyShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				SpriteRenderer& renderer = m_EntityManager->getComponent<SpriteRenderer>(playerCharacter->m_UnitEntity);
				UnitComponent& unit = m_EntityManager->getComponent<UnitComponent>(playerCharacter->m_UnitEntity);
#pragma warning(push)
#pragma warning(disable: 26815)
				CharacterSystem* characterSystem = (CharacterSystem*)m_EntityManager->getSystem<CharacterSystem>().get();
#pragma warning(pop)
				if (unit.m_Raised)
				{
					characterSystem->dismissUnit(playerCharacter->m_EntityID);
					regionWindow.m_RaiseArmyColor = sf::Color::Transparent;
				}
				else
				{
					Vector2DInt capitalPosition = Map::getRegionCapitalLocation(regionWindow.m_CurrentRegionID);
					characterSystem->raiseUnit(playerCharacter->m_EntityID, true, unit, renderer, capitalPosition);
					regionWindow.m_RaiseArmyColor = regionWindow.m_OwnerColor;
				}
				return;
			}
			for (unsigned int index = 0; index < regionWindow.m_NumberOfBuildingSlots; index++)
			{
				if (regionWindow.m_BuildingSlotShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
				{
					playerCharacter->constructBuilding(index + 1, regionWindow.m_CurrentRegionID, index);
					break;
				}
			}
		}
	}

	bool checkIfPlayerRegion(EntityID currentRegionID)
	{
		for (unsigned int ownedRegionID : playerCharacter->m_OwnedRegionIDs)
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
};