#pragma once

#include <SFML/Graphics.hpp>
#include "ECS/Component.h"

struct CharacterWindow : public Component
{
	sf::RectangleShape m_Shape;
	sf::RectangleShape m_DeclareWarShape;
	sf::RectangleShape m_MakePeaceShape;
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Red;
	sf::Color m_DeclareWarColor = sf::Color(210, 32, 60);
	sf::Color m_MakePeaceColor = sf::Color(67, 175, 17);
	float m_SizeX = 600.0f;
	float m_SizeY = 1060.0f;
	float m_OutlineThickness = 10.0f;
	bool m_Visible = false;
	sf::Text m_OwnerNameText;
	sf::Text m_TaxText;
	sf::Text m_RegionNameText;
	sf::Text m_OwnerCharacterText;
	sf::Font m_Font;
	sf::Text m_DeclareWarText;
	sf::Text m_MakePeaceText;
	int m_RegionTax = 0;
	unsigned int m_CurrentRegionID = 0;
	std::string m_RegionName = "";
	std::string m_OwnerKingdomName = "";
	std::string m_OwnerCharacterName = "";
	std::string m_OwnerTitle = "";
	std::string m_DeclareWar = "Declare War";
	std::string m_MakePeace = "Make Peace";
	std::vector<std::string> titles;
	int m_CharacterSize = 50;
#pragma warning(push)
#pragma warning(disable: 26812)
	sf::Text::Style m_Style = sf::Text::Regular;
#pragma warning(pop)
	bool m_Open = false;
	unsigned int m_TopPortraitID = 0;
	unsigned int m_RegionWindowID = 0;
	unsigned int m_BottomPortraitID = 0;

	CharacterWindow(){}

	CharacterWindow(sf::Font font, unsigned int topPortraitID, unsigned int regionWindowID, unsigned int bottomPortraitID)
	{
		m_Font = font;
		m_TopPortraitID = topPortraitID;
		m_RegionWindowID = regionWindowID;
		m_BottomPortraitID = bottomPortraitID;
		titles.push_back("Emperor ");
		titles.push_back("King ");
		titles.push_back("Duke ");
		titles.push_back("Count ");
		titles.push_back("Baron ");
	}
};