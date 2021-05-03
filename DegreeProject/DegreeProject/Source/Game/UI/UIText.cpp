#include "Game/UI/UIText.h"
#include "Engine/Window.h"
#include "Game/Map/Map.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Engine/InputHandler.h"

UIText::UIText(UIID id, sf::Font font, std::string realmName, std::vector<unsigned int> ownedRegions)
{
	m_OwnedUIElement = id;
	m_Font = font;
	m_CountryName = realmName;
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

			m_OpacityColor = m_FillColor;
			m_OpacityColor.a = (sf::Uint8)(m_OpacityColor.a * InputHandler::m_TotalZoom * m_FadeSpeed);
			if (m_OpacityColor.a > m_MaxTextOpacity)
			{
				m_OpacityColor.a = m_MaxTextOpacity;
			}
			m_CountryNameText.setFillColor(m_OpacityColor);

			m_OpacityColor = m_OutlineColor;
			m_OpacityColor.a = (sf::Uint8)(m_OpacityColor.a * InputHandler::m_TotalZoom * m_FadeSpeed);
			if (m_OpacityColor.a > m_MaxTextOpacity)
			{
				m_OpacityColor.a = m_MaxTextOpacity;
			}
			m_CountryNameText.setOutlineColor(m_OpacityColor);

			if (m_Debug)
			{
				for (sf::RectangleShape& cornerShape : m_CornerShapes)
				{
					cornerShape.setScale(InputHandler::m_InverseZoom, InputHandler::m_InverseZoom);
				}
			}
		}
		m_CountryNameText.setPosition(sf::Vector2f(m_Window->mapCoordsToPixel({ m_PositionX, m_PositionY })));

		if (m_Debug)
		{
			for (unsigned int index = 0; index < m_CornerShapes.size(); index++)
			{
				m_CornerShapes[index].setPosition(sf::Vector2f(m_Window->mapCoordsToPixel(m_CornerPositions[index])));
			}
		}
	}
}
void UIText::render()
{
	if (!m_Conquered && !m_Hidden)
	{
		m_Window->draw(m_CountryNameText);
	}

	if (m_Debug)
	{
		for (sf::RectangleShape& cornerShape : m_CornerShapes)
		{
			m_Window->draw(cornerShape);
		}
	}
}

void UIText::adjustText()
{
	if (m_Debug)
	{
		m_CornerShapes.clear();
		m_CornerPositions.clear();
	}

	if (m_OwnedRegionIDs.size() > 0)
	{
		m_Conquered = false;
		Map& map = Map::get();
		std::vector<SquareData>& mapData = map.m_MapSquareData;
		Vector2DInt topLeftMostPosition = { map.width, map.height };
		Vector2DInt topRightMostPosition = { 0, map.height };
		Vector2DInt bottomLeftMostPosition = { map.width, 0 };
		Vector2DInt bottomRightMostPosition = { 0, 0 };
		for (SquareData& square : mapData)
		{
			for (unsigned int regionId : m_OwnedRegionIDs)
			{
				if (regionId == square.m_RegionID)
				{
					if (square.m_Position.x + square.m_Position.y * map.m_AspectRatio <= topLeftMostPosition.x + topLeftMostPosition.y * map.m_AspectRatio)
					{
						topLeftMostPosition = square.m_Position;
					}

					if (square.m_Position.x - square.m_Position.y * map.m_AspectRatio > topRightMostPosition.x - topRightMostPosition.y * map.m_AspectRatio)
					{
						if (square.m_Position.x - topRightMostPosition.x >= square.m_Position.y * map.m_AspectRatio - topRightMostPosition.y * map.m_AspectRatio)
						{
							topRightMostPosition = square.m_Position;
						}
					}

					if (square.m_Position.x - square.m_Position.y * map.m_AspectRatio < bottomLeftMostPosition.x - bottomLeftMostPosition.y * map.m_AspectRatio)
					{
						if (square.m_Position.x - bottomLeftMostPosition.x <= square.m_Position.y * map.m_AspectRatio - bottomLeftMostPosition.y * map.m_AspectRatio)
						{
							bottomLeftMostPosition = square.m_Position;
						}
					}

					if (square.m_Position.x + square.m_Position.y * map.m_AspectRatio >= bottomRightMostPosition.x + bottomRightMostPosition.y * map.m_AspectRatio)
					{
						bottomRightMostPosition = square.m_Position;
					}
				}
			}
		}

		Vector2D topLeftMostPositionScreen = map.convertToScreen(topLeftMostPosition);
		Vector2D topRightMostPositionScreen = map.convertToScreen(topRightMostPosition);
		Vector2D bottomLeftMostPositionScreen = map.convertToScreen(bottomLeftMostPosition);
		Vector2D bottomRightMostPositionScreen = map.convertToScreen(bottomRightMostPosition);

		if (m_Debug)
		{
			sf::RectangleShape cornerShape;
			sf::Color color = sf::Color(255, 0, 0, 150); // RED
			setShape(cornerShape, color, m_OutlineColor, m_OutlineThickness, { 16.0f, 16.0f * map.m_AspectRatio }, { topLeftMostPositionScreen.x, topLeftMostPositionScreen.y });
			m_CornerShapes.push_back(cornerShape);											  
			m_CornerPositions.push_back(cornerShape.getPosition());							  
																							  
			color = sf::Color(255, 0, 255, 150); // MAGENTA									  
			setShape(cornerShape, color, m_OutlineColor, m_OutlineThickness, { 16.0f, 16.0f * map.m_AspectRatio }, { topRightMostPositionScreen.x, topRightMostPositionScreen.y });
			m_CornerShapes.push_back(cornerShape);											  
			m_CornerPositions.push_back(cornerShape.getPosition());							  
																							  
			color = sf::Color(255, 255, 0, 150); // YELLOW									  
			setShape(cornerShape, color, m_OutlineColor, m_OutlineThickness, { 16.0f, 16.0f * map.m_AspectRatio }, { bottomLeftMostPositionScreen.x, bottomLeftMostPositionScreen.y });
			m_CornerShapes.push_back(cornerShape);											  
			m_CornerPositions.push_back(cornerShape.getPosition());							  
																							  
			color = sf::Color(0, 255, 255, 150); // CYAN									  
			setShape(cornerShape, color, m_OutlineColor, m_OutlineThickness, { 16.0f, 16.0f * map.m_AspectRatio }, { bottomRightMostPositionScreen.x, bottomRightMostPositionScreen.y });
			m_CornerShapes.push_back(cornerShape);
			m_CornerPositions.push_back(cornerShape.getPosition());
		}

		Vector2D diagonal;
		Vector2D startPosition;
		Vector2D topLeftToBottomRightDiagonal = bottomRightMostPositionScreen - topLeftMostPositionScreen;
		Vector2D bottomLeftToTopRightDiagonal = topRightMostPositionScreen - bottomLeftMostPositionScreen;
		topLeftToBottomRightDiagonal.y *= map.m_AspectRatio;
		bottomLeftToTopRightDiagonal.y *= map.m_AspectRatio;
		float topLeftbottomRightLength = topLeftToBottomRightDiagonal.getLength();
		float bottomLeftToTopRightLength = bottomLeftToTopRightDiagonal.getLength();
		if (topLeftbottomRightLength >= bottomLeftToTopRightLength)
		{
			diagonal = topLeftToBottomRightDiagonal;
		}
		else
		{
			diagonal = bottomLeftToTopRightDiagonal;
		}
		startPosition = calculateIntersection(topLeftMostPositionScreen, bottomRightMostPositionScreen, bottomLeftMostPositionScreen, topRightMostPositionScreen);

		float diagonalLength = diagonal.getLength();
		if ((unsigned int)(diagonalLength * 0.1f) >= m_MinCharacterSize && (unsigned int)(diagonalLength * 0.1f) <= m_MaxCharacterSize)
		{
			m_CharacterSize = (unsigned int)(diagonalLength * 0.1f);
			m_OutlineThickness = diagonalLength * 0.001f;
		}
		else if ((unsigned int)(diagonalLength * 0.1f) < m_MinCharacterSize)
		{
			m_CharacterSize = m_MinCharacterSize;
			m_OutlineThickness = m_MinOutlineThickness;
		}
		else
		{
			m_CharacterSize = m_MaxCharacterSize;
			m_OutlineThickness = m_MaxOutlineThickness;
		}

		m_Rotation = (float)(std::atan2f(diagonal.y, diagonal.x) * 180.0f) / (float)M_PI;
		m_PositionX = startPosition.x;
		m_PositionY = startPosition.y;
		setText(m_CountryNameText, m_Font, m_CharacterSize, m_FillColor, m_OutlineColor, m_OutlineThickness, { m_PositionX, m_PositionY }, m_Rotation, m_CountryName);
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

void UIText::setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position)
{
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThickness);
	shape.setSize(size);
	shape.setPosition(position);
	shape.setScale(InputHandler::m_InverseZoom, InputHandler::m_InverseZoom);
}

void UIText::setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f position, float rotation, std::string string)
{
	text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setFillColor(fillColor);
	text.setOutlineColor(outlineColor);
	text.setOutlineThickness(outlineThickness);
	text.setString(string);
	text.setOrigin(text.getLocalBounds().width * 0.5f, text.getLocalBounds().height * 0.75f);
	text.setPosition(position);
	text.setRotation(rotation);
	text.setScale(InputHandler::m_InverseZoom, InputHandler::m_InverseZoom);
}

Vector2D UIText::calculateIntersection(Vector2D startV1, Vector2D endV1, Vector2D startV2, Vector2D endV2)
{
	// y = k * x + m
	float k1 = (endV1.y - startV1.y) / (endV1.x - startV1.x);
	float k2 = (endV2.y - startV2.y) / (endV2.x - startV2.x);
	float m1 = endV1.y - k1 * endV1.x;
	float m2 = endV2.y - k2 * endV2.x;
	float x = (m2 - m1) / (k1 - k2);
	float y = k1 * x + m1;
	if (m_Debug)
	{
		sf::RectangleShape intersectShape;
		sf::Color color = sf::Color::Black;
		setShape(intersectShape, color, m_OutlineColor, 0.0f, { 16.0f, 16.0f * Map::get().m_AspectRatio }, { x, y });
		m_CornerShapes.push_back(intersectShape);
		m_CornerPositions.push_back(intersectShape.getPosition());
	}
	return Vector2D(x, y);
}
