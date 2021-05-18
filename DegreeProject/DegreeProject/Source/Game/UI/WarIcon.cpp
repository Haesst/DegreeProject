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
#include "Game/Systems/HeraldicShieldManager.h"
#include "Game/Map/Map.h"
#include "Game/UI/CharacterWindow.h"
#include "Game/Game.h"

WarIcon::WarIcon(UIID ID, sf::Font font, unsigned int index, CharacterID attackerID, CharacterID defenderID)
{
	m_OwnerUIElement = ID;
	m_Font = font;
	m_Index = index;
	m_AttackerID = attackerID;
	m_DefenderID = defenderID;
	m_WarStartDate = Time::m_GameDate.m_Date;

	m_Window = Window::getWindow();

	sf::Vector2f warIconPosition = { m_Window->getSize().x - 600 - m_SpriteSize - m_OutlineThickness * 5 - (m_SpriteSize + m_OutlineThickness * 4) * m_Index, m_Window->getSize().y - m_SpriteSize - m_OutlineThickness * 2 };
	setShape(m_WarIconShape, m_WarIconOutlineColor, m_WarIconOutlineColor, m_OutlineThickness, { m_SizeX, m_SizeY }, warIconPosition);
	setText(m_WarscoreText, m_Font, m_CharacterSize, m_PositiveColor, m_TextOutlineColor, m_TextOutlineThickness, { warIconPosition.x + m_SizeX * 0.5f, warIconPosition.y + m_SizeY * 0.5f });

	activate();
}

void WarIcon::activate()
{
	m_Active = true;
	m_DaySubscriptionHandle = Time::m_GameDate.subscribeToDayChange([](void* data) { WarIcon& warIcon = *static_cast<WarIcon*>(data); warIcon.onDayChange(); }, static_cast<void*>(this));
	Game::m_Sound.pause();
	if (Game::m_BattleSound.getStatus() != sf::SoundSource::Playing)
	{
		Game::m_BattleSound.play();
	}
	updateInfo();
}

void WarIcon::deactivate()
{
	m_WarIconShape.setSize(sf::Vector2f());
	if (Game::m_BattleSound.getStatus() == sf::SoundSource::Playing && DiplomacyManager::get().getWarHandlesOfCharacter(CharacterManager::get().getPlayerCharacterID()).empty())
	{
		Game::m_BattleSound.stop();
		Game::m_Sound.play();
	}
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
	m_WarscoreText.setPosition({ warIconPosition.x + m_SizeX * 0.5f, warIconPosition.y + m_SizeY * 0.5f });
}

void WarIcon::setWarscore(int& warscore, std::stringstream& stream)
{
	if (warscore > 100)
	{
		warscore = 100;
	}
	else if (warscore < -100)
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
		DiplomacyManager& diplomacyManager = DiplomacyManager::get();
		War& war = *diplomacyManager.getWar(m_WarHandle);
		MapRegion& wargoalRegion = Map::get().getRegionById(war.m_WargoalRegion);
		m_HeraldicShield = &wargoalRegion.m_HeraldicShield;

		std::stringstream stream;
		CharacterID playerCharacterID = CharacterManager::get().getPlayerCharacterID();
		m_PlayerAttacker = diplomacyManager.isAttacker(m_WarHandle, playerCharacterID);
		if (m_PlayerAttacker)
		{
			int warscore = diplomacyManager.getWarscore(m_WarHandle, m_AttackerID);
			setWarscore(warscore, stream);
		}
		else
		{
			int warscore = diplomacyManager.getWarscore(m_WarHandle, m_DefenderID);
			setWarscore(warscore, stream);
		}
		stream << m_PercentSign;
		m_WarscoreText.setString(stream.str());
		stream.str(std::string());
		stream.clear();

		m_WarscoreText.setOrigin(m_WarscoreText.getLocalBounds().width * 0.5f, m_WarscoreText.getLocalBounds().height * 0.75f);
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
		HeraldicShieldManager::renderShield(*m_HeraldicShield, { m_WarIconShape.getPosition() }, m_HeraldicShieldScale);
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
	if (InputHandler::getRightMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (m_WarIconShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setRightMouseReleased(false);
			UIManager& uiManager = UIManager::get();
			CharacterWindow& characterWindow = *uiManager.m_CharacterWindow;
			if (m_PlayerAttacker)
			{
				characterWindow.m_CurrentCharacterID = m_DefenderID;
			}
			else
			{
				characterWindow.m_CurrentCharacterID = m_AttackerID;
			}
			characterWindow.checkIfPlayerCharacter();
			characterWindow.updateInfo();
			characterWindow.openWindow();
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

void WarIcon::setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f position)
{
	text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setFillColor(fillColor);
	text.setOutlineColor(outlineColor);
	text.setOutlineThickness(outlineThickness);
	text.setPosition(position);
}
