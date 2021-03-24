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

	RegionWindowSystem()
	{
		AddComponentSignature<RegionWindow>();
		m_EntityManager = &EntityManager::Get();
		m_Window = Window::GetWindow();
	}

	~RegionWindowSystem(){}

	virtual void Start() override
	{
		m_RegionWindows = m_EntityManager->GetComponentArray<RegionWindow>();
	}

	virtual void Update() override
	{
		for (auto entity : m_Entities)
		{
			UpdateInfo(m_RegionWindows[entity]);
			OpenWindow(m_RegionWindows[entity]);

			if (m_RegionWindows[entity].m_Visible)
			{
				ClickButton(m_RegionWindows[entity]);

				int positionX = (int)m_RegionWindows[entity].m_OutlineThickness;
				int positionY = m_Window->getSize().y - (int)(m_RegionWindows[entity].m_SizeY + m_RegionWindows[entity].m_OutlineThickness);

				m_RegionWindows[entity].m_WindowShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX, positionY)));
				m_RegionWindows[entity].m_WindowShape.setFillColor(m_RegionWindows[entity].m_FillColor);
				m_RegionWindows[entity].m_WindowShape.setOutlineColor(m_RegionWindows[entity].m_OwnerColor);
				m_RegionWindows[entity].m_WindowShape.setOutlineThickness(m_RegionWindows[entity].m_OutlineThickness);
				m_RegionWindows[entity].m_WindowShape.setSize(sf::Vector2f(m_RegionWindows[entity].m_SizeX, m_RegionWindows[entity].m_SizeY));

				for (unsigned int index = 0; index < m_RegionWindows[entity].m_NumberOfBuildingSlots; index++)
				{
					m_RegionWindows[entity].m_BuildingSlotShapes[index].setFillColor(m_RegionWindows[entity].m_BuildingSlotColors[index]);
					m_RegionWindows[entity].m_BuildingSlotShapes[index].setOutlineColor(m_RegionWindows[entity].m_OwnerColor);
					m_RegionWindows[entity].m_BuildingSlotShapes[index].setOutlineThickness(m_RegionWindows[entity].m_OutlineThickness * 0.5f);
					m_RegionWindows[entity].m_BuildingSlotShapes[index].setSize(sf::Vector2f(m_RegionWindows[entity].m_SizeX * 0.05f, m_RegionWindows[entity].m_SizeY * 0.05f));
					m_RegionWindows[entity].m_BuildingSlotShapes[index].setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)m_RegionWindows[entity].m_SizeX - 41 - 52 * index, Window::GetWindow()->getSize().y - 61)));
				}

				m_RegionWindows[entity].m_RegionNameText.setFont(m_RegionWindows[entity].m_Font);
				m_RegionWindows[entity].m_RegionNameText.setCharacterSize(m_RegionWindows[entity].m_CharacterSize);
				m_RegionWindows[entity].m_RegionNameText.setStyle(m_RegionWindows[entity].m_Style);
				m_RegionWindows[entity].m_RegionNameText.setString(m_RegionWindows[entity].m_RegionName);
				m_RegionWindows[entity].m_RegionNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_RegionWindows[entity].m_SizeX * 0.1f), positionY)));
				m_RegionWindows[entity].m_RegionNameText.setFillColor(m_RegionWindows[entity].m_OwnerColor);

				m_RegionWindows[entity].m_RegionTaxText.setFont(m_RegionWindows[entity].m_Font);
				m_RegionWindows[entity].m_RegionTaxText.setCharacterSize((int)(m_RegionWindows[entity].m_CharacterSize * 0.5f));
				m_RegionWindows[entity].m_RegionTaxText.setStyle(m_RegionWindows[entity].m_Style);
				m_RegionWindows[entity].m_RegionTaxText.setString("Tax: " + m_RegionWindows[entity].m_RegionTax);
				m_RegionWindows[entity].m_RegionTaxText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_RegionWindows[entity].m_SizeX * 0.7f), positionY + (int)(m_RegionWindows[entity].m_SizeY * 0.4f))));
				m_RegionWindows[entity].m_RegionTaxText.setFillColor(m_RegionWindows[entity].m_OwnerColor);

				m_RegionWindows[entity].m_KingdomNameText.setFont(m_RegionWindows[entity].m_Font);
				m_RegionWindows[entity].m_KingdomNameText.setCharacterSize((int)(m_RegionWindows[entity].m_CharacterSize * 0.5f));
				m_RegionWindows[entity].m_KingdomNameText.setStyle(m_RegionWindows[entity].m_Style);
				m_RegionWindows[entity].m_KingdomNameText.setString(m_RegionWindows[entity].m_KingdomName);
				m_RegionWindows[entity].m_KingdomNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_RegionWindows[entity].m_SizeX * 0.1f), positionY + (int)(m_RegionWindows[entity].m_SizeY * 0.1f))));
				m_RegionWindows[entity].m_KingdomNameText.setFillColor(m_RegionWindows[entity].m_OwnerColor);
			}
		}
	}

	virtual void Render() override
	{
		for (auto entity : m_Entities)
		{
			if (m_RegionWindows[entity].m_Visible)
			{
				m_Window->draw(m_RegionWindows[entity].m_WindowShape);
				m_Window->draw(m_RegionWindows[entity].m_RegionNameText);
				m_Window->draw(m_RegionWindows[entity].m_RegionTaxText);
				m_Window->draw(m_RegionWindows[entity].m_KingdomNameText);
				for (unsigned int index = 0; index < m_RegionWindows[entity].m_NumberOfBuildingSlots; index++)
				{
					m_Window->draw(m_RegionWindows[entity].m_BuildingSlotShapes[index]);
				}
			}
		}
	}

	void UpdateInfo(RegionWindow& regionWindow)
	{
		if (InputHandler::GetLeftMouseClicked() && !InputHandler::GetPlayerUnitSelected() && !regionWindow.m_Visible)
		{
			Vector2DInt mousePosition = InputHandler::GetMouseMapPosition();
			if (Map::m_MapUnitData.find(mousePosition) != Map::m_MapUnitData.end())
			{
				EntityID regionID = Map::m_MapUnitData[mousePosition].m_RegionID;
				if (regionWindow.m_CurrentRegionID != regionID)
				{
					CharacterComponent* characters = m_EntityManager->GetComponentArray<CharacterComponent>();
					regionWindow.m_CurrentRegionID = regionID;
					MapRegion& currentMapRegion = Map::GetRegionById(regionID);
					regionWindow.m_RegionTax = std::to_string(currentMapRegion.m_RegionTax);
					regionWindow.m_RegionName = currentMapRegion.m_RegionName;
					regionWindow.m_KingdomName = characters[currentMapRegion.m_OwnerID].m_KingdomName;
					regionWindow.m_OwnerColor = characters[currentMapRegion.m_OwnerID].m_RegionColor;
					for (unsigned int index = 0; index < regionWindow.m_NumberOfBuildingSlots; index++)
					{
						if (currentMapRegion.m_BuildingSlots[index].m_Finished)
						{
							regionWindow.m_BuildingSlotColors[index] = regionWindow.m_OwnerColor;
							m_EntityManager->GetComponent<UISpriteRenderer>(regionWindow.m_BuildingIconIDs[index]).m_Sprite.setColor(regionWindow.m_OwnerColor);
						}
						else
						{
							regionWindow.m_BuildingSlotColors[index] = sf::Color::Transparent;
							m_EntityManager->GetComponent<UISpriteRenderer>(regionWindow.m_BuildingIconIDs[index]).m_Sprite.setColor(sf::Color::White);
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
	}

	void OpenWindow(RegionWindow& regionWindow)
	{
		InputHandler::SetRegionWindowOpen(regionWindow.m_Visible);
		if (InputHandler::GetLeftMouseReleased() && !InputHandler::GetPlayerUnitSelected() && regionWindow.m_Open)
		{
			Vector2D mousePosition = InputHandler::GetMousePosition();
			if (!regionWindow.m_WindowShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				regionWindow.m_Visible = !regionWindow.m_Visible;
				if (regionWindow.m_Visible)
				{
					m_EntityManager->SetEntityActive(regionWindow.m_RegionPortraitID, true);
					for (unsigned int index = 0; index < regionWindow.m_NumberOfBuildingSlots; index++)
					{
						m_EntityManager->SetEntityActive(regionWindow.m_BuildingIconIDs[index], true);
					}
					m_EntityManager->SetEntityActive(regionWindow.m_BottomPortraitID, false);
				}
				else
				{
					CloseWindow(regionWindow);
					m_EntityManager->SetEntityActive(regionWindow.m_BottomPortraitID, true);
				}
			}
		}
		else if (InputHandler::GetPlayerUnitSelected() || InputHandler::GetCharacterWindowOpen())
		{
			CloseWindow(regionWindow);
		}
	}

	void CloseWindow(RegionWindow& regionWindow)
	{
		regionWindow.m_Open = false;
		regionWindow.m_Visible = false;
		m_EntityManager->SetEntityActive(regionWindow.m_RegionPortraitID, false);
		for (unsigned int index = 0; index < regionWindow.m_NumberOfBuildingSlots; index++)
		{
			m_EntityManager->SetEntityActive(regionWindow.m_BuildingIconIDs[index], false);
		}
	}

	void ClickButton(RegionWindow& regionWindow)
	{
		if (InputHandler::GetLeftMouseReleased())
		{
			Vector2D mousePosition = InputHandler::GetMousePosition();
			for (unsigned int index = 0; index < regionWindow.m_NumberOfBuildingSlots; index++)
			{
				if (regionWindow.m_BuildingSlotShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
				{
					Map::StartConstructionOfBuilding(index + 1, index, regionWindow.m_CurrentRegionID);
					break;
				}
			}
		}
	}
};