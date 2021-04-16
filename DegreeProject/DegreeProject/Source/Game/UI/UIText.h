#pragma once

#include <SFML/Graphics.hpp>
#include "Game/Data/UIType.h"
#include "Game/Data/Types.h"

class UIText
{
public:

	UIText(UIID id, sf::Font font, std::string countryName, std::vector<unsigned int> ownedRegions);

	void start();
	void render();
	void adjustText();
	void conquerRegion(unsigned int regionID);
	void loseRegion(unsigned regionID);

	sf::Color m_FillColor = sf::Color(255, 252, 240, 150);
	sf::Color m_OutlineColor = sf::Color(0, 0, 0, 150);
	float m_OutlineThickness = 1.0f;
	float m_PositionX = 960.0f;
	float m_PositionY = 540.0f;
	sf::Text m_CountryNameText;
	sf::Font m_Font;
	std::string m_CountryName = "";
	unsigned int m_CharacterSize = 20;
	unsigned int m_MinCharacterSize = 30;
#pragma warning(push)
#pragma warning(disable: 26812)
	sf::Text::Style m_Style = sf::Text::Regular;
#pragma warning(pop)
	float m_Rotation = 0.0f;
	std::vector<unsigned int> m_OwnedRegionIDs;
	bool m_Conquered = false;
	sf::RenderWindow* m_Window;
	UIID m_OwnedUIElement = INVALID_UI_ID;
};