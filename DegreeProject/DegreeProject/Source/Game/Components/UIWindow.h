#pragma once

#include <SFML/Graphics.hpp>
#include "ECS/Component.h"
#include "ECS/EntityManager.h"
#include "Engine/Vector2D.h"

struct UIWindow : public Component
{
	sf::RectangleShape m_Shape;
	sf::Color m_FillColor = sf::Color::Cyan;
	sf::Color m_OutlineColor = sf::Color::Red;
	float m_SizeX = 480.0f;
	float m_SizeY = 1060.0f;
	bool m_Visible = false;
	sf::Text m_TaxText;
	sf::Text m_RegionNameText;
	sf::Font m_Font;
	int m_RegionTax = 0;
	std::string m_RegionName = "";
	int m_CharacterSize = 50;
	sf::Text::Style m_Style = sf::Text::Regular;

	UIWindow()
	{
		//m_Font.loadFromFile("Assets/Fonts/TestFont.ttf");
	}

	UIWindow(sf::Font font)
	{
		m_Font = font;
	}
};