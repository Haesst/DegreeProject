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
	bool m_PlayerRegion = false;

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
#pragma warning(push)
#pragma warning(disable: 26815)
		CharacterSystem* characterSystem = (CharacterSystem*)m_EntityManager->GetSystem<CharacterSystem>().get();
#pragma warning(pop)
		playerCharacter = &m_EntityManager->GetComponent<CharacterComponent>(characterSystem->GetPlayerID());
	}

	virtual void Update() override
	{
		for (auto entity : m_Entities)
		{
			UpdateInfo(m_RegionWindows[entity]);
			HandleWindow(m_RegionWindows[entity]);

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

				m_RegionWindows[entity].m_RaiseArmyShape.setFillColor(m_RegionWindows[entity].m_RaiseArmyColor);
				m_RegionWindows[entity].m_RaiseArmyShape.setOutlineColor(m_RegionWindows[entity].m_OwnerColor);
				m_RegionWindows[entity].m_RaiseArmyShape.setOutlineThickness(m_RegionWindows[entity].m_OutlineThickness * 0.5f);
				m_RegionWindows[entity].m_RaiseArmyShape.setSize(sf::Vector2f(m_RegionWindows[entity].m_SizeX * 0.05f, m_RegionWindows[entity].m_SizeY * 0.05f));
				m_RegionWindows[entity].m_RaiseArmyShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)m_RegionWindows[entity].m_SizeX - 41, Window::GetWindow()->getSize().y - 61 - 52)));

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
				m_Window->draw(m_RegionWindows[entity].m_RaiseArmyShape);
				for (unsigned int index = 0; index < m_RegionWindows[entity].m_NumberOfBuildingSlots; index++)
				{
					m_Window->draw(m_RegionWindows[entity].m_BuildingSlotShapes[index]);
				}
			}
		}
	}

	void UpdateInfo(RegionWindow& regionWindow)
	{
		if (InputHandler::GetLeftMouseClicked() && !InputHandler::GetPlayerUnitSelected() && !InputHandler::GetCharacterWindowOpen())
		{
			Vector2D mousePosition = InputHandler::GetMousePosition();
			if (!regionWindow.m_WindowShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				Vector2DInt mouseMapPosition = InputHandler::GetMouseMapPosition();
				if (Map::m_MapUnitData.find(mouseMapPosition) != Map::m_MapUnitData.end())
				{
					EntityID regionID = Map::m_MapUnitData[mouseMapPosition].m_RegionID;
					if (regionWindow.m_CurrentRegionID != regionID)
					{
						regionWindow.m_CurrentRegionID = regionID;
						CheckIfPlayerRegion(regionWindow.m_CurrentRegionID);
						MapRegion& currentMapRegion = Map::GetRegionById(regionID);
						regionWindow.m_RegionTax = std::to_string(currentMapRegion.m_RegionTax);
						regionWindow.m_RegionName = currentMapRegion.m_RegionName;
						CharacterComponent& character = m_EntityManager->GetComponent<CharacterComponent>(currentMapRegion.m_OwnerID);
						regionWindow.m_KingdomName = character.m_KingdomName;
						regionWindow.m_OwnerColor = character.m_RegionColor;
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
			else
			{
				regionWindow.m_Open = false;
			}
		}
	}

	void HandleWindow(RegionWindow& regionWindow)
	{
		InputHandler::SetRegionWindowOpen(regionWindow.m_Visible);
		if (InputHandler::GetLeftMouseReleased() && !InputHandler::GetPlayerUnitSelected() && regionWindow.m_Open && !regionWindow.m_Visible)
		{
			OpenWindow(regionWindow);
		}
		else if (regionWindow.m_Visible && (InputHandler::GetPlayerUnitSelected() || InputHandler::GetEscapePressed()))
		{
			CloseWindow(regionWindow);
			m_EntityManager->SetEntityActive(regionWindow.m_BottomPortraitID, true);
		}
		else if(InputHandler::GetCharacterWindowOpen())
		{
			CloseWindow(regionWindow);
		}
	}

	void OpenWindow(RegionWindow& regionWindow)
	{
		regionWindow.m_Visible = true;
		m_EntityManager->SetEntityActive(regionWindow.m_RegionPortraitID, true);
		m_EntityManager->SetEntityActive(regionWindow.m_RaiseArmyID, true);
		for (unsigned int index = 0; index < regionWindow.m_NumberOfBuildingSlots; index++)
		{
			m_EntityManager->SetEntityActive(regionWindow.m_BuildingIconIDs[index], true);
		}
		m_EntityManager->SetEntityActive(regionWindow.m_BottomPortraitID, false);
	}

	void CloseWindow(RegionWindow& regionWindow)
	{
		regionWindow.m_Open = false;
		regionWindow.m_Visible = false;
		m_EntityManager->SetEntityActive(regionWindow.m_RegionPortraitID, false);
		m_EntityManager->SetEntityActive(regionWindow.m_RaiseArmyID, false);
		for (unsigned int index = 0; index < regionWindow.m_NumberOfBuildingSlots; index++)
		{
			m_EntityManager->SetEntityActive(regionWindow.m_BuildingIconIDs[index], false);
		}
	}

	void ClickButton(RegionWindow& regionWindow)
	{
		if (InputHandler::GetLeftMouseReleased() && m_PlayerRegion)
		{
			Vector2D mousePosition = InputHandler::GetMousePosition();
			if (regionWindow.m_RaiseArmyShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				SpriteRenderer& renderer = m_EntityManager->GetComponent<SpriteRenderer>(playerCharacter->m_UnitEntity);
				UnitComponent& unit = m_EntityManager->GetComponent<UnitComponent>(playerCharacter->m_UnitEntity);
				Vector2DInt capitalPosition = Map::GetRegionCapitalLocation(regionWindow.m_CurrentRegionID);
#pragma warning(push)
#pragma warning(disable: 26815)
				CharacterSystem* characterSystem = (CharacterSystem*)m_EntityManager->GetSystem<CharacterSystem>().get();
#pragma warning(pop)
				characterSystem->RaiseUnit(playerCharacter->m_EntityID, true, unit, renderer, capitalPosition);
				return;
			}
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

	bool CheckIfPlayerRegion(EntityID currentRegionID)
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