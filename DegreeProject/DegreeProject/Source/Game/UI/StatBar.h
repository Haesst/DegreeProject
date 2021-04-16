#pragma once

#include <SFML/Graphics.hpp>
#include "Game/Data/Types.h"
#include "Game/Data/Titles.h"
#include "Engine/Vector2D.h"

class StatBar
{
public:

	StatBar(UIID id, sf::Font font, Vector2D, Vector2D size);

	void start();
	void update();
	void render();
	void onDayChange();
	void updateStats();
	void updateSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, int spriteSize = m_SpriteSize);

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
	int m_DaySubscriptionHandle = 0;
};