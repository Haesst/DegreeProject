#include <Game/UI/EndWindow.h>
#include <Engine/Window.h>
#include <Engine/InputHandler.h>
#include <Game/Systems/CharacterManager.h>
#include <Engine/Time.h>
#include <Game/Systems/SoundManager.h>

EndWindow::EndWindow(UIID id, sf::Font font, Vector2D position, Vector2D size) : UIWindow(id, font, position, size)
{
	m_OutlineThickness = 10.0f;
	m_CharacterSize = 25;
	m_ButtonStrings = { "Exit" };
}

void EndWindow::start()
{
	UIWindow::start();

	setShape(m_BackgroundShape, m_BackgroundFillColor, m_TransparentColor, 0.0f, { (float)m_Window->getSize().x, (float)m_Window->getSize().y }, { m_Window->getSize().x * 0.5f, m_Window->getSize().y * 0.5f }, true);
	setShape(m_WindowShape, m_WindowFillColor, m_WindowFillColor, m_OutlineThickness, { m_SizeX, m_SizeY }, m_BackgroundShape.getPosition(), true);
	for (unsigned int index = 0; index < m_ButtonStrings.size(); index++)
	{
		sf::Vector2f buttonPosition = { m_WindowShape.getPosition().x, m_WindowShape.getPosition().y + m_SizeY * 0.4f };
		m_Buttons.push_back(new Button(m_Font, m_Window, buttonPosition, { m_SizeX * 0.25f, m_SizeY * 0.1f }));
		m_Buttons[index]->setShape(m_TransparentColor, m_OutlineColor, m_OutlineThickness * 0.5f, { m_SizeX * 0.25f, m_SizeY * 0.1f }, buttonPosition, true);
		m_Buttons[index]->setText(m_Font, m_CharacterSize, m_TextFillColor, buttonPosition, m_ButtonStrings[index], true);
	}
}

void EndWindow::update()
{
	UIWindow::update();

	if (m_Visible)
	{
		clickButton();
	}
}

void EndWindow::render()
{
	UIWindow::render();
}

void EndWindow::openWindow(unsigned int loseCause)
{
	Time::pauseGame();

	sf::Color ownerColor;
	Character& playerCharacter = CharacterManager::get().getPlayerCharacter();
	if (!playerCharacter.m_Dead)
	{
		ownerColor = playerCharacter.m_RegionColor;
	}
	else
	{
		ownerColor = playerCharacter.m_ColorAtDeath;
	}
	m_TextFillColor = ownerColor;
	m_OutlineColor = ownerColor;

	UIWindow::openWindow();

	m_Texts.push_back(sf::Text());
	setText(m_Texts[0], m_Font, m_CharacterSize, m_TextFillColor, m_WindowShape.getPosition(), m_EndStrings[loseCause], true);

	SoundManager& soundManager = SoundManager::get();

	soundManager.stopCurrentMusic();
	soundManager.stopBattleMusic();
	if (loseCause > 1)
	{
		soundManager.playVictoryMusic();
	}
	else
	{
		soundManager.playDefeatMusic();
	}
}

void EndWindow::clickButton()
{
	UIWindow::clickButton();

	if (InputHandler::getLeftMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		for (Button* button : m_Buttons)
		{
			if (button->m_Shape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				InputHandler::setLeftMouseReleased(false);
				m_Window->close();
			}
		}
	}
}
