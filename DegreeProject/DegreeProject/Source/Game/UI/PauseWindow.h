#pragma once

#include "Game/Data/Types.h"
#include <SFML/Graphics.hpp>
#include "Engine/Vector2D.h"

class PauseWindow
{
public:
	PauseWindow(UIID ID, sf::Font font, Vector2D position, Vector2D size);

	void render();
	void deactivate();
	void activate();
	void setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position);
	void setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string = "");
	bool m_Active = false;

private:
	UIID m_OwnerUIElement;
	sf::Font m_Font;
	float m_SizeX = 300.0f;
	float m_SizeY = 100.0f;
	float m_PositionX = 960.0f;
	float m_PositionY = 540.0f;
	int m_CharacterSize = 25;
	sf::Color m_PausedShapeColor = sf::Color(0, 0, 0, 100);
	sf::Color m_PausedTextColor = sf::Color::Red;
	const char* m_Paused = "Paused";
	sf::RenderWindow* m_Window = nullptr;
	sf::RectangleShape m_PausedShape;
	sf::Text m_PausedText;
};
