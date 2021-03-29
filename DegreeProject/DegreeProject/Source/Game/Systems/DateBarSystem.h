#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Window.h"
#include "Engine/Time.h"
#include "Game/Components/DateBar.h"

struct DateBarSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	sf::RenderWindow* m_Window = nullptr;
	unsigned int m_CurrentSpeedLevel = 3;

	DateBarSystem()
	{
		addComponentSignature<DateBar>();
		m_EntityManager = &EntityManager::get();
		m_Window = Window::getWindow();
	}

	~DateBarSystem() {}

	virtual void start() override
	{
#pragma warning(push)
#pragma warning(disable: 26815)
		CharacterSystem* characterSystem = (CharacterSystem*)m_EntityManager->getSystem<CharacterSystem>().get();
#pragma warning(pop)

		CharacterComponent& playerCharacter = m_EntityManager->getComponent<CharacterComponent>(characterSystem->getPlayerID());

		DateBar* dateBars = m_EntityManager->getComponentArray<DateBar>();
		for (auto entity : m_Entities)
		{
			updateStats(dateBars[entity]);

			float sizeX = dateBars[entity].m_ButtonThickness * 0.5f;
			float sizeY = dateBars[entity].m_SizeY * 0.5f;

			dateBars[entity].m_OwnerColor = playerCharacter.m_RegionColor;

			dateBars[entity].m_Shape.setFillColor(dateBars[entity].m_FillColor);
			dateBars[entity].m_Shape.setOutlineColor(dateBars[entity].m_OwnerColor);
			dateBars[entity].m_Shape.setOutlineThickness(dateBars[entity].m_OutlineThickness);
			dateBars[entity].m_Shape.setSize(sf::Vector2f(dateBars[entity].m_SizeX, dateBars[entity].m_SizeY));

			for (unsigned int index = 0; index < dateBars[entity].m_NumberOfButtons; index++)
			{
				dateBars[entity].m_ButtonShapes[index].setFillColor(sf::Color::Transparent);
				dateBars[entity].m_ButtonShapes[index].setOutlineColor(dateBars[entity].m_OwnerColor);
				dateBars[entity].m_ButtonShapes[index].setOutlineThickness(dateBars[entity].m_ButtonThickness * 0.25f);
				dateBars[entity].m_ButtonShapes[index].setSize(sf::Vector2f(sizeY + sizeX, sizeY + sizeX));
			}

			for (unsigned int index = 0; index < dateBars[entity].m_NumberOfSpeeds; index++)
			{
				dateBars[entity].m_SpeedShapes[index].setFillColor(sf::Color::Transparent);
				dateBars[entity].m_SpeedShapes[index].setOutlineColor(dateBars[entity].m_OwnerColor);
				dateBars[entity].m_SpeedShapes[index].setOutlineThickness(dateBars[entity].m_ButtonThickness * 0.25f);
				dateBars[entity].m_SpeedShapes[index].setSize(sf::Vector2f(sizeY * 0.5f + sizeX, sizeY + sizeX));
			}

			dateBars[entity].m_PauseLeftShape.setFillColor(dateBars[entity].m_OwnerColor);
			dateBars[entity].m_PauseLeftShape.setSize(sf::Vector2f(sizeX, sizeY));

			dateBars[entity].m_PauseRightShape.setFillColor(dateBars[entity].m_OwnerColor);
			dateBars[entity].m_PauseRightShape.setSize(sf::Vector2f(sizeX, sizeY));

			dateBars[entity].m_DecreaseSpeedShape.setFillColor(dateBars[entity].m_OwnerColor);
			dateBars[entity].m_DecreaseSpeedShape.setSize(sf::Vector2f(sizeX, sizeY));
			dateBars[entity].m_DecreaseSpeedShape.setRotation(90.0f);

			dateBars[entity].m_IncreaseSpeedHorizontalShape.setFillColor(dateBars[entity].m_OwnerColor);
			dateBars[entity].m_IncreaseSpeedHorizontalShape.setSize(sf::Vector2f(sizeX, sizeY));
			dateBars[entity].m_IncreaseSpeedHorizontalShape.setRotation(90.0f);

			dateBars[entity].m_IncreaseSpeedVerticalShape.setFillColor(dateBars[entity].m_OwnerColor);
			dateBars[entity].m_IncreaseSpeedVerticalShape.setSize(sf::Vector2f(sizeX, sizeY));

			dateBars[entity].m_DateText.setFont(dateBars[entity].m_Font);
			dateBars[entity].m_DateText.setCharacterSize(dateBars[entity].m_CharacterSize);
			dateBars[entity].m_DateText.setStyle(dateBars[entity].m_Style);
			dateBars[entity].m_DateText.setFillColor(dateBars[entity].m_OwnerColor);
		}
	}

	virtual void update() override
	{
		DateBar* dateBars = m_EntityManager->getComponentArray<DateBar>();

		for (auto entity : m_Entities)
		{
			updateStats(dateBars[entity]);
			clickButton(dateBars[entity]);

			int positionX = m_Window->getSize().x - (int)(dateBars[entity].m_SizeX + dateBars[entity].m_OutlineThickness);
			int positionY = m_Window->getSize().y - (int)(dateBars[entity].m_SizeY + dateBars[entity].m_OutlineThickness);

			dateBars[entity].m_Shape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX, positionY)));

			for (unsigned int index = 0; index < dateBars[entity].m_NumberOfButtons; index++)
			{
				dateBars[entity].m_ButtonShapes[index].setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(dateBars[entity].m_ButtonThickness * 1.25f + dateBars[entity].m_ButtonThickness * 4.5f * index), positionY + (int)(dateBars[entity].m_SizeY * 0.25f - dateBars[entity].m_ButtonThickness * 0.25f))));
			}

			for (unsigned int index = 0; index < dateBars[entity].m_NumberOfSpeeds; index++)
			{
				dateBars[entity].m_SpeedShapes[index].setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(dateBars[entity].m_ButtonThickness * 14.5f + dateBars[entity].m_ButtonThickness * 2.5f * index), positionY + (int)(dateBars[entity].m_SizeY * 0.25f - dateBars[entity].m_ButtonThickness * 0.25f))));
				if (index < m_CurrentSpeedLevel && Time::gamePaused())
				{
					dateBars[entity].m_SpeedShapes[index].setFillColor(sf::Color::Red);
				}
				else if (index < m_CurrentSpeedLevel)
				{
					dateBars[entity].m_SpeedShapes[index].setFillColor(dateBars[entity].m_SpeedColor);
				}
				else
				{
					dateBars[entity].m_SpeedShapes[index].setFillColor(sf::Color::Transparent);
				}
			}

			dateBars[entity].m_PauseLeftShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(dateBars[entity].m_ButtonThickness * 2), positionY + (int)(dateBars[entity].m_SizeY * 0.25f))));

			dateBars[entity].m_PauseRightShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(dateBars[entity].m_ButtonThickness * 3), positionY + (int)(dateBars[entity].m_SizeY * 0.25f))));

			dateBars[entity].m_DecreaseSpeedShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(dateBars[entity].m_ButtonThickness * 8.5f), positionY + (int)(dateBars[entity].m_SizeY * 0.5f - dateBars[entity].m_ButtonThickness * 0.25f))));

			dateBars[entity].m_IncreaseSpeedHorizontalShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(dateBars[entity].m_ButtonThickness * 13), positionY + (int)(dateBars[entity].m_SizeY * 0.5f - dateBars[entity].m_ButtonThickness * 0.25f))));
			
			dateBars[entity].m_IncreaseSpeedVerticalShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(dateBars[entity].m_ButtonThickness * 11.5f), positionY + (int)(dateBars[entity].m_SizeY * 0.25f))));

			dateBars[entity].m_DateText.setString(dateBars[entity].m_Date);
			dateBars[entity].m_DateText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(positionX + (int)(dateBars[entity].m_SizeX * 0.475f), positionY + (int)(dateBars[entity].m_OutlineThickness * 0.5f))));
		}
	}
	
	virtual void render() override
	{
		DateBar* dateBars = m_EntityManager->getComponentArray<DateBar>();

		for (auto entity : m_Entities)
		{
			m_Window->draw(dateBars[entity].m_Shape);
			for (unsigned int index = 0; index < dateBars[entity].m_NumberOfButtons; index++)
			{
				m_Window->draw(dateBars[entity].m_ButtonShapes[index]);
			}
			for (unsigned int index = 0; index < dateBars[entity].m_NumberOfSpeeds; index++)
			{
				m_Window->draw(dateBars[entity].m_SpeedShapes[index]);
			}
			m_Window->draw(dateBars[entity].m_PauseLeftShape);
			m_Window->draw(dateBars[entity].m_PauseRightShape);
			m_Window->draw(dateBars[entity].m_DecreaseSpeedShape);
			m_Window->draw(dateBars[entity].m_IncreaseSpeedHorizontalShape);
			m_Window->draw(dateBars[entity].m_IncreaseSpeedVerticalShape);
			m_Window->draw(dateBars[entity].m_DateText);
		}
	}

	void updateStats(DateBar& dateBar)
	{
		dateBar.m_Date = Time::m_GameDate.getDateString();
		m_CurrentSpeedLevel = Time::m_CurrentSpeedLevel;
	}

	void clickButton(DateBar& dateBar)
	{
		if (InputHandler::getLeftMouseReleased())
		{
			Vector2D mousePosition = InputHandler::getMousePosition();
			for (unsigned int index = 0; index < dateBar.m_NumberOfButtons; index++)
			{
				if (dateBar.m_ButtonShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
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