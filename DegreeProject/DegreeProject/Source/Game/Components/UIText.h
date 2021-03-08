#pragma once

#include <SFML/Graphics.hpp>
#include "ECS/Component.h"

struct UIText : public Component
{
	sf::Color m_FillColor = sf::Color::White;
	sf::Color m_OutlineColor = sf::Color::Black;
	float m_OutlineThickness = 1.0f;
	float m_PositionX = 960.0f;
	float m_PositionY = 540.0f;
	sf::Text m_CountryNameText;
	sf::Font m_Font;
	std::string m_CountryName = "";
	int m_CharacterSize = 25;
	sf::Text::Style m_Style = sf::Text::Regular;
	float m_Rotation = 0.0f;

	UIText(){}

	UIText(sf::Font font, std::string countryName, float positionX, float positionY, unsigned int size = 25, float rotation = 0.0f, sf::Color fillColor = sf::Color::White, sf::Color outlineColor = sf::Color::Black, float outlineThickness = 1.0f, sf::Text::Style textStyle = sf::Text::Regular)
	{
		m_Font = font;
		m_CountryName = countryName;
		m_PositionX = positionX;
		m_PositionY = positionY;
		m_CharacterSize = size;
		m_Rotation = rotation;
		m_FillColor = fillColor;
		m_OutlineColor = outlineColor;
		m_OutlineThickness = outlineThickness;
		m_Style = textStyle;
	}
};