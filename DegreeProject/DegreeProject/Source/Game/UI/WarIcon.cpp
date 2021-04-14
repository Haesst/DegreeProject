#include "Game/UI/WarIcon.h"
#include "Engine/Window.h"
#include "Engine/AssetHandler.h"
#include "Engine/InputHandler.h"
#include "Game/UI/UIManager.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/UI/WarWindow.h"
#include "Game/WarManager.h"
#include "Time.h"
#include <sstream>

WarIcon::WarIcon(UIID ID, sf::Font font, unsigned int index, CharacterID attackerID, CharacterID defenderID)
{
	m_DaySubscriptionHandle = Time::m_GameDate.subscribeToDayChange([](void* data) { WarIcon& warIcon = *static_cast<WarIcon*>(data); warIcon.onDayChange(); }, static_cast<void*>(this));

	m_OwnerUIElement = ID;
	m_Font = font;
	m_Index = index;
	m_AttackerID = attackerID;
	m_DefenderID = defenderID;
	m_WarStartDate = Time::m_GameDate.m_Date;

	m_Window = Window::getWindow();

	m_WarIconPosition = Vector2DInt(m_Window->getSize().x - (int)(600 + m_SpriteSize + m_OutlineThickness * 5) - (m_SpriteSize + m_OutlineThickness * 4) * m_Index, m_Window->getSize().y - (int)(m_SpriteSize + m_OutlineThickness * 2));
	m_WarIconTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Charizard.png");

	m_WarIconShape.setFillColor(m_WarIconOutlineColor);
	m_WarIconShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));
	m_WarIconShape.setOutlineColor(m_WarIconOutlineColor);
	m_WarIconShape.setOutlineThickness(m_OutlineThickness);

	m_WarscoreText.setCharacterSize(m_CharacterSize);
	m_WarscoreText.setFont(m_Font);
	m_WarscoreText.setStyle(m_Style);
	m_WarscoreTextPosition = Vector2DInt(m_WarIconPosition.x, m_WarIconPosition.y + m_SizeY * 0.5f);

	activate();
}

void WarIcon::activate()
{
	m_Active = true;
	updateInfo();
}

void WarIcon::deactivate()
{
	m_WarIconShape.setSize(sf::Vector2f());
	Time::m_GameDate.unsubscribeToDayChange(m_DaySubscriptionHandle);
	m_Active = false;
}

void WarIcon::onDayChange()
{
	if (m_Active)
	{
		updateInfo();
	}
}

void WarIcon::updateInfo()
{
	m_War = WarManager::get().getWarAgainst(m_AttackerID, m_DefenderID);
	if (m_War != nullptr)
	{
		std::stringstream stream;
		CharacterID playerCharacterID = CharacterManager::get()->getPlayerCharacterID();
		if (m_AttackerID == playerCharacterID)
		{
			int warscore = m_War->getWarscore(m_AttackerID);
			if (warscore > 0 || warscore == 0)
			{
				m_WarscoreText.setFillColor(m_PositiveColor);
				stream << m_PositiveSign << warscore;
			}
			else
			{
				m_WarscoreText.setFillColor(m_NegativeColor);
				stream << warscore;
			}
		}
		else if (m_DefenderID == playerCharacterID)
		{
			int warscore = m_War->getWarscore(m_DefenderID);
			if (warscore > 0 || warscore == 0)
			{
				m_WarscoreText.setFillColor(m_PositiveColor);
				stream << m_PositiveSign << warscore;
			}
			else
			{
				m_WarscoreText.setFillColor(m_NegativeColor);
				stream << warscore;
			}
		}
		stream << m_PercentSign;
		m_WarscoreText.setString(stream.str());
		stream.str(std::string());
		stream.clear();
	}
	else
	{
		deactivate();
	}
}

void WarIcon::update()
{
	if (m_Active)
	{
		clickButton();
		m_WarIconShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_WarIconPosition.x, m_WarIconPosition.y)));
		m_WarscoreText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_WarscoreTextPosition.x, m_WarscoreTextPosition.y)));
	}
}

void WarIcon::render()
{
	if (m_Active)
	{
		m_Window->draw(m_WarIconShape);
		updateSprite(m_WarIconSprite, m_WarIconTexture, m_WarIconPosition);
		m_Window->draw(m_WarscoreText);
	}
}

void WarIcon::clickButton()
{
	if (InputHandler::getLeftMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getMousePosition();
		if (m_WarIconShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			UIManager::get()->m_WarWindow->openWindow(m_AttackerID, m_DefenderID, m_WarStartDate);
		}
	}
}

void WarIcon::updateSprite(sf::Sprite& sprite, sf::Texture& texture, Vector2DInt position, unsigned int spriteSize)
{
	sprite.setTexture(texture, true);
	sprite.setPosition(Window::getWindow()->mapPixelToCoords(sf::Vector2i(position.x, position.y)));

	sf::FloatRect localSize = sprite.getLocalBounds();

	sprite.setScale(spriteSize / localSize.width, spriteSize / localSize.height);

	m_Window->draw(sprite);
}
