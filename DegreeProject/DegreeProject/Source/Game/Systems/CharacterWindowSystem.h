#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Window.h"
#include "Game/Components/CharacterWindow.h"
#include "Engine/InputHandler.h"

struct CharacterWindowSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	sf::RenderWindow* m_Window = nullptr;
	CharacterWindow* m_CharacterWindows = nullptr;

	CharacterWindowSystem()
	{
		AddComponentSignature<CharacterWindow>();
		m_EntityManager = &EntityManager::Get();
		m_Window = Window::GetWindow();
	}

	~CharacterWindowSystem(){}

	virtual void Start() override
	{
		m_CharacterWindows = m_EntityManager->GetComponentArray<CharacterWindow>();
	}

	virtual void Update() override
	{
		for (auto entity : m_Entities)
		{
			UpdateInfo(m_CharacterWindows[entity]);
			OpenWindow(m_CharacterWindows[entity]);

			if (m_CharacterWindows[entity].m_Visible)
			{
				m_CharacterWindows[entity].m_Shape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)m_CharacterWindows[entity].m_OutlineThickness, (int)m_CharacterWindows[entity].m_OutlineThickness)));
				m_CharacterWindows[entity].m_Shape.setFillColor(m_CharacterWindows[entity].m_FillColor);
				m_CharacterWindows[entity].m_Shape.setOutlineColor(m_CharacterWindows[entity].m_OwnerColor);
				m_CharacterWindows[entity].m_Shape.setOutlineThickness(m_CharacterWindows[entity].m_OutlineThickness);
				m_CharacterWindows[entity].m_Shape.setSize(sf::Vector2f(m_CharacterWindows[entity].m_SizeX, m_CharacterWindows[entity].m_SizeY));
				
				m_CharacterWindows[entity].m_OwnerCharacterText.setFont(m_CharacterWindows[entity].m_Font);
				m_CharacterWindows[entity].m_OwnerCharacterText.setCharacterSize(m_CharacterWindows[entity].m_CharacterSize);
				m_CharacterWindows[entity].m_OwnerCharacterText.setStyle(m_CharacterWindows[entity].m_Style);
				m_CharacterWindows[entity].m_OwnerCharacterText.setString(m_CharacterWindows[entity].m_OwnerTitle + m_CharacterWindows[entity].m_OwnerCharacterName);
				m_CharacterWindows[entity].m_OwnerCharacterText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_CharacterWindows[entity].m_SizeX * 0.34f), (int)(m_CharacterWindows[entity].m_SizeY * 0.025f))));
				m_CharacterWindows[entity].m_OwnerCharacterText.setFillColor(m_CharacterWindows[entity].m_OwnerColor);
				
				m_CharacterWindows[entity].m_OwnerNameText.setFont(m_CharacterWindows[entity].m_Font);
				m_CharacterWindows[entity].m_OwnerNameText.setCharacterSize(m_CharacterWindows[entity].m_CharacterSize);
				m_CharacterWindows[entity].m_OwnerNameText.setStyle(m_CharacterWindows[entity].m_Style);
				m_CharacterWindows[entity].m_OwnerNameText.setString(m_CharacterWindows[entity].m_OwnerKingdomName);
				m_CharacterWindows[entity].m_OwnerNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_CharacterWindows[entity].m_SizeX * 0.1f), (int)(m_CharacterWindows[entity].m_SizeY * 0.1f))));
				m_CharacterWindows[entity].m_OwnerNameText.setFillColor(m_CharacterWindows[entity].m_OwnerColor);
				
				m_CharacterWindows[entity].m_RegionNameText.setFont(m_CharacterWindows[entity].m_Font);
				m_CharacterWindows[entity].m_RegionNameText.setCharacterSize(m_CharacterWindows[entity].m_CharacterSize);
				m_CharacterWindows[entity].m_RegionNameText.setStyle(m_CharacterWindows[entity].m_Style);
				m_CharacterWindows[entity].m_RegionNameText.setString("Region: " + m_CharacterWindows[entity].m_RegionName);
				m_CharacterWindows[entity].m_RegionNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_CharacterWindows[entity].m_SizeX * 0.1f), (int)(m_CharacterWindows[entity].m_SizeY * 0.2f))));
				m_CharacterWindows[entity].m_RegionNameText.setFillColor(m_CharacterWindows[entity].m_OwnerColor);
				
				m_CharacterWindows[entity].m_TaxText.setFont(m_CharacterWindows[entity].m_Font);
				m_CharacterWindows[entity].m_TaxText.setCharacterSize(m_CharacterWindows[entity].m_CharacterSize);
				m_CharacterWindows[entity].m_TaxText.setStyle(m_CharacterWindows[entity].m_Style);
				m_CharacterWindows[entity].m_TaxText.setString("Tax: " + std::to_string(m_CharacterWindows[entity].m_RegionTax));
				m_CharacterWindows[entity].m_TaxText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_CharacterWindows[entity].m_SizeX * 0.1f), (int)(m_CharacterWindows[entity].m_SizeY * 0.3f))));
				m_CharacterWindows[entity].m_TaxText.setFillColor(m_CharacterWindows[entity].m_OwnerColor);
			}
		}
	}

	virtual void Render() override
	{
		for (auto entity : m_Entities)
		{
			if (m_CharacterWindows[entity].m_Visible)
			{
				m_Window->draw(m_CharacterWindows[entity].m_Shape);
				m_Window->draw(m_CharacterWindows[entity].m_OwnerNameText);
				m_Window->draw(m_CharacterWindows[entity].m_RegionNameText);
				m_Window->draw(m_CharacterWindows[entity].m_TaxText);
				m_Window->draw(m_CharacterWindows[entity].m_OwnerCharacterText);
			}
		}
	}

	void UpdateInfo(CharacterWindow& characterWindow)
	{
		if (InputHandler::GetRightMouseClicked() && !InputHandler::GetPlayerUnitSelected() && !characterWindow.m_Visible)
		{
			Vector2DInt mousePosition = InputHandler::GetMouseMapPosition();
			if (Map::m_MapUnitData.find(mousePosition) != Map::m_MapUnitData.end())
			{
				EntityID regionID = Map::m_MapUnitData[mousePosition].m_RegionID;
				if (characterWindow.m_CurrentRegionID != regionID)
				{

					characterWindow.m_CurrentRegionID = regionID;
					MapRegion& currentMapRegion = Map::GetRegionById(regionID);
					characterWindow.m_RegionTax = currentMapRegion.m_RegionTax;
					characterWindow.m_RegionName = currentMapRegion.m_RegionName;
					CharacterComponent& character = m_EntityManager->GetComponent<CharacterComponent>(currentMapRegion.m_OwnerID);
					characterWindow.m_OwnerKingdomName = character.m_KingdomName;
					characterWindow.m_OwnerCharacterName = character.m_Name;
					characterWindow.m_OwnerTitle = characterWindow.titles[(unsigned int)character.m_CharacterTitle];
					characterWindow.m_OwnerColor = character.m_RegionColor;
				}
				characterWindow.m_Open = true;
			}
			else
			{
				characterWindow.m_Open = false;
			}
		}
	}

	void OpenWindow(CharacterWindow& characterWindow)
	{
		InputHandler::SetCharacterWindowOpen(characterWindow.m_Visible);
		if (InputHandler::GetRightMouseReleased() && !InputHandler::GetPlayerUnitSelected() && characterWindow.m_Open)
		{
			characterWindow.m_Visible = !characterWindow.m_Visible;
			if (characterWindow.m_Visible)
			{
				m_EntityManager->SetEntityActive(characterWindow.m_TopPortraitID, true);
				m_EntityManager->SetEntityActive(characterWindow.m_BottomPortraitID, false);
			}
			else
			{
				m_EntityManager->SetEntityActive(characterWindow.m_TopPortraitID, false);
				m_EntityManager->SetEntityActive(characterWindow.m_BottomPortraitID, true);
			}
		}
		else if (InputHandler::GetPlayerUnitSelected() || InputHandler::GetRegionWindowOpen())
		{
			CloseWindow(characterWindow);
		}
	}

	void CloseWindow(CharacterWindow& characterWindow)
	{
		characterWindow.m_Open = false;
		characterWindow.m_Visible = false;
		m_EntityManager->SetEntityActive(characterWindow.m_TopPortraitID, false);
	}
};