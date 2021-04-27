#include "Game/UI/WarIcon.h"
#include "Engine/Window.h"
#include "Engine/AssetHandler.h"
#include "Engine/InputHandler.h"
#include "Game/UI/UIManager.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/UI/WarWindow.h"
#include "Game/DiplomacyManager.h"
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

	sf::Vector2f warIconPosition = { m_Window->getSize().x - 600 - m_SpriteSize - m_OutlineThickness * 5 - (m_SpriteSize + m_OutlineThickness * 4) * m_Index, m_Window->getSize().y - m_SpriteSize - m_OutlineThickness * 2 };
	setShape(m_WarIconShape, m_WarIconOutlineColor, m_WarIconOutlineColor, m_OutlineThickness, { m_SizeX, m_SizeY }, warIconPosition);
	setSprite(m_WarIconSprite, m_WarIconTexture, warIconPosition);
	setText(m_WarscoreText, m_Font, m_CharacterSize, { warIconPosition.x, warIconPosition.y + m_SizeY * 0.5f });

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

void WarIcon::updatePosition(unsigned int index)
{
	m_Index = index;
	sf::Vector2f warIconPosition = { m_Window->getSize().x - 600 - m_SpriteSize - m_OutlineThickness * 5 - (m_SpriteSize + m_OutlineThickness * 4) * m_Index, m_Window->getSize().y - m_SpriteSize - m_OutlineThickness * 2 };
	m_WarIconShape.setPosition(warIconPosition);
	m_WarIconSprite.setPosition(warIconPosition);
	m_WarscoreText.setPosition({ warIconPosition.x, warIconPosition.y + m_SizeY * 0.5f });
}

void WarIcon::setWarscore(CharacterID& characterID, std::stringstream& stream)
{
	DiplomacyManager& warManager = DiplomacyManager::get();

	int warscore = warManager.getWarscore(m_WarHandle, characterID);
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
}

void WarIcon::updateInfo()
{
	m_WarHandle = DiplomacyManager::get().getWarHandleAgainst(m_AttackerID, m_DefenderID);
	if (m_WarHandle != -1)
	{
		std::stringstream stream;
		CharacterID playerCharacterID = CharacterManager::get().getPlayerCharacterID();
		if (m_AttackerID == playerCharacterID)
		{
			setWarscore(m_AttackerID, stream);
		}
		else if (m_DefenderID == playerCharacterID)
		{
			setWarscore(m_DefenderID, stream);
		}
		else
		{
			setWarscore(m_AttackerID, stream);
		}
		stream << m_PercentSign;
		m_WarscoreText.setString(stream.str());
		stream.str(std::string());
		stream.clear();
	}
	else if (m_DaySubscriptionHandle != -1)
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
		m_Window->draw(m_WarIconSprite);
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

void WarIcon::setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position)
{
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThickness);
	shape.setSize(size);
	shape.setPosition(position);
}

void WarIcon::setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Vector2f position)
{
	text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setPosition(position);
}

void WarIcon::setSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position)
{
	sprite.setTexture(texture, true);
	sprite.setScale(m_SpriteSize / sprite.getGlobalBounds().width, m_SpriteSize / sprite.getGlobalBounds().height);
	sprite.setPosition(position);
}
