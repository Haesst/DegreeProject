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

	setShape(m_BackgroundShape, m_BackgroundFillColor, m_TransparentColor, 0.0f, { (float)m_Window->getSize().x, (float)m_Window->getSize().y }, {  });

	m_MainMenuPosition = { m_PositionX - m_SizeX * 0.5f, m_PositionY - m_SizeY * 0.5f };
	setShape(m_MainMenuShape, m_FillColor, m_OwnerColor, m_OutlineThickness, { m_SizeX, m_SizeY }, m_MainMenuPosition);
	setText(m_MainMenuText, m_Font, m_CharacterSize, m_OwnerColor, { m_MainMenuPosition.x + m_SizeX * 0.25f, m_MainMenuPosition.y }, m_MainMenuString);

	for (unsigned int index = 0; index < m_NumberOfButtons; index++)
	{
		sf::Vector2f buttonPosition = { m_MainMenuPosition.x + m_SizeX * 0.25f, m_MainMenuPosition.y + 100 + 100 * index };

		sf::RectangleShape buttonShape;
		setShape(buttonShape, m_TransparentColor, m_OwnerColor, m_OutlineThickness, { m_SizeX * 0.25f, m_SizeY * 0.1f }, buttonPosition);
		m_ButtonShapes.push_back(buttonShape);

		sf::Text buttonText;
		setText(buttonText, m_Font, m_CharacterSize, m_OwnerColor, buttonPosition, m_ButtonStrings[index]);
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
				InputHandler::setLeftMouseReleased(false);
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
		m_OwnerColor = CharacterManager::get().getPlayerCharacter().m_RegionColor;
		openWindow();
	}
	else if (m_Visible && InputHandler::getEscapePressed())
	{
		closeWindow();
	}
}

void MainMenu::setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position)
{
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThickness);
	shape.setSize(size);
	shape.setPosition(position);
}

void MainMenu::setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string)
{
	text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setFillColor(fillColor);
	text.setString(string);
	text.setPosition(position);
}
