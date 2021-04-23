#include "Game/UI/PauseWindow.h"
#include "Engine/Window.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/Data/Character.h"

PauseWindow::PauseWindow(UIID ID, sf::Font font, Vector2D position, Vector2D size)
{
	m_OwnerUIElement = ID;
	m_Font = font;
	m_SizeX = size.x;
	m_SizeY = size.y;
	m_PositionX = position.x;
	m_PositionY = position.y;

	m_Window = Window::getWindow();

	sf::Vector2f shapeSize = { m_SizeX, m_SizeY };
	sf::Vector2f shapePosition = { m_PositionX - m_SizeX * 0.5f, m_PositionY - m_SizeY * 0.5f };
	setShape(m_PausedShape, m_PausedShapeColor, m_PausedShapeColor, 0.0f, shapeSize, shapePosition);
	setText(m_PausedText, m_Font, m_CharacterSize, m_PausedTextColor, { shapePosition.x + m_SizeX * 0.25f, shapePosition.y + m_SizeY * 0.2f }, m_Paused);
}

void PauseWindow::render()
{
	if (m_Active)
	{
		m_Window->draw(m_PausedShape);
		m_Window->draw(m_PausedText);
	}
}

void PauseWindow::deactivate()
{
	m_Active = false;
}

void PauseWindow::activate()
{
	m_PausedTextColor = CharacterManager::get().getPlayerCharacter().m_RegionColor;
	m_PausedText.setFillColor(m_PausedTextColor);
	m_Active = true;
}

void PauseWindow::setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position)
{
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThickness);
	shape.setSize(size);
	shape.setPosition(position);
}

void PauseWindow::setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string)
{
	text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setFillColor(fillColor);
	text.setString(string);
	text.setPosition(position);
}
