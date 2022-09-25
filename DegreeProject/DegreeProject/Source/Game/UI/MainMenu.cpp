#include <Game/UI/MainMenu.h>
#include <Game/Systems/CharacterManager.h>
#include <Engine/Time.h>
#include <Engine/InputHandler.h>

MainMenu::MainMenu(UIID id, sf::Font font, Vector2D position, Vector2D size) : UIWindow(id, font, position, size)
{
	m_OutlineThickness = 10.0f;
	m_WindowFillColor = sf::Color(255, 252, 240);
	m_WindowPosition = { m_PositionX - m_SizeX * 0.5f, m_PositionY - m_SizeY * 0.5f };
	m_TextStrings = { "Main Menu" };
	m_CharacterSize = 25;
	m_ButtonStrings = { "Resume", "Options", "Exit" };
}

void MainMenu::start()
{
	UIWindow::start();

	setShape(m_BackgroundShape, m_BackgroundFillColor, m_TransparentColor, 0.0f, { (float)m_Window->getSize().x, (float)m_Window->getSize().y }, {  });
	setShape(m_WindowShape, m_WindowFillColor, m_TextFillColor, m_OutlineThickness, { m_SizeX, m_SizeY }, m_WindowPosition);
	for (unsigned int index = 0; index < m_TextStrings.size(); index++)
	{
		m_Texts.push_back(sf::Text());
		setText(m_Texts[index], m_Font, m_CharacterSize, m_TextFillColor, { m_WindowPosition.x + m_SizeX * 0.5f, m_WindowPosition.y + m_SizeY * 0.05f }, m_TextStrings[index].c_str(), true);
	}
	for (unsigned int index = 0; index < m_ButtonStrings.size(); index++)
	{
		sf::Vector2f buttonPosition = { m_WindowPosition.x + m_SizeX * 0.5f, m_WindowPosition.y + m_SizeY * 0.25f + m_SizeY * 0.25f * index };
		m_Buttons.push_back(new Button(m_Font, m_Window, buttonPosition, { m_SizeX * 0.25f, m_SizeY * 0.1f }));
		m_Buttons[index]->setShape(m_TransparentColor, m_OutlineColor, m_OutlineThickness * 0.5f, { m_SizeX * 0.25f, m_SizeY * 0.1f }, buttonPosition, true);
		m_Buttons[index]->setText(m_Font, m_CharacterSize, m_TextFillColor, buttonPosition, m_ButtonStrings[index], true);
	}
}

void MainMenu::update()
{
	UIWindow::update();

	if (m_Visible)
	{
		clickButton();
	}
}

void MainMenu::render()
{
	UIWindow::render();
}

void MainMenu::openWindow()
{
	UIWindow::openWindow();

	m_WasPaused = Time::gamePaused();
	if (!m_WasPaused)
	{
		Time::pauseGame();
	}
}

void MainMenu::closeWindow()
{
	UIWindow::closeWindow();

	if (!m_WasPaused)
	{
		Time::unpauseGame();
	}
}

void MainMenu::clickButton()
{
	UIWindow::clickButton();

	if (InputHandler::getLeftMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		for (Button* button : m_Buttons)
		{
			if (button->m_Shape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				std::string buttonString = button->m_Text.getString();
				InputHandler::setLeftMouseReleased(false);
				if (buttonString == m_ButtonStrings[0])
				{
					closeWindow();
				}
				else if (buttonString == m_ButtonStrings[m_ButtonStrings.size() - 1])
				{
					m_Window->close();
				}
			}
		}
	}
}

void MainMenu::handleWindow()
{
	UIWindow::handleWindow();

	if (!m_Visible && InputHandler::getEscapePressed() && !InputHandler::getCharacterWindowOpen() && !InputHandler::getRegionWindowOpen() && !InputHandler::getWarWindowOpen())
	{
		m_TextFillColor = CharacterManager::get().getPlayerCharacter().m_RegionColor;
		m_OutlineColor = m_TextFillColor;
		openWindow();
	}
	else if (m_Visible && InputHandler::getEscapePressed())
	{
		closeWindow();
	}
}
