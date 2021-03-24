#pragma once

#include <SFML/Graphics.hpp>
#include "ECS/Component.h"

struct RegionWindow : public Component
{
	sf::RectangleShape m_WindowShape;
	sf::RectangleShape m_BuildingSlotShape;
	sf::RectangleShape m_BuildingSlotShape2;
	sf::RectangleShape m_BuildingSlotShape3;
	sf::RectangleShape m_BuildingSlotShape4;
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Red;
	sf::Color m_BuildingSlotColor = sf::Color::Transparent;
	sf::Color m_BuildingSlotColor2 = sf::Color::Transparent;
	sf::Color m_BuildingSlotColor3 = sf::Color::Transparent;
	sf::Color m_BuildingSlotColor4 = sf::Color::Transparent;
	float m_SizeX = 600.0f;
	float m_SizeY = 600.0f;
	float m_OutlineThickness = 10.0f;
	sf::Text m_RegionNameText;
	sf::Text m_RegionTaxText;
	sf::Text m_KingdomNameText;
	sf::Font m_Font;
	std::string m_RegionName = "";
	std::string m_RegionTax = "";
	std::string m_KingdomName = "";
	int m_CharacterSize = 50;
	unsigned int m_RegionPortraitID = 0;
	unsigned int m_BuildSlotID = 0;
	unsigned int m_BuildSlotID2 = 0;
	unsigned int m_BuildSlotID3 = 0;
	unsigned int m_BuildSlotID4 = 0;
	unsigned int m_CharacterWindowID = 0;
	unsigned int m_BottomPortraitID = 0;
	bool m_Visible = false;
	bool m_Open = false;
	unsigned int m_CurrentRegionID = 0;
#pragma warning(push)
#pragma warning(disable: 26812)
	sf::Text::Style m_Style = sf::Text::Regular;
#pragma warning(pop)

	RegionWindow() {}

	RegionWindow(sf::Font font, unsigned int regionPortraitID, unsigned int regionBuildSlotID, unsigned int regionBuildSlotID2, unsigned int regionBuildSlotID3, unsigned int regionBuildSlotID4, unsigned int characterWindowID, unsigned int bottomPortraitID)
	{
		m_Font = font;
		m_RegionPortraitID = regionPortraitID;
		m_BuildSlotID = regionBuildSlotID;
		m_BuildSlotID2 = regionBuildSlotID2;
		m_BuildSlotID3 = regionBuildSlotID3;
		m_BuildSlotID4 = regionBuildSlotID4;
		m_CharacterWindowID = characterWindowID;
		m_BottomPortraitID = bottomPortraitID;
	}
};