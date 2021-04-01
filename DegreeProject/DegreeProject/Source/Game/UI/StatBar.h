#pragma once

#include <SFML/Graphics.hpp>
#include "Engine/Window.h"
#include "Game/Data/Character.h"
#include "Engine/CharacterManager.h"

struct StatBar
{
	sf::RectangleShape m_Shape;
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Red;
	float m_SizeX = 400.0f;
	float m_SizeY = 50.0f;
	float m_OutlineThickness = 10.0f;
	sf::Text m_CurrentGoldText;
	sf::Text m_CurrentIncomeText;
	sf::Text m_MaxArmyText;
	sf::Text m_CurrentMaxArmyText;
	sf::Font m_Font;
	std::string m_CurrentGold = "";
	std::string m_CurrentIncome = "";
	std::string m_MaxArmy = "";
	std::string m_CurrentMaxArmy = "";
	int m_CharacterSize = 30;
#pragma warning(push)
#pragma warning(disable: 26812)
	sf::Text::Style m_Style = sf::Text::Regular;
#pragma warning(pop)
	UIWindowID m_OwnedUIWindow = INVALID_UI_ID;
	sf::RenderWindow* m_Window = nullptr;
	const std::string m_PositiveSign = "+";

	StatBar(UIWindowID id, sf::Font font)
	{
		m_OwnedUIWindow = id;
		m_Font = font;
		m_Window = Window::getWindow();
	}

	void start()
	{
		updateStats();

		int positionX = m_Window->getSize().x - (int)(m_SizeX + m_OutlineThickness);
		int positionY = (int)m_OutlineThickness;

		m_Shape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX, positionY)));
		m_Shape.setFillColor(m_FillColor);
		m_Shape.setOutlineColor(m_OwnerColor);
		m_Shape.setOutlineThickness(m_OutlineThickness);
		m_Shape.setSize(sf::Vector2f(m_SizeX, m_SizeY));

		m_CurrentGoldText.setFont(m_Font);
		m_CurrentGoldText.setCharacterSize(m_CharacterSize);
		m_CurrentGoldText.setStyle(m_Style);
		m_CurrentGoldText.setString(m_CurrentGold);
		m_CurrentGoldText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_SizeX * 0.2f), positionY)));
		m_CurrentGoldText.setFillColor(m_OwnerColor);

		m_CurrentIncomeText.setFont(m_Font);
		m_CurrentIncomeText.setCharacterSize((int)(m_CharacterSize * 0.5f));
		m_CurrentIncomeText.setStyle(m_Style);
		m_CurrentIncomeText.setString(m_CurrentIncome);
		m_CurrentIncomeText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_SizeX * 0.2f), positionY * 4)));
		m_CurrentIncomeText.setFillColor(m_OwnerColor);

		m_MaxArmyText.setFont(m_Font);
		m_MaxArmyText.setCharacterSize(m_CharacterSize);
		m_MaxArmyText.setStyle(m_Style);
		m_MaxArmyText.setString(m_MaxArmy);
		m_MaxArmyText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_SizeX * 0.6f), positionY)));
		m_MaxArmyText.setFillColor(m_OwnerColor);

		m_CurrentMaxArmyText.setFont(m_Font);
		m_CurrentMaxArmyText.setCharacterSize((int)(m_CharacterSize * 0.5f));
		m_CurrentMaxArmyText.setStyle(m_Style);
		m_CurrentMaxArmyText.setString(m_CurrentMaxArmy);
		m_CurrentMaxArmyText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_SizeX * 0.625f), positionY * 4)));
		m_CurrentMaxArmyText.setFillColor(m_OwnerColor);
	}

	void update()
	{
		updateStats();

		int positionX = m_Window->getSize().x - (int)(m_SizeX + m_OutlineThickness);
		int positionY = (int)m_OutlineThickness;

		m_Shape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX, positionY)));

		m_CurrentGoldText.setString(m_CurrentGold);
		m_CurrentGoldText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_SizeX * 0.2f), positionY)));

		m_CurrentIncomeText.setString(m_CurrentIncome);
		m_CurrentIncomeText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_SizeX * 0.2f), positionY * 4)));

		m_MaxArmyText.setString(m_MaxArmy);
		m_MaxArmyText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_SizeX * 0.6f), positionY)));

		m_CurrentMaxArmyText.setString(m_CurrentMaxArmy);
		m_CurrentMaxArmyText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_SizeX * 0.625f), positionY * 4)));

	}

	void render()
	{
		m_Window->draw(m_Shape);
		m_Window->draw(m_CurrentGoldText);
		m_Window->draw(m_CurrentIncomeText);
		m_Window->draw(m_MaxArmyText);
		m_Window->draw(m_CurrentMaxArmyText);
	}

	void updateStats()
	{
		Character& character = CharacterManager::get()->getPlayerCharacter();
		m_CurrentGold = std::to_string(character.m_CurrentGold);
		float income = character.m_Income;
		if (income >= 0.0f)
		{
			m_CurrentIncome = m_PositiveSign + std::to_string(character.m_Income);
		}
		else
		{
			m_CurrentIncome = std::to_string(character.m_Income);
		}
		m_MaxArmy = std::to_string(character.m_MaxArmySize);
		m_CurrentMaxArmy = std::to_string(character.m_CurrentMaxArmySize);
		m_OwnerColor = character.m_RegionColor;
	}
};