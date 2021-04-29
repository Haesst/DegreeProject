#pragma once

#include <SFML/Graphics.hpp>
#include "Game/Data/UIType.h"
#include "Game/Data/Types.h"

class UIText
{
public:

	UIText(UIID id, sf::Font font, std::string countryName, std::vector<unsigned int> ownedRegions);

	void start();
	void update();
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
	unsigned int m_MaxCharacterSize = 100;
	const float m_MinOutlineThickness = 0.1f;
	const float m_MaxOutlineThickness = 1.0f;
	float m_Rotation = 0.0f;
	std::vector<unsigned int> m_OwnedRegionIDs;
	bool m_Conquered = false;
	sf::RenderWindow* m_Window;
	UIID m_OwnedUIElement = INVALID_UI_ID;
	bool m_Hidden = false;
	float m_HiddenDistance = 0.5f;
};