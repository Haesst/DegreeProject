#include "Game/UI/UIText.h"
#include "Engine/Window.h"
#include "Game/Map/Map.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Engine/InputHandler.h"

UIText::UIText(UIID id, sf::Font font, std::string countryName, std::vector<unsigned int> ownedRegions)
{
	m_OwnedUIElement = id;
	m_Font = font;
	m_CountryName = countryName;
	m_OwnedRegionIDs = ownedRegions;
	m_Window = Window::getWindow();

	adjustText();
}

void UIText::start()
{
	adjustText();
}

void UIText::update()
{
	if (!m_Conquered)
	{
		if (InputHandler::getMouseScrolled())
		{
			if (InputHandler::m_TotalZoom <= m_HiddenDistance)
			{
				m_Hidden = true;
			}
			else
			{
				m_Hidden = false;
			}
			m_CountryNameText.setScale(InputHandler::m_InverseZoom, InputHandler::m_InverseZoom);
		}
		m_CountryNameText.setPosition(sf::Vector2f(m_Window->mapCoordsToPixel({ m_PositionX, m_PositionY })));	
	}
}
void UIText::render()
{
	if (!m_Conquered && !m_Hidden)
	{
		m_Window->draw(m_CountryNameText);
	}
}

void UIText::adjustText()
{
	if (m_OwnedRegionIDs.size() > 0)
	{
		m_Conquered = false;
		std::vector<SquareData>& mapData = Map::get().m_MapSquareData;
		Vector2DInt leftMostPosition = { Map::get().width, 0 };
		Vector2DInt rightMostPosition = { 0, 0 };
		for (SquareData& square : mapData)
		{
			for (unsigned int regionId : m_OwnedRegionIDs)
			{
				if (regionId == square.m_RegionID)
				{
					if (square.m_Position.x <= leftMostPosition.x)
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
		if ((unsigned int)(diagonal.x * 0.1f) >= m_MinCharacterSize && (unsigned int)(diagonal.x * 0.1f) <= m_MaxCharacterSize)
		{
			m_CharacterSize = (unsigned int)(diagonal.x * 0.1f);
		}
		else if ((unsigned int)(diagonal.x * 0.1f) < m_MinCharacterSize)
		{
			m_CharacterSize = m_MinCharacterSize;
		}
		else
		{
			m_CharacterSize = m_MaxCharacterSize;
		}
		m_OutlineThickness = diagonal.x * 0.001f;
		m_Rotation = (float)(std::atan2f(diagonal.y, diagonal.x) * 180.0f) / (float)M_PI;
		if (m_Rotation < 0.0f)
		{
			offsetY = m_Rotation * 0.005f;
		}
		else if (m_Rotation > 0.0f)
		{
			offsetY = m_Rotation * 0.001f;
		}
		else
		{
			offsetY = 0.01f;
		}
		m_PositionX = diagonal.x * 0.5f + leftMostPositionScreen.x - m_CharacterSize * m_CountryName.size() * 2 * 0.05f;
		m_PositionY = -(int)m_CharacterSize + diagonal.y * 0.5f + leftMostPositionScreen.y - m_CharacterSize * m_CountryName.size() * offsetY;
		m_CountryNameText.setFont(m_Font);
		m_CountryNameText.setString(m_CountryName);
		m_CountryNameText.setFillColor(m_FillColor);
		m_CountryNameText.setOutlineColor(m_OutlineColor);
		m_CountryNameText.setCharacterSize(m_CharacterSize);
		m_CountryNameText.setOutlineThickness(m_OutlineThickness);
		m_CountryNameText.setPosition(m_PositionX, m_PositionY);
		m_CountryNameText.setRotation(m_Rotation);
	}
	else
	{
		m_Conquered = true;
	}
}

void UIText::conquerRegion(unsigned int regionID)
{
	m_OwnedRegionIDs.push_back(regionID);
	adjustText();
}

void UIText::loseRegion(unsigned regionID)
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