#pragma once

#include <SFML/Graphics.hpp>
#include "Game/Data/Types.h"
#include "Engine/Vector2D.h"

class EndWindow
{
public:

	sf::RenderWindow* m_Window = nullptr;
	UIID m_OwnedUIWindow = INVALID_UI_ID;
	float m_SizeX = 300.0f;
	float m_SizeY = 500.0f;
	float m_PositionX = 960.0f;
	float m_PositionY = 540.0f;
	float m_OutlineThickness = 10.0f;
	int m_CharacterSize = 25;
	bool m_Visible = false;
	sf::Font m_Font;
	sf::Color m_BackgroundFillColor = sf::Color(0, 0, 0, 100);
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Black;
	sf::Color m_TransparentColor = sf::Color::Transparent;

	sf::RectangleShape m_EndWindowShape;
	sf::Text m_EndWindowText;
	std::vector<const char*> m_EndStrings = { "Your\nRoyal\nLine\nEnds\nHere", "Your\nRealm\nWas\nConquered", "The\nWorld\nIs\nYours" };
	unsigned int m_EndStringIndex = 2;

	sf::RectangleShape m_BackgroundShape;

	sf::RectangleShape m_ButtonShape;
	sf::Text m_ButtonText;
	const char* m_ButtonString = "Exit";

	EndWindow(UIID id, sf::Font font, Vector2D, Vector2D size);

	void update();
	void render();
	void openWindow(unsigned int loseCause);
	void clickButton();
	void setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position);
	void setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string);
};
