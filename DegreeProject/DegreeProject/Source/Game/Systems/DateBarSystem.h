#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Window.h"
#include "Engine/Time.h"
#include "Game/Components/DateBar.h"

struct DateBarSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	sf::RenderWindow* m_Window = nullptr;

	DateBarSystem()
	{
		AddComponentSignature<DateBar>();
		m_EntityManager = &EntityManager::Get();
		m_Window = Window::GetWindow();
	}

	~DateBarSystem() {}

	virtual void Start() override
	{
#pragma warning(push)
#pragma warning(disable: 26815)
		CharacterSystem* characterSystem = (CharacterSystem*)m_EntityManager->GetSystem<CharacterSystem>().get();
#pragma warning(pop)

		CharacterComponent& character = m_EntityManager->GetComponent<CharacterComponent>(characterSystem->GetPlayerID());

		DateBar* dateBars = m_EntityManager->GetComponentArray<DateBar>();
		for (auto entity : m_Entities)
		{
			UpdateStats(dateBars[entity]);

			int positionX = m_Window->getSize().x - (int)(dateBars[entity].m_SizeX + dateBars[entity].m_OutlineThickness);
			int positionY = m_Window->getSize().y - (int)(dateBars[entity].m_SizeY + dateBars[entity].m_OutlineThickness);

			dateBars[entity].m_OwnerColor = character.m_RegionColor;

			dateBars[entity].m_Shape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX, positionY)));
			dateBars[entity].m_Shape.setFillColor(dateBars[entity].m_FillColor);
			dateBars[entity].m_Shape.setOutlineColor(dateBars[entity].m_OwnerColor);
			dateBars[entity].m_Shape.setOutlineThickness(dateBars[entity].m_OutlineThickness);
			dateBars[entity].m_Shape.setSize(sf::Vector2f(dateBars[entity].m_SizeX, dateBars[entity].m_SizeY));

			dateBars[entity].m_DateText.setFont(dateBars[entity].m_Font);
			dateBars[entity].m_DateText.setCharacterSize(dateBars[entity].m_CharacterSize);
			dateBars[entity].m_DateText.setStyle(dateBars[entity].m_Style);
			dateBars[entity].m_DateText.setString(dateBars[entity].m_Date);
			dateBars[entity].m_DateText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(dateBars[entity].m_SizeX * 0.2f), positionY)));
			dateBars[entity].m_DateText.setFillColor(dateBars[entity].m_OwnerColor);
		}
	}

	virtual void Update() override
	{
		DateBar* dateBars = m_EntityManager->GetComponentArray<DateBar>();

		for (auto entity : m_Entities)
		{
			UpdateStats(dateBars[entity]);

			int positionX = m_Window->getSize().x - (int)(dateBars[entity].m_SizeX + dateBars[entity].m_OutlineThickness);
			int positionY = m_Window->getSize().y - (int)(dateBars[entity].m_SizeY + dateBars[entity].m_OutlineThickness);

			dateBars[entity].m_Shape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX, positionY)));
			
			dateBars[entity].m_DateText.setString(dateBars[entity].m_Date);
			dateBars[entity].m_DateText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(dateBars[entity].m_SizeX * 0.2f), positionY)));
		}
	}
	
	virtual void Render() override
	{
		DateBar* dateBars = m_EntityManager->GetComponentArray<DateBar>();

		for (auto entity : m_Entities)
		{
			m_Window->draw(dateBars[entity].m_Shape);
			m_Window->draw(dateBars[entity].m_DateText);
		}
	}

	void UpdateStats(DateBar& dateBar)
	{
		dateBar.m_Date = Time::m_GameDate.GetDateString();
	}
};