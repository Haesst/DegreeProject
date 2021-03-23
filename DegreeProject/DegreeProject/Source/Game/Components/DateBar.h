#pragma once

#include <SFML/Graphics.hpp>
#include "ECS/Component.h"

struct DateBar : public Component
{
	sf::RectangleShape m_Shape;
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Red;
	float m_SizeX = 400.0f;
	float m_SizeY = 50.0f;
	float m_OutlineThickness = 10.0f;
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
	}
};