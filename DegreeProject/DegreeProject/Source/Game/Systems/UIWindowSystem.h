#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Time.h"
#include "Engine/Window.h"
#include "ECS/Components/Transform.h"
#include "Game/Components/UIWindow.h";
#include "Engine/InputHandler.h"

struct UIWindowSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	sf::RenderWindow* m_Window;

	// Constructor, Runs when the system is initialized
	// Do any kind of init here but remember to register
	// the systems component signature. IE: add every component
	// that the system needs or should look for
	UIWindowSystem()
	{
		AddComponentSignature<UIWindow>();
		m_EntityManager = &EntityManager::Get();
		m_Window = Window::GetWindow();
	}

	// Update gets called every frame and loops through every entity that has the signature that
	// the system has registered and do the necessary update
	virtual void Update() override
	{
		Transform* transforms = m_EntityManager->GetComponentArray<Transform>();
		UIWindow* UIWindows = m_EntityManager->GetComponentArray<UIWindow>();

		for (auto entity : m_Entities)
		{
			SetOptions(&transforms[entity], &UIWindows[entity]);
			OpenWindow(&transforms[entity], &UIWindows[entity]);
			UIWindows[entity].m_Shape.setPosition(10.0f, 10.0f);
			UIWindows[entity].m_Shape.setFillColor(UIWindows[entity].m_FillColor);
			UIWindows[entity].m_Shape.setOutlineColor(UIWindows[entity].m_OutlineColor);
			UIWindows[entity].m_Shape.setOutlineThickness(10.0f);
			UIWindows[entity].m_Shape.setSize(sf::Vector2(UIWindows[entity].m_SizeX, UIWindows[entity].m_SizeY));
			UIWindows[entity].m_TaxText.setFont(UIWindows[entity].m_Font);
			UIWindows[entity].m_RegionNameText.setFont(UIWindows[entity].m_Font);
			UIWindows[entity].m_TaxText.setCharacterSize(UIWindows[entity].m_CharacterSize);
			UIWindows[entity].m_RegionNameText.setCharacterSize(UIWindows[entity].m_CharacterSize);
			UIWindows[entity].m_TaxText.setStyle(UIWindows[entity].m_Style);
			UIWindows[entity].m_RegionNameText.setStyle(UIWindows[entity].m_Style);
			UIWindows[entity].m_TaxText.setString("Tax: " + std::to_string(UIWindows[entity].m_RegionTax));
			UIWindows[entity].m_RegionNameText.setString("Name: " + UIWindows[entity].m_RegionName);
			UIWindows[entity].m_TaxText.setPosition(UIWindows[entity].m_SizeX * 0.25f, UIWindows[entity].m_SizeY * 0.5f);
			UIWindows[entity].m_RegionNameText.setPosition(UIWindows[entity].m_SizeX * 0.1f, UIWindows[entity].m_SizeY * 0.25f);
			UIWindows[entity].m_TaxText.setFillColor(sf::Color::Red);
			UIWindows[entity].m_RegionNameText.setFillColor(sf::Color::Red);
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
				m_Window->draw(UIWindows[entity].m_TaxText);
				m_Window->draw(UIWindows[entity].m_RegionNameText);
			}
		}
	}

	void SetOptions(Transform* transform, UIWindow* UIWindow)
	{
		if (InputHandler::GetRightMouseClicked() == true && InputHandler::GetPlayerSelected() == false && !UIWindow->m_Visible)
		{
			unsigned int random = std::rand() % (16 + 1);
			UIWindow->m_RegionTax = MapInfo::GetRegionTax(random);
			UIWindow->m_RegionName = MapInfo::GetRegionName(random);
		}
	}

	void OpenWindow(Transform* transform, UIWindow* UIWindow)
	{
		if (InputHandler::GetRightMouseReleased() == true && InputHandler::GetPlayerSelected() == false)
		{
			UIWindow->m_Visible = !UIWindow->m_Visible;
		}
	}
};