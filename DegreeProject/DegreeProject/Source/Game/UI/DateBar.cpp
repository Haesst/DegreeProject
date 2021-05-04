#include "Game/UI/DateBar.h"
#include <SFML/Graphics.hpp>
#include "Engine/Window.h"
#include "Engine/Time.h"
#include "Game/GameDate.h"
#include "Game/Systems/CharacterManager.h"
#include "Engine/InputHandler.h"
#include "Engine/AssetHandler.h"

DateBar::DateBar(UIID id, sf::Font font, Vector2D, Vector2D size)
{
	m_OwnedUIWindow = id;
	m_Font = font;
	m_SizeX = size.x;
	m_SizeY = size.y;

	m_Window = Window::getWindow();

	AssetHandler assetHandler = AssetHandler::get();

	float sizeX = m_ButtonThickness * 0.5f;
	float sizeY = m_SizeY * 0.5f;

	m_OwnerColor = CharacterManager::get().getPlayerCharacter().m_RegionColor;

	float positionX = m_Window->getSize().x - m_SizeX - m_OutlineThickness;
	float positionY = m_Window->getSize().y - m_SizeY - m_OutlineThickness;

	sf::Vector2f position = { positionX, positionY };
	setShape(m_WindowShape, m_FillColor, m_OwnerColor, m_OutlineThickness, { m_SizeX, m_SizeY }, position);

	sf::Vector2f buttonSize = { sizeY + sizeX, sizeY + sizeX };
	for (unsigned int index = 0; index < m_NumberOfButtons; index++)
	{
		m_ButtonShapes.push_back(sf::RectangleShape());
		sf::Vector2f buttonPosition = { positionX + m_ButtonThickness * 1.25f + m_ButtonThickness * 4.5f * index, positionY + m_SizeY * 0.25f - m_ButtonThickness * 0.25f };
		setShape(m_ButtonShapes[index], m_TransparentColor, m_OwnerColor, m_ButtonThickness * 0.25f, buttonSize, buttonPosition);

		const char* path;
		if (index == 0)
		{
			path = "Assets/Graphics/UI/PauseButton.png";
		}
		else if (index == 1)
		{
			path = "Assets/Graphics/UI/MinusButton.png";
		}
		else if (index == 2)
		{
			path = "Assets/Graphics/UI/PlusButton.png";
		}
		m_ButtonTextures.push_back(assetHandler.getTextureAtPath(path));

		m_ButtonSprites.push_back(sf::Sprite());
	}
	m_ButtonTextures.push_back(assetHandler.getTextureAtPath("Assets/Graphics/UI/PlayButton.png"));
	
	for (unsigned int index = 0; index < m_NumberOfButtons; index++)
	{
		setSprite(m_ButtonSprites[index], m_ButtonTextures[index], m_ButtonShapes[index].getPosition());
	}

	sf::Vector2f speedSize = { sizeY * 0.5f + sizeX, sizeY + sizeX };
	for (unsigned int index = 0; index < m_NumberOfSpeeds; index++)
	{
		m_SpeedShapes.push_back(sf::RectangleShape());
		sf::Vector2f speedPosition = { positionX + m_ButtonThickness * 14.5f + m_ButtonThickness * 2.5f * index, positionY + m_SizeY * 0.25f - m_ButtonThickness * 0.25f };
		setShape(m_SpeedShapes[index], m_TransparentColor, m_OwnerColor, m_ButtonThickness * 0.25f, speedSize, speedPosition);
	}

	setText(m_DateText, m_Font, m_CharacterSize, m_OwnerColor, { positionX + m_SizeX * 0.475f, positionY + m_OutlineThickness * 0.5f });
}

void DateBar::start()
{
	m_DaySubscriptionHandle = Time::m_GameDate.subscribeToDayChange([](void* data) { DateBar& datebar = *static_cast<DateBar*>(data); datebar.onDayChange(); }, static_cast<void*>(this));
	updateStats();
	updateSpeedShapes();
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
		m_Window->draw(m_ButtonSprites[index]);
	}
	for (unsigned int index = 0; index < m_NumberOfSpeeds; index++)
	{
		m_Window->draw(m_SpeedShapes[index]);
	}
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
			if (m_Paused != Time::gamePaused() || m_CurrentSpeedLevel != Time::m_CurrentSpeedLevel)
			{
				break;
			}
		}
		for (unsigned int index = 0; index < m_NumberOfSpeeds; index++)
		{
			if (m_SpeedShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				InputHandler::setLeftMouseReleased(false);
				Time::setGameSpeed(index + 1);
				break;
			}
		}
		if (m_WindowShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
		}
	}
	if (m_Paused != Time::gamePaused() || m_CurrentSpeedLevel != Time::m_CurrentSpeedLevel)
	{
		updateSpeedShapes();
	}
}

void DateBar::updateSpeedShapes()
{
	m_CurrentSpeedLevel = Time::m_CurrentSpeedLevel;
	for (unsigned int index = 0; index < m_NumberOfSpeeds; index++)
	{
		if (index < m_CurrentSpeedLevel && Time::gamePaused())
		{
			m_SpeedShapes[index].setFillColor(m_PauseSpeedColor);
			if (m_Paused != Time::gamePaused())
			{
				setSprite(m_ButtonSprites.front(), m_ButtonTextures.back(), m_ButtonSprites.front().getPosition());
			}
		}
		else if (index < m_CurrentSpeedLevel)
		{
			m_SpeedShapes[index].setFillColor(m_PlaySpeedColor);
			if (m_Paused != Time::gamePaused())
			{
				setSprite(m_ButtonSprites.front(), m_ButtonTextures.front(), m_ButtonSprites.front().getPosition());
			}
		}
		else
		{
			m_SpeedShapes[index].setFillColor(m_TransparentColor);
		}
	}
	m_Paused = Time::gamePaused();
}

void DateBar::updateOwnerColor(sf::Color& newColor)
{
	m_OwnerColor = newColor;
	m_DateText.setFillColor(m_OwnerColor);
	m_WindowShape.setOutlineColor(m_OwnerColor);
	for (unsigned int index = 0; index < m_NumberOfButtons; index++)
	{
		m_ButtonShapes[index].setOutlineColor(m_OwnerColor);
	}

	for (unsigned int index = 0; index < m_NumberOfSpeeds; index++)
	{
		m_SpeedShapes[index].setOutlineColor(m_OwnerColor);
	}
}

void DateBar::setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position)
{
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThickness);
	shape.setSize(size);
	shape.setPosition(position);
}

void DateBar::setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position)
{
	text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setFillColor(fillColor);
	text.setPosition(position);
}

void DateBar::setSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, unsigned int spriteSize)
{
	sprite.setTexture(texture, true);
	sprite.setScale(spriteSize / sprite.getLocalBounds().width, spriteSize / sprite.getLocalBounds().height);
	sprite.setPosition(position);
}