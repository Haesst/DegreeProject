#pragma once

#include <SFML/Graphics.hpp>
#include "Engine/Window.h"
#include "Engine/Time.h"

const static unsigned int NUMBER_OF_SPEEDS = 5;
const static unsigned int NUMBER_OF_BUTTONS = 3;

#pragma warning(push)
#pragma warning(disable: 26812)
struct DateBar
{
	sf::RectangleShape m_WindowShape;
	sf::RectangleShape m_DecreaseSpeedShape;
	sf::RectangleShape m_IncreaseSpeedVerticalShape;
	sf::RectangleShape m_IncreaseSpeedHorizontalShape;
	sf::RectangleShape m_PauseLeftShape;
	sf::RectangleShape m_PauseRightShape;
	sf::RectangleShape m_ButtonShapes[NUMBER_OF_BUTTONS];
	sf::RectangleShape m_SpeedShapes[NUMBER_OF_SPEEDS];
	unsigned int m_NumberOfSpeeds = NUMBER_OF_SPEEDS;
	unsigned int m_NumberOfButtons = NUMBER_OF_BUTTONS;
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Red;
	sf::Color m_SpeedColor = sf::Color::Green;
	float m_SizeX = 600.0f;
	float m_SizeY = 50.0f;
	float m_OutlineThickness = 5.0f;
	float m_ButtonThickness = 10.0f;
	sf::Text m_DateText;
	sf::Font m_Font;
	int m_CharacterSize = 30;
	sf::Text::Style m_Style = sf::Text::Regular;
	sf::RenderWindow* m_Window = nullptr;
	unsigned int m_CurrentSpeedLevel = 3;

	UIID m_OwnedUIWindow = INVALID_UI_ID;

	DateBar(UIID id, sf::Font font, Vector2D, Vector2D size)
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

	void start()
	{
		Time::m_GameDate.subscribeToDayChange([](void* data) { DateBar& datebar = *static_cast<DateBar*>(data); datebar.onDayChanged(); }, static_cast<void*>(this));

		m_Window = Window::getWindow();

		updateStats();

		float sizeX = m_ButtonThickness * 0.5f;
		float sizeY = m_SizeY * 0.5f;

		m_OwnerColor = CharacterManager::get()->getPlayerCharacter().m_RegionColor;

		m_WindowShape.setFillColor(m_FillColor);
		m_WindowShape.setOutlineColor(m_OwnerColor);
		m_WindowShape.setOutlineThickness(m_OutlineThickness);
		m_WindowShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));

		for (unsigned int index = 0; index < m_NumberOfButtons; index++)
		{
			m_ButtonShapes[index].setFillColor(sf::Color::Transparent);
			m_ButtonShapes[index].setOutlineColor(m_OwnerColor);
			m_ButtonShapes[index].setOutlineThickness(m_ButtonThickness * 0.25f);
			m_ButtonShapes[index].setSize(sf::Vector2f(sizeY + sizeX, sizeY + sizeX));
		}

		for (unsigned int index = 0; index < m_NumberOfSpeeds; index++)
		{
			m_SpeedShapes[index].setFillColor(sf::Color::Transparent);
			m_SpeedShapes[index].setOutlineColor(m_OwnerColor);
			m_SpeedShapes[index].setOutlineThickness(m_ButtonThickness * 0.25f);
			m_SpeedShapes[index].setSize(sf::Vector2f(sizeY * 0.5f + sizeX, sizeY + sizeX));
		}

		m_PauseLeftShape.setFillColor(m_OwnerColor);
		m_PauseLeftShape.setSize(sf::Vector2f(sizeX, sizeY));

		m_PauseRightShape.setFillColor(m_OwnerColor);
		m_PauseRightShape.setSize(sf::Vector2f(sizeX, sizeY));

		m_DecreaseSpeedShape.setFillColor(m_OwnerColor);
		m_DecreaseSpeedShape.setSize(sf::Vector2f(sizeX, sizeY));
		m_DecreaseSpeedShape.setRotation(90.0f);

		m_IncreaseSpeedHorizontalShape.setFillColor(m_OwnerColor);
		m_IncreaseSpeedHorizontalShape.setSize(sf::Vector2f(sizeX, sizeY));
		m_IncreaseSpeedHorizontalShape.setRotation(90.0f);

		m_IncreaseSpeedVerticalShape.setFillColor(m_OwnerColor);
		m_IncreaseSpeedVerticalShape.setSize(sf::Vector2f(sizeX, sizeY));

		m_DateText.setFont(m_Font);
		m_DateText.setCharacterSize(m_CharacterSize);
		m_DateText.setStyle(m_Style);
		m_DateText.setFillColor(m_OwnerColor);
	}

	void update()
	{
		clickButton();

		int positionX = m_Window->getSize().x - (int)(m_SizeX + m_OutlineThickness);
		int positionY = m_Window->getSize().y - (int)(m_SizeY + m_OutlineThickness);

		m_WindowShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX, positionY)));

		for (unsigned int index = 0; index < m_NumberOfButtons; index++)
		{
			m_ButtonShapes[index].setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_ButtonThickness * 1.25f + m_ButtonThickness * 4.5f * index), positionY + (int)(m_SizeY * 0.25f - m_ButtonThickness * 0.25f))));
		}

		for (unsigned int index = 0; index < m_NumberOfSpeeds; index++)
		{
			m_SpeedShapes[index].setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_ButtonThickness * 14.5f + m_ButtonThickness * 2.5f * index), positionY + (int)(m_SizeY * 0.25f - m_ButtonThickness * 0.25f))));
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

		m_PauseLeftShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_ButtonThickness * 2), positionY + (int)(m_SizeY * 0.25f))));

		m_PauseRightShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_ButtonThickness * 3), positionY + (int)(m_SizeY * 0.25f))));

		m_DecreaseSpeedShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_ButtonThickness * 8.5f), positionY + (int)(m_SizeY * 0.5f - m_ButtonThickness * 0.25f))));

		m_IncreaseSpeedHorizontalShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_ButtonThickness * 13), positionY + (int)(m_SizeY * 0.5f - m_ButtonThickness * 0.25f))));

		m_IncreaseSpeedVerticalShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_ButtonThickness * 11.5f), positionY + (int)(m_SizeY * 0.25f))));

		m_DateText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(m_SizeX * 0.475f), positionY + (int)(m_OutlineThickness * 0.5f))));
	}

	void render()
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

	void onDayChanged()
	{
		updateStats();
	}

	void updateStats()
	{
		m_DateText.setString(Time::m_GameDate.getDateString());
		m_CurrentSpeedLevel = Time::m_CurrentSpeedLevel;
	}

	void clickButton()
	{
		if (InputHandler::getLeftMouseReleased())
		{
			Vector2D mousePosition = InputHandler::getMousePosition();
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
	}
};
#pragma warning(pop)