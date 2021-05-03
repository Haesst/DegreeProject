#include "Game/UI/EndWindow.h"
#include "Engine/Window.h"
#include "Engine/InputHandler.h"
#include "Game/Systems/CharacterManager.h"
#include "Engine/Time.h"

EndWindow::EndWindow(UIID id, sf::Font font, Vector2D, Vector2D size)
{
	m_OwnedUIWindow = id;
	m_Font = font;
	m_SizeX = size.x;
	m_SizeY = size.y;

	m_Window = Window::getWindow();

	setShape(m_BackgroundShape, m_BackgroundFillColor, m_TransparentColor, 0.0f, { (float)m_Window->getSize().x, (float)m_Window->getSize().y }, { m_Window->getSize().x * 0.5f, m_Window->getSize().y * 0.5f });

	setShape(m_EndWindowShape, m_FillColor, m_OwnerColor, m_OutlineThickness, { m_SizeX, m_SizeY }, m_BackgroundShape.getPosition());

	sf::Vector2f buttonPosition = { m_EndWindowShape.getPosition().x, m_EndWindowShape.getPosition().y + m_SizeY * 0.4f };
	setShape(m_ButtonShape, m_TransparentColor, m_OwnerColor, m_OutlineThickness, { m_SizeX * 0.25f, m_SizeY * 0.1f }, buttonPosition);

	setText(m_ButtonText, m_Font, m_CharacterSize, m_OwnerColor, m_ButtonShape.getPosition(), m_ButtonString);
}

void EndWindow::update()
{
	if (m_Visible)
	{
		clickButton();
	}
}

void EndWindow::render()
{
	if (m_Visible)
	{
		m_Window->draw(m_BackgroundShape);

		m_Window->draw(m_EndWindowShape);
		m_Window->draw(m_EndWindowText);

		m_Window->draw(m_ButtonShape);
		m_Window->draw(m_ButtonText);
	}
}

void EndWindow::openWindow(unsigned int loseCause)
{
	Time::pauseGame();

	Character& playerCharacter = CharacterManager::get().getPlayerCharacter();
	if (!playerCharacter.m_Dead)
	{
		m_OwnerColor = playerCharacter.m_RegionColor;
	}
	else
	{
		m_OwnerColor = playerCharacter.m_ColorAtDeath;
	}

	m_EndWindowShape.setOutlineColor(m_OwnerColor);

	setText(m_EndWindowText, m_Font, m_CharacterSize, m_OwnerColor, m_EndWindowShape.getPosition(), m_EndStrings[loseCause]);

	m_ButtonShape.setOutlineColor(m_OwnerColor);
	m_ButtonText.setFillColor(m_OwnerColor);

	m_Visible = true;
}

void EndWindow::clickButton()
{
	if (InputHandler::getLeftMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (m_ButtonShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
			m_Window->close();
		}
	}
}

void EndWindow::setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position)
{
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThickness);
	shape.setSize(size);
	shape.setOrigin(shape.getLocalBounds().width * 0.5f, shape.getLocalBounds().height * 0.5f);
	shape.setPosition(position);
}

void EndWindow::setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string)
{
	text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setFillColor(fillColor);
	text.setString(string);
	text.setOrigin(text.getLocalBounds().width * 0.5f, text.getLocalBounds().height * 0.5f);
	text.setPosition(position);
}
