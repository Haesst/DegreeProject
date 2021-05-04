#pragma once

#include <SFML/Graphics.hpp>
#include "Game/Data/Types.h"
#include "Engine/Vector2D.h"

class DateBar
{
public:

	DateBar(UIID id, sf::Font font, Vector2D, Vector2D size);
	void start();
	void update();
	void render();
	void onDayChange();
	void updateStats();
	void clickButton();
	void updateOwnerColor(sf::Color& newColor);
	void updateSpeedShapes();
	void setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position);
	void setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position);
	void setSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, unsigned int spriteSize = m_SpriteSize);

	sf::RectangleShape m_WindowShape;
	std::vector<sf::Sprite> m_ButtonSprites;
	std::vector<sf::Texture> m_ButtonTextures;
	std::vector<sf::RectangleShape> m_ButtonShapes;
	std::vector<sf::RectangleShape> m_SpeedShapes;
	const unsigned int m_NumberOfSpeeds = 5;
	const unsigned int m_NumberOfButtons = 3;
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Red;
	sf::Color m_PauseSpeedColor = sf::Color::Red;
	sf::Color m_PlaySpeedColor = sf::Color::Green;
	sf::Color m_TransparentColor = sf::Color::Transparent;
	float m_SizeX = 600.0f;
	float m_SizeY = 50.0f;
	float m_OutlineThickness = 5.0f;
	float m_ButtonThickness = 10.0f;
	sf::Text m_DateText;
	sf::Font m_Font;
	int m_CharacterSize = 30;
	sf::RenderWindow* m_Window = nullptr;
	unsigned int m_CurrentSpeedLevel = 3;
	int m_DaySubscriptionHandle = 0;
	UIID m_OwnedUIWindow = INVALID_UI_ID;
	const static int m_SpriteSize = 32;
	bool m_Paused = false;
};