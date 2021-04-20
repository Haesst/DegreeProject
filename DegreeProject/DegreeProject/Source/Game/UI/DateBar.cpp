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
	Time::m_GameDate.subscribeToMonthChange(std::bind(&DateBar::onMonthChange, this));

	m_Window = Window::getWindow();

	updateStats();

	float sizeX = m_ButtonThickness * 0.5f;
	float sizeY = m_SizeY * 0.5f;

	m_OwnerColor = CharacterManager::get()->getPlayerCharacter().m_RegionColor;

	float positionX = m_Window->getSize().x - m_SizeX - m_OutlineThickness;
	float positionY = m_Window->getSize().y - m_SizeY - m_OutlineThickness;

	m_WindowShape.setFillColor(m_FillColor);
	m_WindowShape.setOutlineColor(m_OwnerColor);
	m_WindowShape.setOutlineThickness(m_OutlineThickness);
	m_WindowShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));
	m_WindowShape.setPosition(positionX, positionY);

	for (unsigned int index = 0; index < m_NumberOfButtons; index++)
	{
		m_ButtonShapes[index].setFillColor(sf::Color::Transparent);
		m_ButtonShapes[index].setOutlineColor(m_OwnerColor);
		m_ButtonShapes[index].setOutlineThickness(m_ButtonThickness * 0.25f);
		m_ButtonShapes[index].setSize(sf::Vector2f(sizeY + sizeX, sizeY + sizeX));
		m_ButtonShapes[index].setPosition(positionX + m_ButtonThickness * 1.25f + m_ButtonThickness * 4.5f * index, positionY + m_SizeY * 0.25f - m_ButtonThickness * 0.25f);
	}

	for (unsigned int index = 0; index < m_NumberOfSpeeds; index++)
	{
		m_SpeedShapes[index].setFillColor(sf::Color::Transparent);
		m_SpeedShapes[index].setOutlineColor(m_OwnerColor);
		m_SpeedShapes[index].setOutlineThickness(m_ButtonThickness * 0.25f);
		m_SpeedShapes[index].setSize(sf::Vector2f(sizeY * 0.5f + sizeX, sizeY + sizeX));
		m_SpeedShapes[index].setPosition(positionX + m_ButtonThickness * 14.5f + m_ButtonThickness * 2.5f * index, positionY + m_SizeY * 0.25f - m_ButtonThickness * 0.25f);
	}

	m_PauseLeftShape.setFillColor(m_OwnerColor);
	m_PauseLeftShape.setSize(sf::Vector2f(sizeX, sizeY));
	m_PauseLeftShape.setPosition(positionX + m_ButtonThickness * 2, positionY + m_SizeY * 0.25f);

	m_PauseRightShape.setFillColor(m_OwnerColor);
	m_PauseRightShape.setSize(sf::Vector2f(sizeX, sizeY));
	m_PauseRightShape.setPosition(positionX + m_ButtonThickness * 3, positionY + m_SizeY * 0.25f);

	m_DecreaseSpeedShape.setFillColor(m_OwnerColor);
	m_DecreaseSpeedShape.setSize(sf::Vector2f(sizeX, sizeY));
	m_DecreaseSpeedShape.setRotation(90.0f);
	m_DecreaseSpeedShape.setPosition(positionX + m_ButtonThickness * 8.5f, positionY + m_SizeY * 0.5f - m_ButtonThickness * 0.25f);

	m_IncreaseSpeedHorizontalShape.setFillColor(m_OwnerColor);
	m_IncreaseSpeedHorizontalShape.setSize(sf::Vector2f(sizeX, sizeY));
	m_IncreaseSpeedHorizontalShape.setRotation(90.0f);
	m_IncreaseSpeedHorizontalShape.setPosition(positionX + m_ButtonThickness * 13, positionY + m_SizeY * 0.5f - m_ButtonThickness * 0.25f);

	m_IncreaseSpeedVerticalShape.setFillColor(m_OwnerColor);
	m_IncreaseSpeedVerticalShape.setSize(sf::Vector2f(sizeX, sizeY));
	m_IncreaseSpeedVerticalShape.setPosition(positionX + m_ButtonThickness * 11.5f, positionY + m_SizeY * 0.25f);

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

void DateBar::onMonthChange()
{
	m_OwnerColor = CharacterManager::get()->getPlayerCharacter().m_RegionColor;

	m_WindowShape.setOutlineColor(m_OwnerColor);

	for (unsigned int index = 0; index < m_NumberOfButtons; index++)
	{
		m_ButtonShapes[index].setOutlineColor(m_OwnerColor);
	}

	for (unsigned int index = 0; index < m_NumberOfSpeeds; index++)
	{
		m_SpeedShapes[index].setOutlineColor(m_OwnerColor);
	}

	m_PauseLeftShape.setFillColor(m_OwnerColor);

	m_PauseRightShape.setFillColor(m_OwnerColor);

	m_DecreaseSpeedShape.setFillColor(m_OwnerColor);

	m_IncreaseSpeedHorizontalShape.setFillColor(m_OwnerColor);

	m_IncreaseSpeedVerticalShape.setFillColor(m_OwnerColor);

	m_DateText.setFillColor(m_OwnerColor);
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