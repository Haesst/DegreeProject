#pragma once

#include <SFML/Graphics.hpp>
#include "Engine/Window.h"
#define _USE_MATH_DEFINES
#include <math.h>

struct UIText
{
	sf::Color m_FillColor = sf::Color(255, 252, 240, 200);
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
	sf::RenderWindow* m_Window;
	UIID m_OwnedUIElement = INVALID_UI_ID;

	UIText(UIID id, sf::Font font, std::string countryName, std::vector<unsigned int> ownedRegions)
	{
		m_OwnedUIElement = id;
		m_Font = font;
		m_CountryName = countryName;
		m_OwnedRegionIDs = ownedRegions;
		m_Window = Window::getWindow();
	}

	void start() 
	{
		m_CountryNameText.setFont(m_Font);
		m_CountryNameText.setStyle(m_Style);
		m_CountryNameText.setString(m_CountryName);
		m_CountryNameText.setFillColor(m_FillColor);
		m_CountryNameText.setOutlineColor(m_OutlineColor);
		m_CountryNameText.setOutlineThickness(m_OutlineThickness);
		adjustText();
	}

	void render() 
	{
		if (!m_Conquered)
		{
			m_Window->draw(m_CountryNameText);
		}	
	}

	void adjustText()
	{
		if (m_OwnedRegionIDs.size() > 0)
		{
			std::vector<SquareData> mapData = Map::get().m_MapSquareData;
			Vector2DInt leftMostPosition = { Map::get().width, 0 };
			Vector2DInt rightMostPosition = { 0, 0 };
			for (SquareData square : mapData)
			{
				for (unsigned int regionId : m_OwnedRegionIDs)
				{
					if (regionId == square.m_RegionID)
					{
						if (square.m_Position.x < leftMostPosition.x)
						{
							leftMostPosition = square.m_Position;
						}
						if (square.m_Position.x >= rightMostPosition.x)
						{
							rightMostPosition = square.m_Position;
						}
					}
				}
			}
			Vector2D leftMostPositionScreen = Map::convertToScreen(leftMostPosition);
			Vector2D diagonal = Map::convertToScreen(rightMostPosition) - leftMostPositionScreen;
			float offsetY = 1.0f;
			m_CharacterSize = (unsigned int)(diagonal.x * 0.1f);
			m_Rotation = (float)(std::atan2f(diagonal.y, diagonal.x) * 180.0f) / (float)M_PI;
			if (m_Rotation < 0.0f)
			{
				offsetY = -0.5f;
			}
			else if (m_Rotation > 0.0f)
			{
				offsetY = 1.5f;
			}
			m_PositionX = diagonal.x * 0.5f + leftMostPositionScreen.x - m_CharacterSize * 3;
			m_PositionY = diagonal.y * 0.5f + leftMostPositionScreen.y - m_CharacterSize * offsetY;
			m_CountryNameText.setCharacterSize(m_CharacterSize);
			m_CountryNameText.setPosition(m_PositionX, m_PositionY);
			m_CountryNameText.setRotation(m_Rotation);
		}
		else
		{
			m_Conquered = true;
		}
	}

	void conquerRegion(unsigned int regionID)
	{
		m_OwnedRegionIDs.push_back(regionID);
		adjustText();
	}

	void loseRegion(unsigned regionID)
	{
		for (unsigned int index = 0; index < m_OwnedRegionIDs.size(); index++)
		{
			if (m_OwnedRegionIDs[index] == regionID)
			{
				m_OwnedRegionIDs.erase(m_OwnedRegionIDs.begin() + index);
				break;
			}
		}
		adjustText();
	}
};