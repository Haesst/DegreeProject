#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Time.h"
#include "Engine/Window.h"
#include "ECS/Components/Transform.h"
#include "Game/Components/UIWindow.h"
#include "Game/Components/UISpriteRenderer.h"
#include "Engine/InputHandler.h"

struct UIWindowSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	sf::RenderWindow* m_Window = nullptr;
	AssetHandler* m_AssetHandler= nullptr;

	// Constructor, Runs when the system is initialized
	// Do any kind of init here but remember to register
	// the systems component signature. IE: add every component
	// that the system needs or should look for
	UIWindowSystem()
	{
		AddComponentSignature<UIWindow>();
		m_EntityManager = &EntityManager::Get();
		m_Window = Window::GetWindow();
		m_AssetHandler = new AssetHandler();
	}

	~UIWindowSystem()
	{
		delete m_AssetHandler;
	}

	// Update gets called every frame and loops through every entity that has the signature that
	// the system has registered and do the necessary update
	virtual void Update() override
	{
		UIWindow* UIWindows = m_EntityManager->GetComponentArray<UIWindow>();

		for (auto entity : m_Entities)
		{
			SetOptions(UIWindows[entity]);
			OpenWindow(UIWindows[entity]);
			if (UIWindows[entity].m_Visible)
			{
				UIWindows[entity].m_Shape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(10, 10)));
				UIWindows[entity].m_Shape.setFillColor(UIWindows[entity].m_FillColor);
				UIWindows[entity].m_Shape.setOutlineColor(UIWindows[entity].m_OwnerColor);
				UIWindows[entity].m_Shape.setOutlineThickness(10.0f);
				UIWindows[entity].m_Shape.setSize(sf::Vector2f(UIWindows[entity].m_SizeX, UIWindows[entity].m_SizeY));
				UIWindows[entity].m_OwnerCharacterText.setFont(UIWindows[entity].m_Font);
				UIWindows[entity].m_OwnerCharacterText.setCharacterSize(UIWindows[entity].m_CharacterSize);
				UIWindows[entity].m_OwnerCharacterText.setStyle(UIWindows[entity].m_Style);
				UIWindows[entity].m_OwnerCharacterText.setString(UIWindows[entity].m_OwnerTitle + UIWindows[entity].m_OwnerCharacterName);
				UIWindows[entity].m_OwnerCharacterText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(UIWindows[entity].m_SizeX * 0.34f), (int)(UIWindows[entity].m_SizeY * 0.025f))));
				UIWindows[entity].m_OwnerCharacterText.setFillColor(UIWindows[entity].m_OwnerColor);
				UIWindows[entity].m_OwnerNameText.setFont(UIWindows[entity].m_Font);
				UIWindows[entity].m_OwnerNameText.setCharacterSize(UIWindows[entity].m_CharacterSize);
				UIWindows[entity].m_OwnerNameText.setStyle(UIWindows[entity].m_Style);
				UIWindows[entity].m_OwnerNameText.setString(UIWindows[entity].m_OwnerKingdomName);
				UIWindows[entity].m_OwnerNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(UIWindows[entity].m_SizeX * 0.1f), (int)(UIWindows[entity].m_SizeY * 0.1f))));
				UIWindows[entity].m_OwnerNameText.setFillColor(UIWindows[entity].m_OwnerColor);
				UIWindows[entity].m_RegionNameText.setFont(UIWindows[entity].m_Font);
				UIWindows[entity].m_RegionNameText.setCharacterSize(UIWindows[entity].m_CharacterSize);
				UIWindows[entity].m_RegionNameText.setStyle(UIWindows[entity].m_Style);
				UIWindows[entity].m_RegionNameText.setString("Region: " + UIWindows[entity].m_RegionName);
				UIWindows[entity].m_RegionNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(UIWindows[entity].m_SizeX * 0.1f), (int)(UIWindows[entity].m_SizeY * 0.2f))));
				UIWindows[entity].m_RegionNameText.setFillColor(UIWindows[entity].m_OwnerColor);
				UIWindows[entity].m_TaxText.setFont(UIWindows[entity].m_Font);
				UIWindows[entity].m_TaxText.setCharacterSize(UIWindows[entity].m_CharacterSize);
				UIWindows[entity].m_TaxText.setStyle(UIWindows[entity].m_Style);
				UIWindows[entity].m_TaxText.setString("Tax: " + std::to_string(UIWindows[entity].m_RegionTax));
				UIWindows[entity].m_TaxText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(UIWindows[entity].m_SizeX * 0.1f), (int)(UIWindows[entity].m_SizeY * 0.3f))));
				UIWindows[entity].m_TaxText.setFillColor(UIWindows[entity].m_OwnerColor);
			}
		}
	}

	virtual void Render() override
	{
		UIWindow* UIWindows = m_EntityManager->GetComponentArray<UIWindow>();

		for (auto entity : m_Entities)
		{
			if (UIWindows[entity].m_Visible == true)
			{
				m_Window->draw(UIWindows[entity].m_Shape);
				m_Window->draw(UIWindows[entity].m_OwnerNameText);
				m_Window->draw(UIWindows[entity].m_RegionNameText);
				m_Window->draw(UIWindows[entity].m_TaxText);
				m_Window->draw(UIWindows[entity].m_OwnerCharacterText);
			}
		}
	}

	void SetOptions(UIWindow& UIWindow)
	{
		if (InputHandler::GetRightMouseClicked() == true && InputHandler::GetPlayerUnitSelected() == false && !UIWindow.m_Visible)
		{
			Vector2DInt mousePosition = InputHandler::GetMouseMapPosition();
			if (Map::m_MapUnitData.find(mousePosition) != Map::m_MapUnitData.end())
			{
				EntityID regionID = Map::m_MapUnitData[mousePosition].m_RegionID;
				if (UIWindow.m_CurrentRegionID != regionID)
				{
					CharacterComponent* characters = m_EntityManager->GetComponentArray<CharacterComponent>();
					UIWindow.m_CurrentRegionID = regionID;
					MapRegion& currentMapRegion = Map::GetRegionById(regionID);
					UIWindow.m_RegionTax = currentMapRegion.m_RegionTax;
					UIWindow.m_RegionName = currentMapRegion.m_RegionName;
					UIWindow.m_OwnerKingdomName = characters[currentMapRegion.m_OwnerID].m_KingdomName;
					UIWindow.m_OwnerCharacterName = characters[currentMapRegion.m_OwnerID].m_Name;
					UIWindow.m_OwnerTitle = UIWindow.titles[(unsigned int)characters[currentMapRegion.m_OwnerID].m_CharacterTitle];
					UIWindow.m_OwnerColor = characters[currentMapRegion.m_OwnerID].m_RegionColor;
				}
				UIWindow.m_Open = true;
			}
			else
			{
				UIWindow.m_Open = false;
			}
		}
	}

	void OpenWindow(UIWindow& UIWindow)
	{
		if (InputHandler::GetRightMouseReleased() == true && InputHandler::GetPlayerUnitSelected() == false && UIWindow.m_Open == true)
		{
			UIWindow.m_Visible = !UIWindow.m_Visible;
			if (UIWindow.m_Visible == true)
			{
				m_EntityManager->SetEntityActive(UIWindow.m_BottomPortraitUI, false);
				m_EntityManager->SetEntityActive(UIWindow.m_TopPortraitUI, true);
			}
			else
			{
				m_EntityManager->SetEntityActive(UIWindow.m_TopPortraitUI, false);
				m_EntityManager->SetEntityActive(UIWindow.m_BottomPortraitUI, true);
			}
		}
	}
};