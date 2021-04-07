#pragma once

#include <SFML/Graphics.hpp>
#include "Engine/Window.h"
#include "Game/Data/Character.h"
#include "Game/Systems/CharacterManager.h"
#include <iomanip>

class StatBar
{
public:
	sf::RectangleShape m_WindowShape;
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
	int m_CharacterSize = 30;
#pragma warning(push)
#pragma warning(disable: 26812)
	sf::Text::Style m_Style = sf::Text::Regular;
#pragma warning(pop)
	UIID m_OwnedUIWindow = INVALID_UI_ID;
	sf::RenderWindow* m_Window = nullptr;
	const std::string m_PositiveSign = "+";
	sf::Texture m_CoinTexture;
	sf::Sprite m_CoinSprite;
	sf::Texture m_ArmyTexture;
	sf::Sprite m_ArmySprite;
	const static int m_SpriteSize = 32;
	sf::Vector2f m_CoinPosition = sf::Vector2f();
	sf::Vector2f m_ArmyPosition = sf::Vector2f();

	StatBar(UIID id, sf::Font font, Vector2D, Vector2D size)
	{
		m_OwnedUIWindow = id;
		m_Font = font;
		m_SizeX = size.x;
		m_SizeY = size.y;
	}

	void start()
	{
		Time::m_GameDate.subscribeToMonthChange(std::bind(&StatBar::onMonthChange, this));

		m_Window = Window::getWindow();

		m_CoinTexture = AssetHandler::get().loadImageFromFile("Assets/Graphics/Coins.png");
		m_ArmyTexture = AssetHandler::get().loadImageFromFile("Assets/Graphics/soldier unit.png");

		m_CoinPosition = sf::Vector2f(m_SizeX * 0.1f + m_Window->getSize().x - m_SizeX - m_OutlineThickness, m_SizeY * 0.3f);
		m_ArmyPosition = sf::Vector2f(m_SizeX * 0.5f + m_Window->getSize().x - m_SizeX - m_OutlineThickness, m_SizeY * 0.3f);

		updateStats();

		m_WindowShape.setFillColor(m_FillColor);
		m_WindowShape.setOutlineColor(m_OwnerColor);
		m_WindowShape.setOutlineThickness(m_OutlineThickness);
		m_WindowShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));

		m_CurrentGoldText.setFont(m_Font);
		m_CurrentGoldText.setCharacterSize(m_CharacterSize);
		m_CurrentGoldText.setStyle(m_Style);
		m_CurrentGoldText.setFillColor(m_OwnerColor);

		m_CurrentIncomeText.setFont(m_Font);
		m_CurrentIncomeText.setCharacterSize((int)(m_CharacterSize * 0.5f));
		m_CurrentIncomeText.setStyle(m_Style);
		m_CurrentIncomeText.setFillColor(m_OwnerColor);

		m_MaxArmyText.setFont(m_Font);
		m_MaxArmyText.setCharacterSize(m_CharacterSize);
		m_MaxArmyText.setStyle(m_Style);
		m_MaxArmyText.setFillColor(m_OwnerColor);

		m_CurrentMaxArmyText.setFont(m_Font);
		m_CurrentMaxArmyText.setCharacterSize((int)(m_CharacterSize * 0.5f));
		m_CurrentMaxArmyText.setStyle(m_Style);
		m_CurrentMaxArmyText.setFillColor(m_OwnerColor);
	}

	void update()
	{
		int positionX = m_Window->getSize().x - (int)(m_SizeX + m_OutlineThickness);
		int positionY = (int)m_OutlineThickness;

		m_WindowShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX, positionY)));

		m_CurrentGoldText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_SizeX * 0.2f), positionY)));

		m_CurrentIncomeText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_SizeX * 0.2f), positionY * 4)));

		m_MaxArmyText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_SizeX * 0.6f), positionY)));

		m_CurrentMaxArmyText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_SizeX * 0.625f), positionY * 4)));
	}

	void render()
	{
		m_Window->draw(m_WindowShape);
		updateSprite(m_CoinSprite, m_CoinTexture, m_CoinPosition);
		updateSprite(m_ArmySprite, m_ArmyTexture, m_ArmyPosition);
		m_Window->draw(m_CurrentGoldText);
		m_Window->draw(m_CurrentIncomeText);
		m_Window->draw(m_MaxArmyText);
		m_Window->draw(m_CurrentMaxArmyText);
	}

	void onMonthChange()
	{
		updateStats();
	}

	void updateStats()
	{
		Character& character = CharacterManager::get()->getPlayerCharacter();

		std::stringstream stream;
		stream << std::fixed << std::setprecision(1) << character.m_CurrentGold;
		m_CurrentGoldText.setString(stream.str());
		stream.str(std::string());
		stream.clear();

		float income = character.m_Income;
		if (income >= 0.0f)
		{
			stream << m_PositiveSign << std::fixed << std::setprecision(1) << character.m_Income;
			m_CurrentIncomeText.setString(stream.str());
		}
		else
		{
			stream << std::fixed << std::setprecision(1) << character.m_Income;
			m_CurrentIncomeText.setString(stream.str());
		}

		m_MaxArmyText.setString(std::to_string(character.m_MaxArmySize));
		m_CurrentMaxArmyText.setString(std::to_string(character.m_RaisedArmySize));
		m_OwnerColor = character.m_RegionColor;
	}

	void updateSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, int spriteSize = m_SpriteSize)
	{
		sprite.setTexture(texture, true);
		sprite.setPosition(Window::getWindow()->mapPixelToCoords(sf::Vector2i((int)position.x, (int)position.y)));

		sf::FloatRect localSize = sprite.getLocalBounds();

		sprite.setScale(spriteSize / localSize.width, spriteSize / localSize.height);

		m_Window->draw(sprite);
	}
};