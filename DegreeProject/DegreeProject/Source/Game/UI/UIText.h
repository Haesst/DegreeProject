#pragma once

#include <SFML/Graphics.hpp>
#include "Game/Data/UIType.h"
#include "Game/Data/Types.h"
#include "Engine/Vector2D.h"

class UIText
{
public:

	UIText(UIID id, sf::Font font, std::string realmName, std::vector<unsigned int> ownedRegions);

	void start();
	void update();
	void render();
	void adjustText();
	void conquerRegion(unsigned int regionID);
	void loseRegion(unsigned regionID);
	void setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position);
	void setFadeColor(sf::Color& color);
	void setText();
	void updateText();
	void setFixedPosition();
	void fadeText();
	void scaleText();
	void calculatePosition(Vector2DInt realmPositions, unsigned int numberOfRealmPositions);
	void calculateSize(Vector2D diagonal);
	Vector2D calculateDiagonal(Vector2D topLeftMostPositionScreen, Vector2D bottomRightMostPositionScreen, Vector2D bottomLeftMostPositionScreen, Vector2D topRightMostPositionScreen);

	sf::Uint8 m_MaxTextOpacity = 150;
	sf::Color m_FillColor = sf::Color(255, 252, 240, 100);
	sf::Color m_OutlineColor = sf::Color(0, 0, 0, 100);
	sf::Color m_OpacityColor;
	float m_OutlineThickness = 1.0f;
	float m_PositionX = 960.0f;
	float m_PositionY = 540.0f;
	sf::Text m_CountryNameText;
	sf::Font m_Font;
	std::string m_CountryName = "";
	unsigned int m_CharacterSize = 20;
	unsigned int m_MinCharacterSize = 30;
	unsigned int m_MaxCharacterSize = 1000;
	const float m_MinOutlineThickness = 0.1f;
	const float m_MaxOutlineThickness = 1.0f;
	float m_Rotation = 0.0f;
	std::vector<unsigned int> m_OwnedRegionIDs;
	bool m_Conquered = false;
	sf::RenderWindow* m_Window;
	UIID m_OwnedUIElement = INVALID_UI_ID;
	bool m_Hidden = false;
	float m_HiddenDistance = 0.6f;
	float m_FadeSpeed = 0.4f;

	bool m_Debug = false;
	std::vector<sf::RectangleShape> m_CornerShapes;
	std::vector<sf::Vector2f> m_CornerPositions;
};