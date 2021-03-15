#pragma once

#include <SFML/Graphics.hpp>
#include "ECS/Component.h"

struct UIWindow : public Component
{
	sf::RectangleShape m_Shape;
	sf::Color m_FillColor = sf::Color::Cyan;
	sf::Color m_OutlineColor = sf::Color::Red;
	float m_SizeX = 480.0f;
	float m_SizeY = 1060.0f;
	bool m_Visible = false;
	sf::Text m_OwnerNameText;
	sf::Text m_TaxText;
	sf::Text m_RegionNameText;
	sf::Font m_Font;
	int m_RegionTax = 0;
	std::string m_RegionName = "";
	std::string m_OwnerName = "";
	int m_CharacterSize = 50;
#pragma warning(push)
#pragma warning(disable: 26812)
	sf::Text::Style m_Style = sf::Text::Regular;
#pragma warning(pop)
	bool m_Open = false;

	UIWindow(){}

	UIWindow(sf::Font font)
	{
		m_Font = font;
	}
};