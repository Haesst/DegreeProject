#pragma once

#include <SFML/Graphics.hpp>
#include "Game/Data/Types.h"
#include "Engine/Vector2D.h"

class MainMenu
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

	sf::Vector2f m_MainMenuPosition;
	sf::RectangleShape m_MainMenuShape;
	sf::Text m_MainMenuText;
	const char* m_MainMenuString = "Main Menu";

	sf::RectangleShape m_BackgroundShape;

	const unsigned int m_NumberOfButtons = 3;
	std::vector<sf::RectangleShape> m_ButtonShapes;
	std::vector<sf::Text> m_ButtonTexts;
	std::vector<const char*> m_ButtonStrings = { "Resume", "Options", "Exit" };

	MainMenu(UIID id, sf::Font font, Vector2D, Vector2D size);

	void update();
	void render();
	void openWindow();
	void closeWindow();
	void clickButton();
	void handleWindow();
};
