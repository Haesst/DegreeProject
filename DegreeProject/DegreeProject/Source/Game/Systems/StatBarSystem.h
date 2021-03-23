#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Window.h"
#include "Game/Components/StatBar.h"
#include "Game/Systems/CharacterSystem.h"

struct StatBarSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	sf::RenderWindow* m_Window = nullptr;
	EntityID m_PlayerID = 0;

	StatBarSystem()
	{
		AddComponentSignature<StatBar>();
		m_EntityManager = &EntityManager::Get();
		m_Window = Window::GetWindow();
	}

	~StatBarSystem(){}

	virtual void Start() override
	{
#pragma warning(push)
#pragma warning(disable: 26815)
		CharacterSystem* characterSystem = (CharacterSystem*)m_EntityManager->GetSystem<CharacterSystem>().get();
#pragma warning(pop)
		m_PlayerID = characterSystem->GetPlayerID();
	}

	virtual void Update() override
	{
		StatBar* statBars = m_EntityManager->GetComponentArray<StatBar>();

		for (auto entity : m_Entities)
		{
			UpdateStats(statBars[entity]);
			statBars[entity].m_Shape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_Window->getSize().x - (int)(statBars[entity].m_SizeX - statBars[entity].m_OutlineThickness), (int)statBars[entity].m_OutlineThickness)));
			statBars[entity].m_Shape.setFillColor(statBars[entity].m_FillColor);
			statBars[entity].m_Shape.setOutlineColor(statBars[entity].m_OwnerColor);
			statBars[entity].m_Shape.setOutlineThickness(statBars[entity].m_OutlineThickness);
			statBars[entity].m_Shape.setSize(sf::Vector2f(statBars[entity].m_SizeX, statBars[entity].m_SizeY));
			statBars[entity].m_CurrentGoldText.setFont(statBars[entity].m_Font);
			statBars[entity].m_CurrentGoldText.setCharacterSize(statBars[entity].m_CharacterSize);
			statBars[entity].m_CurrentGoldText.setStyle(statBars[entity].m_Style);
			statBars[entity].m_CurrentGoldText.setString(statBars[entity].m_CurrentGold);
			statBars[entity].m_CurrentGoldText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_Window->getSize().x - (int)(statBars[entity].m_SizeX + statBars[entity].m_OutlineThickness - statBars[entity].m_SizeX * 0.2f), (int)(statBars[entity].m_SizeY * 0.2f))));
			statBars[entity].m_CurrentGoldText.setFillColor(statBars[entity].m_OwnerColor);
			statBars[entity].m_CurrentIncomeText.setFont(statBars[entity].m_Font);
			statBars[entity].m_CurrentIncomeText.setCharacterSize((int)(statBars[entity].m_CharacterSize * 0.5f));
			statBars[entity].m_CurrentIncomeText.setStyle(statBars[entity].m_Style);
			statBars[entity].m_CurrentIncomeText.setString(statBars[entity].m_CurrentIncome);
			statBars[entity].m_CurrentIncomeText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_Window->getSize().x - (int)(statBars[entity].m_SizeX + statBars[entity].m_OutlineThickness - statBars[entity].m_SizeX * 0.2f), (int)(statBars[entity].m_SizeY * 0.8f))));
			statBars[entity].m_CurrentIncomeText.setFillColor(statBars[entity].m_OwnerColor);
			statBars[entity].m_MaxArmyText.setFont(statBars[entity].m_Font);
			statBars[entity].m_MaxArmyText.setCharacterSize(statBars[entity].m_CharacterSize);
			statBars[entity].m_MaxArmyText.setStyle(statBars[entity].m_Style);
			statBars[entity].m_MaxArmyText.setString(statBars[entity].m_MaxArmy);
			statBars[entity].m_MaxArmyText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_Window->getSize().x - (int)(statBars[entity].m_SizeX + statBars[entity].m_OutlineThickness - statBars[entity].m_SizeX * 0.6f), (int)(statBars[entity].m_SizeY * 0.2f))));
			statBars[entity].m_MaxArmyText.setFillColor(statBars[entity].m_OwnerColor);
			statBars[entity].m_CurrentMaxArmyText.setFont(statBars[entity].m_Font);
			statBars[entity].m_CurrentMaxArmyText.setCharacterSize((int)(statBars[entity].m_CharacterSize * 0.5f));
			statBars[entity].m_CurrentMaxArmyText.setStyle(statBars[entity].m_Style);
			statBars[entity].m_CurrentMaxArmyText.setString(statBars[entity].m_CurrentMaxArmy);
			statBars[entity].m_CurrentMaxArmyText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_Window->getSize().x - (int)(statBars[entity].m_SizeX + statBars[entity].m_OutlineThickness - statBars[entity].m_SizeX * 0.625f), (int)(statBars[entity].m_SizeY * 0.8f))));
			statBars[entity].m_CurrentMaxArmyText.setFillColor(statBars[entity].m_OwnerColor);
		}
	}

	virtual void Render() override
	{
		StatBar* statBars = m_EntityManager->GetComponentArray<StatBar>();

		for (auto entity : m_Entities)
		{
			m_Window->draw(statBars[entity].m_Shape);
			m_Window->draw(statBars[entity].m_CurrentGoldText);
			m_Window->draw(statBars[entity].m_CurrentIncomeText);
			m_Window->draw(statBars[entity].m_MaxArmyText);
			m_Window->draw(statBars[entity].m_CurrentMaxArmyText);
		}
	}

	void UpdateStats(StatBar& statBar)
	{
		CharacterComponent* characters = m_EntityManager->GetComponentArray<CharacterComponent>();
		statBar.m_CurrentGold = std::to_string(characters[m_PlayerID].m_CurrentGold);
		int income = characters[m_PlayerID].m_Income;
		if (income >= 0)
		{
			std::string sign = "+";
			statBar.m_CurrentIncome = sign + std::to_string(characters[m_PlayerID].m_Income);
		}
		else
		{
			statBar.m_CurrentIncome = std::to_string(characters[m_PlayerID].m_Income);
		}
		statBar.m_MaxArmy = std::to_string(characters[m_PlayerID].m_MaxArmySize);
		statBar.m_CurrentMaxArmy = std::to_string(characters[m_PlayerID].m_CurrentMaxArmySize);
		statBar.m_OwnerColor = characters[m_PlayerID].m_RegionColor;
	}
};