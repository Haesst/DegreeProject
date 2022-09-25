#include <Game/UI/DateBar.h>
#include <SFML/Graphics.hpp>
#include <Engine/Window.h>
#include <Engine/Time.h>
#include <Game/GameDate.h>
#include <Game/Systems/CharacterManager.h>
#include <Engine/InputHandler.h>
#include <Engine/AssetHandler.h>

DateBar::DateBar(UIID id, sf::Font font, Vector2D position, Vector2D size) : UIWindow(id, font, position, size)
{
	m_Visible = true;
	m_CharacterSize = 30;
	m_OutlineThickness = 5.0f;
}

void DateBar::start()
{
	UIWindow::start();

	AssetHandler assetHandler = AssetHandler::get();
	float sizeX = m_ButtonThickness * 0.5f;
	float sizeY = m_SizeY * 0.5f;
	m_OutlineColor = CharacterManager::get().getPlayerCharacter().m_RegionColor;
	m_TextFillColor = m_OutlineColor;
	float positionX = m_Window->getSize().x - m_SizeX - m_OutlineThickness;
	float positionY = m_Window->getSize().y - m_SizeY - m_OutlineThickness;
	setShape(m_WindowShape, m_WindowFillColor, m_OutlineColor, m_OutlineThickness, { m_SizeX, m_SizeY }, { positionX, positionY });
	sf::Vector2f buttonSize = { sizeY + sizeX, sizeY + sizeX };
	for (unsigned int index = 0; index < m_NumberOfButtons; index++)
	{
		sf::Vector2f buttonPosition = { positionX + m_ButtonThickness * 1.25f + m_ButtonThickness * 4.5f * index, positionY + m_SizeY * 0.25f - m_ButtonThickness * 0.25f };
		m_Buttons.push_back(new Button(m_Font, m_Window, buttonPosition, buttonSize));
		if (index == 0)
		{
			m_Buttons[index]->fetchTexture("Assets/Graphics/UI/PauseButton.png");
			m_Buttons[index]->fetchTexture("Assets/Graphics/UI/PlayButton.png", true);
		}
		else if (index == 1)
		{
			m_Buttons[index]->fetchTexture("Assets/Graphics/UI/MinusButton.png");
		}
		else
		{
			m_Buttons[index]->fetchTexture("Assets/Graphics/UI/PlusButton.png");
		}
		m_Buttons[index]->setShape(m_TransparentColor, m_OutlineColor, m_ButtonThickness * 0.25f, buttonSize, buttonPosition);
		m_Buttons[index]->setSprite(m_Buttons[index]->m_Texture, buttonPosition);
	}
	sf::Vector2f speedSize = { sizeY * 0.5f + sizeX, sizeY + sizeX };
	for (unsigned int index = m_NumberOfButtons; index < m_NumberOfButtons + m_NumberOfSpeeds; index++)
	{
		sf::Vector2f speedPosition = { positionX + m_ButtonThickness * 14.5f + m_ButtonThickness * 2.5f * (index - m_NumberOfButtons), positionY + m_SizeY * 0.25f - m_ButtonThickness * 0.25f };
		m_Buttons.push_back(new Button(m_Font, m_Window, speedPosition, speedSize));
		m_Buttons[index]->setShape(m_TransparentColor, m_OutlineColor, m_ButtonThickness * 0.25f, speedSize, speedPosition);
	}
	m_Texts.push_back(sf::Text());
	setText(m_Texts[0], m_Font, m_CharacterSize, m_TextFillColor, { positionX + m_SizeX * 0.475f, positionY + m_OutlineThickness * 0.5f });


	m_DaySubscriptionHandle = Time::m_GameDate.subscribeToDayChange([](void* data) { DateBar& datebar = *static_cast<DateBar*>(data); datebar.onDayChange(); }, static_cast<void*>(this));
	updateStats();
	updateSpeedShapes();
}

void DateBar::update()
{
	UIWindow::update();
	
	clickButton();
}

void DateBar::render()
{
	UIWindow::render();
}

void DateBar::onDayChange()
{
	updateStats();
}

void DateBar::updateStats()
{
	for (sf::Text& text : m_Texts)
	{
		text.setString(Time::m_GameDate.getDateString());
	}
	m_CurrentSpeedLevel = Time::m_CurrentSpeedLevel;
}

void DateBar::clickButton()
{
	UIWindow::clickButton();

	if (InputHandler::getLeftMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		for (unsigned int index = 0; index < m_NumberOfButtons; index++)
		{
			if (m_Buttons[index]->m_Shape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				InputHandler::setLeftMouseReleased(false);
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
			if (m_Paused != Time::gamePaused() || m_CurrentSpeedLevel != (unsigned int)Time::m_CurrentSpeedLevel)
			{
				break;
			}
		}
		for (unsigned int index = m_NumberOfButtons; index < m_NumberOfButtons + m_NumberOfSpeeds; index++)
		{
			if (m_Buttons[index]->m_Shape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				InputHandler::setLeftMouseReleased(false);
				Time::setGameSpeed(index - m_NumberOfButtons + 1);
				break;
			}
		}
		if (m_WindowShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
		}
	}
	if (m_Paused != Time::gamePaused() || m_CurrentSpeedLevel != (unsigned int)Time::m_CurrentSpeedLevel)
	{
		updateSpeedShapes();
	}
}

void DateBar::updateSpeedShapes()
{
	m_CurrentSpeedLevel = Time::m_CurrentSpeedLevel;
	for (unsigned int index = m_NumberOfButtons; index < m_NumberOfButtons + m_NumberOfSpeeds; index++)
	{
		if (index - m_NumberOfButtons < m_CurrentSpeedLevel && Time::gamePaused())
		{
			m_Buttons[index]->m_Shape.setFillColor(m_PauseSpeedColor);
			if (m_Paused != Time::gamePaused())
			{
				m_Buttons.front()->setSprite(m_Buttons.front()->m_Texture, m_Buttons.front()->m_Shape.getPosition());
			}
		}
		else if (index - m_NumberOfButtons < m_CurrentSpeedLevel)
		{
			m_Buttons[index]->m_Shape.setFillColor(m_PlaySpeedColor);
			if (m_Paused != Time::gamePaused())
			{
				m_Buttons.front()->setSprite(m_Buttons.front()->m_AlternativeTexture, m_Buttons.front()->m_Shape.getPosition());
			}
		}
		else
		{
			m_Buttons[index]->m_Shape.setFillColor(m_TransparentColor);
		}
	}
	m_Paused = Time::gamePaused();
}

void DateBar::updateOwnerColor(sf::Color& newColor)
{
	m_OutlineColor = newColor;
	m_TextFillColor = m_OutlineColor;
	m_WindowShape.setOutlineColor(m_OutlineColor);
	for (sf::Text& text : m_Texts)
	{
		text.setFillColor(m_TextFillColor);
	}
	for (Button* button : m_Buttons)
	{
		button->m_Shape.setFillColor(m_TextFillColor);
	}
}
