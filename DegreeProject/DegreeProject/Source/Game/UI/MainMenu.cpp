#include "Game/UI/MainMenu.h"
#include "Engine/Window.h"
#include "Engine/InputHandler.h"
#include "Game/Systems/CharacterManager.h"
#include "Engine/Time.h"

MainMenu::MainMenu(UIID id, sf::Font font, Vector2D, Vector2D size)
{
	m_OwnedUIWindow = id;
	m_Font = font;
	m_SizeX = size.x;
	m_SizeY = size.y;

	m_Window = Window::getWindow();

	m_BackgroundShape.setFillColor(m_BackgroundFillColor);
	m_BackgroundShape.setSize(sf::Vector2f((float)m_Window->getSize().x, (float)m_Window->getSize().y));

	m_MainMenuPosition = Vector2DInt((int)(m_PositionX - m_SizeX * 0.5f), (int)(m_PositionY - m_SizeY * 0.5f));
	m_MainMenuShape.setFillColor(m_FillColor);
	m_MainMenuShape.setOutlineThickness(m_OutlineThickness);
	m_MainMenuShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));

	m_MainMenuText.setFont(m_Font);
	m_MainMenuText.setCharacterSize(m_CharacterSize);
	m_MainMenuText.setStyle(m_Style);
	m_MainMenuText.setString(m_MainMenuString);
	
	for (unsigned int index = 0; index < m_NumberOfButtons; index++)
	{
		m_ButtonPositions.push_back(Vector2DInt((int)(m_MainMenuPosition.x + m_SizeX * 0.25f), (int)(m_MainMenuPosition.y + 100 + 100 * index)));

		sf::RectangleShape buttonShape(sf::Vector2f(m_SizeX * 0.25f, m_SizeY * 0.1f));
		buttonShape.setFillColor(sf::Color::Transparent);
		buttonShape.setOutlineThickness(m_OutlineThickness);
		m_ButtonShapes.push_back(buttonShape);

		sf::Text buttonText;
		buttonText.setFont(m_Font);
		buttonText.setCharacterSize(m_CharacterSize);
		buttonText.setStyle(m_Style);
		buttonText.setString(m_ButtonStrings[index]);
		m_ButtonTexts.push_back(buttonText);
	}
}

void MainMenu::start()
{

}

void MainMenu::update()
{
	handleWindow();

	if (m_Visible)
	{
		m_BackgroundShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i()));

		m_MainMenuShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_MainMenuPosition.x, m_MainMenuPosition.y)));
		m_MainMenuText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_MainMenuPosition.x + m_SizeX * 0.25f), m_MainMenuPosition.y)));

		for (unsigned int index = 0; index < m_NumberOfButtons; index++)
		{
			m_ButtonShapes[index].setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_ButtonPositions[index].x, m_ButtonPositions[index].y)));
			m_ButtonTexts[index].setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_ButtonPositions[index].x, m_ButtonPositions[index].y)));
		}

		clickButton();
	}
}

void MainMenu::render()
{
	if (m_Visible)
	{
		m_Window->draw(m_BackgroundShape);

		m_Window->draw(m_MainMenuShape);
		m_Window->draw(m_MainMenuText);

		for (unsigned int index = 0; index < m_NumberOfButtons; index++)
		{
			m_Window->draw(m_ButtonShapes[index]);
			m_Window->draw(m_ButtonTexts[index]);
		}
	}
}

void MainMenu::openWindow()
{
	Time::pauseGame();

	m_MainMenuShape.setOutlineColor(m_OwnerColor);

	m_MainMenuText.setFillColor(m_OwnerColor);

	for (unsigned int index = 0; index < m_NumberOfButtons; index++)
	{
		m_ButtonShapes[index].setOutlineColor(m_OwnerColor);
		m_ButtonTexts[index].setFillColor(m_OwnerColor);
	}

	m_Visible = true;
}

void MainMenu::closeWindow()
{
	Time::unpauseGame();
	m_Visible = false;
}

void MainMenu::clickButton()
{
	if (InputHandler::getLeftMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getMousePosition();
		for (unsigned int index = 0; index < m_NumberOfButtons; index++)
		{
			if (m_ButtonShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				if (m_ButtonStrings[index] == std::string("Resume"))
				{
					closeWindow();
				}
				else if (m_ButtonStrings[index] == std::string("Exit"))
				{
					m_Window->close();
				}
			}
		}
	}
}

void MainMenu::handleWindow()
{
	if (!m_Visible && InputHandler::getEscapePressed() && !InputHandler::getCharacterWindowOpen() && !InputHandler::getRegionWindowOpen() && !InputHandler::getWarWindowOpen())
	{
		m_OwnerColor = CharacterManager::get()->getPlayerCharacter().m_RegionColor;
		openWindow();
	}
	else if (m_Visible && InputHandler::getEscapePressed())
	{
		closeWindow();
	}
}
