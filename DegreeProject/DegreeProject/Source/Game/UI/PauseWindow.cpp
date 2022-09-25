#include <Game/UI/PauseWindow.h>
#include <Game/Systems/CharacterManager.h>

PauseWindow::PauseWindow(UIID id, sf::Font font, Vector2D position, Vector2D size) : UIWindow(id, font, position, size)
{
	m_OutlineColor = m_BackgroundFillColor;
	m_WindowPosition = { m_PositionX - m_SizeX * 0.5f, m_PositionY - m_SizeY * 0.5f };
	m_TextStrings = { "Paused" };
	m_CharacterSize = 25;
}

void PauseWindow::start()
{
	UIWindow::start();

	setShape(m_WindowShape, m_BackgroundFillColor, m_BackgroundFillColor, m_OutlineThickness, { m_SizeX, m_SizeY }, m_WindowPosition);
	for (unsigned int index = 0; index < m_TextStrings.size(); index++)
	{
		m_Texts.push_back(sf::Text());
		setText(m_Texts[index], m_Font, m_CharacterSize, m_TextFillColor, { m_WindowPosition.x + m_SizeX * 0.25f, m_WindowPosition.y + m_SizeY * 0.2f }, m_TextStrings[index].c_str());
	}
}

void PauseWindow::render()
{
	UIWindow::render();
}

void PauseWindow::closeWindow()
{
	UIWindow::closeWindow();
}

void PauseWindow::openWindow()
{
	m_TextFillColor = CharacterManager::get().getPlayerCharacter().m_RegionColor;

	UIWindow::openWindow();
}
