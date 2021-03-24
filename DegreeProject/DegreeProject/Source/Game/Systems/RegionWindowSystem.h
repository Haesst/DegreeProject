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

				m_RegionWindows[entity].m_BuildingSlotShape.setFillColor(sf::Color::Transparent);
				m_RegionWindows[entity].m_BuildingSlotShape.setOutlineColor(m_RegionWindows[entity].m_OwnerColor);
				m_RegionWindows[entity].m_BuildingSlotShape.setOutlineThickness(m_RegionWindows[entity].m_OutlineThickness * 0.5f);
				m_RegionWindows[entity].m_BuildingSlotShape.setSize(sf::Vector2f(m_RegionWindows[entity].m_SizeX * 0.05f, m_RegionWindows[entity].m_SizeY * 0.05f));
				m_RegionWindows[entity].m_BuildingSlotShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)m_RegionWindows[entity].m_SizeX - 41, Window::GetWindow()->getSize().y - 61)));

				m_RegionWindows[entity].m_BuildingSlotShape2.setFillColor(sf::Color::Transparent);
				m_RegionWindows[entity].m_BuildingSlotShape2.setOutlineColor(m_RegionWindows[entity].m_OwnerColor);
				m_RegionWindows[entity].m_BuildingSlotShape2.setOutlineThickness(m_RegionWindows[entity].m_OutlineThickness * 0.5f);
				m_RegionWindows[entity].m_BuildingSlotShape2.setSize(sf::Vector2f(m_RegionWindows[entity].m_SizeX * 0.05f, m_RegionWindows[entity].m_SizeY * 0.05f));
				m_RegionWindows[entity].m_BuildingSlotShape2.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)m_RegionWindows[entity].m_SizeX - 41 - 52, Window::GetWindow()->getSize().y - 61)));

				m_RegionWindows[entity].m_BuildingSlotShape3.setFillColor(sf::Color::Transparent);
				m_RegionWindows[entity].m_BuildingSlotShape3.setOutlineColor(m_RegionWindows[entity].m_OwnerColor);
				m_RegionWindows[entity].m_BuildingSlotShape3.setOutlineThickness(m_RegionWindows[entity].m_OutlineThickness * 0.5f);
				m_RegionWindows[entity].m_BuildingSlotShape3.setSize(sf::Vector2f(m_RegionWindows[entity].m_SizeX * 0.05f, m_RegionWindows[entity].m_SizeY * 0.05f));
				m_RegionWindows[entity].m_BuildingSlotShape3.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)m_RegionWindows[entity].m_SizeX - 41 - 104, Window::GetWindow()->getSize().y - 61)));

				m_RegionWindows[entity].m_BuildingSlotShape4.setFillColor(sf::Color::Transparent);
				m_RegionWindows[entity].m_BuildingSlotShape4.setOutlineColor(m_RegionWindows[entity].m_OwnerColor);
				m_RegionWindows[entity].m_BuildingSlotShape4.setOutlineThickness(m_RegionWindows[entity].m_OutlineThickness * 0.5f);
				m_RegionWindows[entity].m_BuildingSlotShape4.setSize(sf::Vector2f(m_RegionWindows[entity].m_SizeX * 0.05f, m_RegionWindows[entity].m_SizeY * 0.05f));
				m_RegionWindows[entity].m_BuildingSlotShape4.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)m_RegionWindows[entity].m_SizeX - 41 - 156, Window::GetWindow()->getSize().y - 61)));

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
				m_Window->draw(m_RegionWindows[entity].m_BuildingSlotShape);
				m_Window->draw(m_RegionWindows[entity].m_BuildingSlotShape2);
				m_Window->draw(m_RegionWindows[entity].m_BuildingSlotShape3);
				m_Window->draw(m_RegionWindows[entity].m_BuildingSlotShape4);
				m_Window->draw(m_RegionWindows[entity].m_RegionNameText);
				m_Window->draw(m_RegionWindows[entity].m_RegionTaxText);
				m_Window->draw(m_RegionWindows[entity].m_KingdomNameText);
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
					m_EntityManager->SetEntityActive(regionWindow.m_BuildSlotID, true);
					m_EntityManager->SetEntityActive(regionWindow.m_BuildSlotID2, true);
					m_EntityManager->SetEntityActive(regionWindow.m_BuildSlotID3, true);
					m_EntityManager->SetEntityActive(regionWindow.m_BuildSlotID4, true);
					m_EntityManager->SetEntityActive(regionWindow.m_BottomPortraitID, false);
				}
				else
				{
					m_EntityManager->SetEntityActive(regionWindow.m_RegionPortraitID, false);
					m_EntityManager->SetEntityActive(regionWindow.m_BuildSlotID, false);
					m_EntityManager->SetEntityActive(regionWindow.m_BuildSlotID2, false);
					m_EntityManager->SetEntityActive(regionWindow.m_BuildSlotID3, false);
					m_EntityManager->SetEntityActive(regionWindow.m_BuildSlotID4, false);
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
		m_EntityManager->SetEntityActive(regionWindow.m_BuildSlotID, false);
		m_EntityManager->SetEntityActive(regionWindow.m_BuildSlotID2, false);
		m_EntityManager->SetEntityActive(regionWindow.m_BuildSlotID3, false);
		m_EntityManager->SetEntityActive(regionWindow.m_BuildSlotID4, false);
	}

	void ClickButton(RegionWindow& regionWindow)
	{
		if (InputHandler::GetLeftMouseReleased())
		{
			Vector2D mousePosition = InputHandler::GetMousePosition();
			if (regionWindow.m_BuildingSlotShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				Map::StartConstructionOfBuilding(1, regionWindow.m_CurrentRegionID);
			}
			else if (regionWindow.m_BuildingSlotShape2.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				Map::StartConstructionOfBuilding(2, regionWindow.m_CurrentRegionID);
			}
			else if (regionWindow.m_BuildingSlotShape3.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				Map::StartConstructionOfBuilding(3, regionWindow.m_CurrentRegionID);
			}
			else if (regionWindow.m_BuildingSlotShape4.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				Map::StartConstructionOfBuilding(4, regionWindow.m_CurrentRegionID);
			}
		}
	}
};