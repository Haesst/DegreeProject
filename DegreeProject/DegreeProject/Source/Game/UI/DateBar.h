#pragma once

#include <SFML/Graphics.hpp>
#include "Game/Data/Types.h"
#include "Engine/Vector2D.h"

const static unsigned int NUMBER_OF_SPEEDS = 5;
const static unsigned int NUMBER_OF_BUTTONS = 3;

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

	void setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f& size, Vector2D& position);

	sf::RectangleShape m_WindowShape;
	sf::RectangleShape m_DecreaseSpeedShape;
	sf::RectangleShape m_IncreaseSpeedVerticalShape;
	sf::RectangleShape m_IncreaseSpeedHorizontalShape;
	sf::RectangleShape m_PauseLeftShape;
	sf::RectangleShape m_PauseRightShape;
	sf::RectangleShape m_ButtonShapes[NUMBER_OF_BUTTONS];
	sf::RectangleShape m_SpeedShapes[NUMBER_OF_SPEEDS];
	unsigned int m_NumberOfSpeeds = NUMBER_OF_SPEEDS;
	unsigned int m_NumberOfButtons = NUMBER_OF_BUTTONS;
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Red;
	sf::Color m_SpeedColor = sf::Color::Green;
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
};