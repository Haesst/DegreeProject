#pragma once

#include <SFML/Graphics.hpp>
#include "ECS/Component.h"

struct UIText : public Component
{
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OutlineColor = sf::Color::Black;
	float m_OutlineThickness = 1.0f;
	float m_PositionX = 960.0f;
	float m_PositionY = 540.0f;
	sf::Text m_CountryNameText;
	sf::Font m_Font;
	std::string m_CountryName = "";
	unsigned int m_CharacterSize = 20;
#pragma warning(push)
#pragma warning(disable: 26812)
	sf::Text::Style m_Style = sf::Text::Regular;
#pragma warning(pop)
	float m_Rotation = 0.0f;
	std::vector<unsigned int> m_OwnedRegionIDs;
	bool m_Conquered = false;

	UIText(){}

	UIText(sf::Font font, std::string countryName, std::vector<unsigned int> ownedRegions)
	{
		m_Font = font;
		m_CountryName = countryName;
		m_OwnedRegionIDs = ownedRegions;
	}
};