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
	m_BackgroundShape.setPosition(sf::Vector2f());

	m_MainMenuPosition = sf::Vector2f(m_PositionX - m_SizeX * 0.5f, m_PositionY - m_SizeY * 0.5f);
	m_MainMenuShape.setFillColor(m_FillColor);
	m_MainMenuShape.setOutlineThickness(m_OutlineThickness);
	m_MainMenuShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));
	m_MainMenuShape.setPosition(m_MainMenuPosition);

	m_MainMenuText.setFont(m_Font);
	m_MainMenuText.setCharacterSize(m_CharacterSize);
	m_MainMenuText.setString(m_MainMenuString);
	m_MainMenuText.setPosition(sf::Vector2f(m_MainMenuPosition.x + m_SizeX * 0.25f, m_MainMenuPosition.y));

	for (unsigned int index = 0; index < m_NumberOfButtons; index++)
	{
		sf::Vector2f buttonPosition = sf::Vector2f(m_MainMenuPosition.x + m_SizeX * 0.25f, m_MainMenuPosition.y + 100 + 100 * index);

		sf::RectangleShape buttonShape(sf::Vector2f(m_SizeX * 0.25f, m_SizeY * 0.1f));
		buttonShape.setFillColor(sf::Color::Transparent);
		buttonShape.setOutlineThickness(m_OutlineThickness);
		buttonShape.setPosition(buttonPosition);
		m_ButtonShapes.push_back(buttonShape);

		sf::Text buttonText;
		buttonText.setFont(m_Font);
		buttonText.setCharacterSize(m_CharacterSize);
		buttonText.setString(m_ButtonStrings[index]);
		buttonText.setPosition(buttonPosition);
		m_ButtonTexts.push_back(buttonText);
	}
}

void MainMenu::update()
{
	handleWindow();

	if (m_Visible)
	{
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
		Vector2D mousePosition = InputHandler::getUIMousePosition();
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
