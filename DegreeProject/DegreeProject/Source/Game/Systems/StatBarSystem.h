#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Time.h"
#include "Engine/Window.h"
#include "ECS/Components/Transform.h"
#include "Game/Components/StatBar.h"
#include "Game/Components/UISpriteRenderer.h"
#include "Game/Systems/CharacterSystem.h"

struct StatBarSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	sf::RenderWindow* m_Window = nullptr;
	EntityID m_PlayerID = 0;

	// Constructor, Runs when the system is initialized
	// Do any kind of init here but remember to register
	// the systems component signature. IE: add every component
	// that the system needs or should look for
	StatBarSystem()
	{
		AddComponentSignature<StatBar>();
		m_EntityManager = &EntityManager::Get();
		m_Window = Window::GetWindow();
	}

	~StatBarSystem()
	{
	}

	virtual void Start() override
	{
#pragma warning(push)
#pragma warning(disable: 26815)
		CharacterSystem* characterSystem = (CharacterSystem*)m_EntityManager->GetSystem<CharacterSystem>().get();
#pragma warning(pop)
		m_PlayerID = characterSystem->GetPlayerID();
	}

	// Update gets called every frame and loops through every entity that has the signature that
	// the system has registered and do the necessary update
	virtual void Update() override
	{
		StatBar* statBars = m_EntityManager->GetComponentArray<StatBar>();

		for (auto entity : m_Entities)
		{
			UpdateStats(statBars[entity]);
			statBars[entity].m_Shape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_Window->getSize().x - statBars[entity].m_SizeX - statBars[entity].m_OutlineThickness, statBars[entity].m_OutlineThickness)));
			statBars[entity].m_Shape.setFillColor(statBars[entity].m_FillColor);
			statBars[entity].m_Shape.setOutlineColor(statBars[entity].m_OwnerColor);
			statBars[entity].m_Shape.setOutlineThickness(statBars[entity].m_OutlineThickness);
			statBars[entity].m_Shape.setSize(sf::Vector2f(statBars[entity].m_SizeX, statBars[entity].m_SizeY));
			statBars[entity].m_CurrentGoldText.setFont(statBars[entity].m_Font);
			statBars[entity].m_CurrentGoldText.setCharacterSize(statBars[entity].m_CharacterSize);
			statBars[entity].m_CurrentGoldText.setStyle(statBars[entity].m_Style);
			statBars[entity].m_CurrentGoldText.setString(statBars[entity].m_CurrentGold);
			statBars[entity].m_CurrentGoldText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_Window->getSize().x - statBars[entity].m_SizeX - statBars[entity].m_OutlineThickness + statBars[entity].m_SizeX * 0.3f, statBars[entity].m_SizeY * 0.25f)));
			statBars[entity].m_CurrentGoldText.setFillColor(statBars[entity].m_OwnerColor);
			statBars[entity].m_MaxArmyText.setFont(statBars[entity].m_Font);
			statBars[entity].m_MaxArmyText.setCharacterSize(statBars[entity].m_CharacterSize);
			statBars[entity].m_MaxArmyText.setStyle(statBars[entity].m_Style);
			statBars[entity].m_MaxArmyText.setString(statBars[entity].m_CurrentMaxArmy);
			statBars[entity].m_MaxArmyText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_Window->getSize().x - statBars[entity].m_SizeX - statBars[entity].m_OutlineThickness + statBars[entity].m_SizeX * 0.65f, statBars[entity].m_SizeY * 0.25f)));
			statBars[entity].m_MaxArmyText.setFillColor(statBars[entity].m_OwnerColor);
		}
	}

	virtual void Render() override
	{
		StatBar* statBars = m_EntityManager->GetComponentArray<StatBar>();

		for (auto entity : m_Entities)
		{
			m_Window->draw(statBars[entity].m_Shape);
			m_Window->draw(statBars[entity].m_CurrentGoldText);
			m_Window->draw(statBars[entity].m_MaxArmyText);
		}
	}

	void UpdateStats(StatBar& statBar)
	{
		CharacterComponent* characters = m_EntityManager->GetComponentArray<CharacterComponent>();
		statBar.m_CurrentGold = std::to_string(characters[m_PlayerID].m_CurrentGold);
		statBar.m_CurrentMaxArmy = std::to_string(characters[m_PlayerID].m_MaxArmySize);
		statBar.m_OwnerColor = characters[m_PlayerID].m_RegionColor;
	}
};