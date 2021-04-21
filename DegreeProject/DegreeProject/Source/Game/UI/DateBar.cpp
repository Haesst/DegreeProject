#include "Game/UI/DateBar.h"
#include <SFML/Graphics.hpp>
#include "Engine/Window.h"
#include "Engine/Time.h"
#include "Game/GameDate.h"
#include "Game/Systems/CharacterManager.h"
#include "Engine/InputHandler.h"

DateBar::DateBar(UIID id, sf::Font font, Vector2D, Vector2D size)
{
	m_OwnedUIWindow = id;
	m_Font = font;
	m_SizeX = size.x;
	m_SizeY = size.y;
	for (unsigned int index = 0; index < NUMBER_OF_BUTTONS; index++)
	{
		m_ButtonShapes[index] = sf::RectangleShape();
	}
	for (unsigned int index = 0; index < NUMBER_OF_SPEEDS; index++)
	{
		m_SpeedShapes[index] = sf::RectangleShape();
	}
}

void DateBar::start()
{
	m_DaySubscriptionHandle = Time::m_GameDate.subscribeToDayChange([](void* data) { DateBar& datebar = *static_cast<DateBar*>(data); datebar.onDayChange(); }, static_cast<void*>(this));

	m_Window = Window::getWindow();

	updateStats();

	float sizeX = m_ButtonThickness * 0.5f;
	float sizeY = m_SizeY * 0.5f;

	m_OwnerColor = CharacterManager::get()->getPlayerCharacter().m_RegionColor;

	float positionX = m_Window->getSize().x - m_SizeX - m_OutlineThickness;
	float positionY = m_Window->getSize().y - m_SizeY - m_OutlineThickness;
	Vector2D position = { positionX, positionY };

	setShape(m_WindowShape, m_FillColor, m_OwnerColor, m_OutlineThickness, sf::Vector2f(m_SizeX, m_SizeY), position);

	sf::Color buttonFillColor = sf::Color::Transparent;
	sf::Vector2f buttonSize = sf::Vector2f(sizeY + sizeX, sizeY + sizeX);
	for (unsigned int index = 0; index < m_NumberOfButtons; index++)
	{
		Vector2D buttonPos = { positionX + m_ButtonThickness * 1.25f + m_ButtonThickness * 4.5f * index, positionY + m_SizeY * 0.25f - m_ButtonThickness * 0.25f };
		setShape(m_ButtonShapes[index], buttonFillColor, m_OwnerColor, m_ButtonThickness * 0.25f, buttonSize, buttonPos);
	}

	sf::Vector2f speedSize = sf::Vector2f(sizeY * 0.5f + sizeX, sizeY + sizeX);
	for (unsigned int index = 0; index < m_NumberOfSpeeds; index++)
	{
		Vector2D buttonPos = { positionX + m_ButtonThickness * 14.5f + m_ButtonThickness * 2.5f * index, positionY + m_SizeY * 0.25f - m_ButtonThickness * 0.25f };
		setShape(m_SpeedShapes[index], buttonFillColor, m_OwnerColor, m_ButtonThickness * 0.25f, speedSize, buttonPos);
	}

	sf::Vector2f buttonShapeSize = sf::Vector2f(sizeX, sizeY);

	Vector2D pauseButtonPosition = { positionX + m_ButtonThickness * 2, positionY + m_SizeY * 0.25f };
	setShape(m_PauseLeftShape, m_OwnerColor, buttonFillColor, 0.0f, buttonShapeSize, pauseButtonPosition);

	Vector2D pauseRightPosition = { positionX + m_ButtonThickness * 3, positionY + m_SizeY * 0.25f };
	setShape(m_PauseRightShape, m_OwnerColor, buttonFillColor, 0.0f, buttonShapeSize, pauseRightPosition);

	Vector2D decreaseSpeedPosition = { positionX + m_ButtonThickness * 8.5f, positionY + m_SizeY * 0.5f - m_ButtonThickness * 0.25f };
	setShape(m_DecreaseSpeedShape, m_OwnerColor, buttonFillColor, 0.0f, buttonShapeSize, decreaseSpeedPosition);
	m_DecreaseSpeedShape.setRotation(90.0f);

	Vector2D increaseSpeedHorizontalPos = { positionX + m_ButtonThickness * 13, positionY + m_SizeY * 0.5f - m_ButtonThickness * 0.25f };
	setShape(m_IncreaseSpeedHorizontalShape, m_OwnerColor, buttonFillColor, 0.0f, buttonShapeSize, increaseSpeedHorizontalPos);
	m_IncreaseSpeedHorizontalShape.setRotation(90.0f);

	Vector2D increaseSpeedVerticalPos = { positionX + m_ButtonThickness * 11.5f, positionY + m_SizeY * 0.25f };
	setShape(m_IncreaseSpeedVerticalShape, m_OwnerColor, buttonFillColor, 0.0f, buttonShapeSize, increaseSpeedVerticalPos);

	m_DateText.setFont(m_Font);
	m_DateText.setCharacterSize(m_CharacterSize);
	m_DateText.setFillColor(m_OwnerColor);
	m_DateText.setPosition(positionX + m_SizeX * 0.475f, positionY + m_OutlineThickness * 0.5f);
}

void DateBar::update()
{
	clickButton();
}

void DateBar::render()
{
	m_Window->draw(m_WindowShape);
	for (unsigned int index = 0; index < m_NumberOfButtons; index++)
	{
		m_Window->draw(m_ButtonShapes[index]);
	}
	for (unsigned int index = 0; index < m_NumberOfSpeeds; index++)
	{
		m_Window->draw(m_SpeedShapes[index]);
	}
	m_Window->draw(m_PauseLeftShape);
	m_Window->draw(m_PauseRightShape);
	m_Window->draw(m_DecreaseSpeedShape);
	m_Window->draw(m_IncreaseSpeedHorizontalShape);
	m_Window->draw(m_IncreaseSpeedVerticalShape);
	m_Window->draw(m_DateText);
}

void DateBar::onDayChange()
{
	updateStats();
}

void DateBar::updateStats()
{
	m_DateText.setString(Time::m_GameDate.getDateString());
	m_CurrentSpeedLevel = Time::m_CurrentSpeedLevel;
}

void DateBar::clickButton()
{
	if (InputHandler::getLeftMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		for (unsigned int index = 0; index < m_NumberOfButtons; index++)
		{
			if (m_ButtonShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				switch (index)
				{
					case 0:
					{
						Time::gamePaused() ? Time::unpauseGame() : Time::pauseGame();
						break;
					}
					case 1:
					{
						Time::decreaseGameSpeed();
						break;
					}
					case 2:
					{
						Time::increaseGameSpeed();
						break;
					}
					default:
					{
						break;
					}
				}
			}
		}
	}
	for (unsigned int index = 0; index < m_NumberOfSpeeds; index++)
	{
		if (index < m_CurrentSpeedLevel && Time::gamePaused())
		{
			m_SpeedShapes[index].setFillColor(sf::Color::Red);
		}
		else if (index < m_CurrentSpeedLevel)
		{
			m_SpeedShapes[index].setFillColor(m_SpeedColor);
		}
		else
		{
			m_SpeedShapes[index].setFillColor(sf::Color::Transparent);
		}
	}
}

void DateBar::updateOwnerColor(sf::Color& newColor)
{
	m_OwnerColor = newColor;

	m_PauseLeftShape.setFillColor(m_OwnerColor);
	m_PauseRightShape.setFillColor(m_OwnerColor);
	m_DecreaseSpeedShape.setFillColor(m_OwnerColor);
	m_IncreaseSpeedHorizontalShape.setFillColor(m_OwnerColor);
	m_IncreaseSpeedVerticalShape.setFillColor(m_OwnerColor);
	m_DateText.setFillColor(m_OwnerColor);
}

void DateBar::setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f& size, Vector2D& position)
{
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThickness);
	shape.setSize(size);
	shape.setPosition(position.x, position.y);
}
