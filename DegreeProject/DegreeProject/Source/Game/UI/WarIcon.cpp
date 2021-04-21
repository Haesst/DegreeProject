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
	m_OwnerUIElement = ID;
	m_Font = font;
	m_Index = index;
	m_AttackerID = attackerID;
	m_DefenderID = defenderID;
	m_WarStartDate = Time::m_GameDate.m_Date;

	m_Window = Window::getWindow();

	m_WarIconTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Charizard.png");

	m_WarIconPosition = sf::Vector2f(m_Window->getSize().x - 600 - m_SpriteSize - m_OutlineThickness * 5 - (m_SpriteSize + m_OutlineThickness * 4) * m_Index, m_Window->getSize().y - m_SpriteSize - m_OutlineThickness * 2);
	m_WarIconShape.setFillColor(m_WarIconOutlineColor);
	m_WarIconShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));
	m_WarIconShape.setOutlineColor(m_WarIconOutlineColor);
	m_WarIconShape.setOutlineThickness(m_OutlineThickness);
	m_WarIconShape.setPosition(m_WarIconPosition);

	m_WarscoreTextPosition = sf::Vector2f(m_WarIconPosition.x, m_WarIconPosition.y + m_SizeY * 0.5f);
	m_WarscoreText.setCharacterSize(m_CharacterSize);
	m_WarscoreText.setFont(m_Font);
	m_WarscoreText.setPosition(m_WarscoreTextPosition);

	activate();
}

void WarIcon::activate()
{
	m_Active = true;
	m_DaySubscriptionHandle = Time::m_GameDate.subscribeToDayChange([](void* data) { WarIcon& warIcon = *static_cast<WarIcon*>(data); warIcon.onDayChange(); }, static_cast<void*>(this));
	updateInfo();
}

void WarIcon::deactivate()
{
	m_WarIconShape.setSize(sf::Vector2f());
	Time::m_GameDate.unsubscribeToDayChange(m_DaySubscriptionHandle);
	m_DaySubscriptionHandle = -1;
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
		//CharacterID playerCharacterID = CharacterManager::get().getPlayerCharacterID();
		//if (m_AttackerID == playerCharacterID)
		//{
			int warscore = m_War->getWarscore(m_AttackerID);
			if (warscore > 100)
			{
				warscore = 100;
			}
			if (warscore < -100)
			{
				warscore = -100;
			}
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
			/*}
			else if (m_DefenderID == playerCharacterID)
			{
				int warscore = m_War->getWarscore(m_DefenderID);
				if (warscore > 100)
				{
					warscore = 100;
				}
				if (warscore < -100)
				{
					warscore = -100;
				}
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
			}*/
		stream << m_PercentSign;
		m_WarscoreText.setString(stream.str());
		stream.str(std::string());
		stream.clear();
	}
	else if(m_DaySubscriptionHandle > 0)
	{
		deactivate();
	}
}

void WarIcon::update()
{
	if (m_Active)
	{
		clickButton();
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
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (m_WarIconShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
			UIManager::get().m_WarWindow->openWindow(m_AttackerID, m_DefenderID, m_WarStartDate);
		}
	}
}

void WarIcon::updateSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, unsigned int spriteSize)
{
	sprite.setTexture(texture, true);
	sprite.setPosition(position);

	sf::FloatRect localSize = sprite.getLocalBounds();

	sprite.setScale(spriteSize / localSize.width, spriteSize / localSize.height);

	m_Window->draw(sprite);
}
