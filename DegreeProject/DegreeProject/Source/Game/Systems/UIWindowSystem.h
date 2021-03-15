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
	Transform* windowUIPortraitTransform = nullptr;

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
		// Transform* transforms = m_EntityManager->GetComponentArray<Transform>(); // Uncommented because it's unused!
		UIWindow* UIWindows = m_EntityManager->GetComponentArray<UIWindow>();

		for (auto entity : m_Entities)
		{
			SetOptions(&UIWindows[entity]);
			OpenWindow(&UIWindows[entity]);
			UIWindows[entity].m_Shape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(10, 10)));
			UIWindows[entity].m_Shape.setFillColor(UIWindows[entity].m_FillColor);
			UIWindows[entity].m_Shape.setOutlineColor(UIWindows[entity].m_OutlineColor);
			UIWindows[entity].m_Shape.setOutlineThickness(10.0f);
			UIWindows[entity].m_Shape.setSize(sf::Vector2f(UIWindows[entity].m_SizeX, UIWindows[entity].m_SizeY));
			UIWindows[entity].m_OwnerNameText.setFont(UIWindows[entity].m_Font);
			UIWindows[entity].m_OwnerNameText.setCharacterSize(UIWindows[entity].m_CharacterSize);
			UIWindows[entity].m_OwnerNameText.setStyle(UIWindows[entity].m_Style);
			UIWindows[entity].m_OwnerNameText.setString("OwnerId: " + UIWindows[entity].m_OwnerName);
			UIWindows[entity].m_OwnerNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(UIWindows[entity].m_SizeX * 0.1f), (int)(UIWindows[entity].m_SizeY * 0.1f))));
			UIWindows[entity].m_OwnerNameText.setFillColor(sf::Color::Red);
			UIWindows[entity].m_RegionNameText.setFont(UIWindows[entity].m_Font);
			UIWindows[entity].m_RegionNameText.setCharacterSize(UIWindows[entity].m_CharacterSize);
			UIWindows[entity].m_RegionNameText.setStyle(UIWindows[entity].m_Style);
			UIWindows[entity].m_RegionNameText.setString("Region: " + UIWindows[entity].m_RegionName);
			UIWindows[entity].m_RegionNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(UIWindows[entity].m_SizeX * 0.1f), (int)(UIWindows[entity].m_SizeY * 0.2f))));
			UIWindows[entity].m_RegionNameText.setFillColor(sf::Color::Red);
			UIWindows[entity].m_TaxText.setFont(UIWindows[entity].m_Font);
			UIWindows[entity].m_TaxText.setCharacterSize(UIWindows[entity].m_CharacterSize);
			UIWindows[entity].m_TaxText.setStyle(UIWindows[entity].m_Style);
			UIWindows[entity].m_TaxText.setString("Tax: " + std::to_string(UIWindows[entity].m_RegionTax));
			UIWindows[entity].m_TaxText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(UIWindows[entity].m_SizeX * 0.1f), (int)(UIWindows[entity].m_SizeY * 0.3f))));
			UIWindows[entity].m_TaxText.setFillColor(sf::Color::Red);
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
			}
		}
	}

	void SetOptions(UIWindow* UIWindow)
	{
		if (InputHandler::GetRightMouseClicked() == true && InputHandler::GetPlayerUnitSelected() == false && !UIWindow->m_Visible)
		{
			Vector2DInt mousePosition = InputHandler::GetMouseMapPosition();
			std::vector<MapRegion> regions = MapInfo::GetMapRegions();
			unsigned int regionIndex = 0;
			for each (MapRegion region in regions)
			{
				for each (Vector2DInt position in region.m_MapSquares)
				{
					if (position == mousePosition)
					{
						UIWindow->m_RegionTax = MapInfo::GetRegionTax(regionIndex);
						UIWindow->m_RegionName = MapInfo::GetRegionName(regionIndex);
						UIWindow->m_OwnerName = MapInfo::GetOwnerName(regionIndex);
						UIWindow->m_Open = true;
						break;
					}
					else
					{
						UIWindow->m_Open = false;
					}
				}
				if (UIWindow->m_Open == true)
				{
					break;
				}
				regionIndex++;
			}
		}
	}

	void OpenWindow(UIWindow* UIWindow)
	{
		if (InputHandler::GetRightMouseReleased() == true && InputHandler::GetPlayerUnitSelected() == false && UIWindow->m_Open == true)
		{
			UIWindow->m_Visible = !UIWindow->m_Visible;
			if (UIWindow->m_Visible == true)
			{
				m_EntityManager->AddComponent<UISpriteRenderer>(UIWindow->GetID(), "Assets/Graphics/Unit.png", 64, 64, m_AssetHandler);
				windowUIPortraitTransform = &m_EntityManager->GetComponent<Transform>(UIWindow->GetID());
			}
			else
			{
				m_EntityManager->RemoveComponent<UISpriteRenderer>(UIWindow->GetID());
			}
		}
		if (UIWindow->m_Visible == true)
		{
			sf::Vector2f windowPortraitPosition = m_Window->mapPixelToCoords(sf::Vector2i((int)(UIWindow->m_SizeX * 0.1f), (int)(UIWindow->m_SizeY * 0.025f)));
			windowUIPortraitTransform->m_Position = Vector2D(windowPortraitPosition.x, windowPortraitPosition.y);
		}
	}
};