#pragma once

#include <SFML/Graphics.hpp>
#include "ECS/Component.h"

const static unsigned int NUMBER_OF_SPEEDS = 5;
const static unsigned int NUMBER_OF_BUTTONS = 3;

struct DateBar : public Component
{
	sf::RectangleShape m_Shape;
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
	std::string m_Date = "";
	int m_CharacterSize = 30;
#pragma warning(push)
#pragma warning(disable: 26812)
	sf::Text::Style m_Style = sf::Text::Regular;
#pragma warning(pop)

	DateBar() {}

	DateBar(sf::Font font)
	{
		m_Font = font;
		for (unsigned int index = 0; index < m_NumberOfButtons; index++)
		{
			m_ButtonShapes[index] = sf::RectangleShape();
		}
		for (unsigned int index = 0; index < m_NumberOfSpeeds; index++)
		{
			m_SpeedShapes[index] = sf::RectangleShape();
		}
	}
};