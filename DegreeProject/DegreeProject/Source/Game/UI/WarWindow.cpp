#include "Game/UI/WarWindow.h"
#include <string>
#include <vector>
#include "Engine/Time.h"
#include "Game/GameDate.h"
#include "Engine/Window.h"
#include "Engine/AssetHandler.h"
#include "Engine/InputHandler.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/Data/Character.h"
#include "Game/UI/UIManager.h"
#include "Game/UI/CharacterWindow.h"
#include <sstream>
#include "Game/WarManager.h"
#include "Game/Map/Map.h"
#include "Game/Game.h"

WarWindow::WarWindow(UIID id, sf::Font font, Vector2D, Vector2D size)
{
	m_OwnedUIWindow = id;
	m_Font = font;
	m_SizeX = size.x;
	m_SizeY = size.y;
}

void WarWindow::start()
{
	m_Window = Window::getWindow();

	AssetHandler& assethandler = AssetHandler::get();

	Character& playerCharacter = CharacterManager::get().getPlayerCharacter();

	m_MaleCharacterTexture = assethandler.getTextureAtPath("Assets/Graphics/MalePortrait.jpg");
	m_FemaleCharacterTexture = assethandler.getTextureAtPath("Assets/Graphics/FemalePortrait.jpg");
	m_YoungMaleCharacterTexture = assethandler.getTextureAtPath("Assets/Graphics/Unit.png");
	m_YoungFemaleCharacterTexture = assethandler.getTextureAtPath("Assets/Graphics/Harriet.jpg");

	m_ArmyTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/soldier unit.png");
	m_AttackerArmyPosition = sf::Vector2f(m_SpriteSize, m_SpriteSize);
	m_DefenderArmyPosition = sf::Vector2f(m_SizeX - m_SpriteSize, m_SpriteSize);

	m_PositionX = m_OutlineThickness;
	m_PositionY = m_Window->getSize().y - m_OutlineThickness - m_SizeY;

	m_OwnerColor = playerCharacter.m_RegionColor;

	m_WindowShape.setFillColor(m_FillColor);
	m_WindowShape.setOutlineThickness(m_OutlineThickness);
	m_WindowShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));
	m_WindowShape.setOutlineColor(m_OwnerColor);
	m_WindowShape.setPosition(m_PositionX, m_PositionY);

	m_AttackerPosition = sf::Vector2f(m_PositionX + m_SizeX - m_SpriteSize * 4, m_PositionY - m_SpriteSize * 4 - m_OutlineThickness * 2);

	m_DefenderPosition = sf::Vector2f(m_PositionX, m_PositionY - m_SpriteSize * 4 - m_OutlineThickness * 2);
	
	m_WarscorePosition = sf::Vector2f(m_PositionX + m_SizeX * 0.5f - (m_SizeX * 0.25f) * 0.5f, m_PositionY + m_SizeY * 0.4f);
	m_WarscoreShape.setFillColor(m_AttackerColor);
	m_WarscoreShape.setOutlineThickness(m_OutlineThickness);
	m_WarscoreShape.setSize(sf::Vector2f(m_SizeX / 4, m_SpriteSize));
	m_WarscoreShape.setOutlineColor(m_OwnerColor);
	m_WarscoreShape.setPosition(m_WarscorePosition.x, m_WarscorePosition.y);

	m_WarscoreProgressPosition = sf::Vector2f(m_PositionX + m_SizeX * 0.5f - (m_SizeX * 0.25f) * 0.5f, m_PositionY + m_SizeY * 0.4f);
	m_WarscoreProgressShape.setFillColor(m_DefenderColor);
	m_WarscoreProgressShape.setSize(sf::Vector2f((m_SizeX / 4) * 0.5f, m_SpriteSize));
	m_WarscoreProgressShape.setPosition(m_WarscoreProgressPosition.x, m_WarscoreProgressPosition.y);

	m_SurrenderPosition = sf::Vector2f(m_PositionX + m_SizeX * 0.5f - m_OutlineThickness - (m_SizeX / 12) * 1.5f, m_PositionY + m_SizeY * 0.4f + m_SpriteSize * 2);
	m_SurrenderShape.setFillColor(sf::Color::Transparent);
	m_SurrenderShape.setOutlineThickness(m_OutlineThickness);
	m_SurrenderShape.setSize(sf::Vector2f(m_SizeX / 12, m_SpriteSize));
	m_SurrenderShape.setOutlineColor(m_OwnerColor);
	m_SurrenderShape.setPosition(m_SurrenderPosition.x, m_SurrenderPosition.y);

	m_WhitePeacePosition = sf::Vector2f(m_PositionX + m_SizeX * 0.5f - (m_SizeX / 12) * 0.5f, m_PositionY + m_SizeY * 0.4f + m_SpriteSize * 2);
	m_WhitePeaceShape.setFillColor(sf::Color::Transparent);
	m_WhitePeaceShape.setOutlineThickness(m_OutlineThickness);
	m_WhitePeaceShape.setSize(sf::Vector2f(m_SizeX / 12, m_SpriteSize));
	m_WhitePeaceShape.setOutlineColor(m_OwnerColor);
	m_WhitePeaceShape.setPosition(m_WhitePeacePosition.x, m_WhitePeacePosition.y);

	m_EnforceDemandsPosition = sf::Vector2f(m_PositionX + m_SizeX * 0.5f + m_OutlineThickness + (m_SizeX / 12) * 0.5f, m_PositionY + m_SizeY * 0.4f + m_SpriteSize * 2);
	m_EnforceDemandsShape.setFillColor(sf::Color::Transparent);
	m_EnforceDemandsShape.setOutlineThickness(m_OutlineThickness);
	m_EnforceDemandsShape.setSize(sf::Vector2f(m_SizeX / 12, m_SpriteSize));
	m_EnforceDemandsShape.setOutlineColor(m_OwnerColor);
	m_EnforceDemandsShape.setPosition(m_EnforceDemandsPosition.x, m_EnforceDemandsPosition.y);

	m_SendPosition = sf::Vector2f(m_PositionX + m_SizeX * 0.5f - (m_SizeX / 12) * 0.5f, m_PositionY + m_SizeY * 0.4f + m_SpriteSize * 4);
	m_SendShape.setFillColor(sf::Color::Transparent);
	m_SendShape.setOutlineThickness(m_OutlineThickness);
	m_SendShape.setSize(sf::Vector2f(m_SizeX / 12, m_SpriteSize));
	m_SendShape.setOutlineColor(m_OwnerColor);
	m_SendShape.setPosition(m_SendPosition);

	m_WarNameTextPosition = sf::Vector2f(m_WarscorePosition.x, m_WarscorePosition.y - m_SpriteSize * 3.5f);
	m_WarNameText.setFont(m_Font);
	m_WarNameText.setCharacterSize(m_CharacterSize);
	m_WarNameText.setFillColor(m_TextColor);
	m_WarNameText.setPosition(m_WarNameTextPosition);

	m_DurationTextPosition = sf::Vector2f(m_WarscorePosition.x, m_WarscorePosition.y - m_SpriteSize * 2.5f);
	m_DurationText.setFont(m_Font);
	m_DurationText.setCharacterSize((int)(m_CharacterSize * 0.75f));
	m_DurationText.setFillColor(m_TextColor);
	m_DurationText.setPosition(m_DurationTextPosition);

	m_WarscoreTextPosition = sf::Vector2f(m_WarscorePosition.x, m_WarscorePosition.y - m_SpriteSize * 1.5f);
	m_WarscoreText.setFont(m_Font);
	m_WarscoreText.setCharacterSize(m_CharacterSize);
	m_WarscoreText.setFillColor(m_TextColor);
	m_WarscoreText.setString(m_WarScore);
	m_WarscoreText.setPosition(m_WarscoreTextPosition);

	m_WarscoreAmountTextPosition = sf::Vector2f(m_WarscorePosition.x + 110, m_WarscorePosition.y - m_SpriteSize * 1.5f);
	m_WarscoreAmountText.setFont(m_Font);
	m_WarscoreAmountText.setCharacterSize(m_CharacterSize);
	m_WarscoreAmountText.setPosition(m_WarscoreAmountTextPosition);

	m_SurrenderTextPosition = sf::Vector2f(m_SurrenderPosition);
	m_SurrenderText.setFont(m_Font);
	m_SurrenderText.setCharacterSize(m_CharacterSize);
	m_SurrenderText.setString(m_Surrender);
	m_SurrenderText.setFillColor(m_TextColor);
	m_SurrenderText.setPosition(m_SurrenderTextPosition);

	m_WhitePeaceTextPosition = sf::Vector2f(m_WhitePeacePosition);
	m_WhitePeaceText.setFont(m_Font);
	m_WhitePeaceText.setCharacterSize(m_CharacterSize);
	m_WhitePeaceText.setString(m_WhitePeace);
	m_WhitePeaceText.setFillColor(m_TextColor);
	m_WhitePeaceText.setPosition(m_WhitePeaceTextPosition);

	m_EnforceDemandsTextPosition = sf::Vector2f(m_EnforceDemandsPosition);
	m_EnforceDemandsText.setFont(m_Font);
	m_EnforceDemandsText.setCharacterSize(m_CharacterSize);
	m_EnforceDemandsText.setString(m_EnforceDemands);
	m_EnforceDemandsText.setFillColor(m_TextColor);
	m_EnforceDemandsText.setPosition(m_EnforceDemandsTextPosition);

	m_SendTextPosition = sf::Vector2f(m_SendPosition);
	m_SendText.setFont(m_Font);
	m_SendText.setCharacterSize(m_CharacterSize);
	m_SendText.setString(m_Send);
	m_SendText.setFillColor(m_TextColor);
	m_SendText.setPosition(m_SendTextPosition);

	m_AttackersTextPosition = sf::Vector2f(m_AttackerPosition.x - m_PositionX, m_PositionY);
	m_AttackersText.setFont(m_Font);
	m_AttackersText.setCharacterSize(m_CharacterSize);
	m_AttackersText.setString(m_Attackers);
	m_AttackersText.setFillColor(m_AttackerColor);
	m_AttackersText.setPosition(m_AttackersTextPosition);

	m_AttackerArmyTextPosition = sf::Vector2f(m_AttackerPosition.x - m_PositionX, m_PositionY + m_SpriteSize);
	m_AttackerArmyText.setFont(m_Font);
	m_AttackerArmyText.setCharacterSize(m_CharacterSize);
	m_AttackerArmyText.setString(m_TotalSoldiers);
	m_AttackerArmyText.setFillColor(m_TextColor);
	m_AttackerArmyText.setPosition(m_AttackerArmyTextPosition);

	m_AttackerArmyStrengthTextPosition = sf::Vector2f(m_AttackerPosition.x - m_PositionX + m_SpriteSize, m_PositionY + m_SpriteSize * 2);
	m_AttackerArmyPosition = sf::Vector2f(m_AttackerArmyStrengthTextPosition.x - m_SpriteSize, m_AttackerArmyStrengthTextPosition.y);
	m_AttackerArmyStrengthText.setFont(m_Font);
	m_AttackerArmyStrengthText.setCharacterSize(m_CharacterSize);
	m_AttackerArmyStrengthText.setFillColor(m_TextColor);
	m_AttackerArmyStrengthText.setPosition(m_AttackerArmyStrengthTextPosition);

	m_DefendersTextPosition = sf::Vector2f(m_PositionX, m_PositionY);
	m_DefendersText.setFont(m_Font);
	m_DefendersText.setCharacterSize(m_CharacterSize);
	m_DefendersText.setString(m_Defenders);
	m_DefendersText.setFillColor(m_DefenderColor);
	m_DefendersText.setPosition(m_DefendersTextPosition);

	m_DefenderArmyTextPosition = sf::Vector2f(m_PositionX, m_PositionY + m_SpriteSize);
	m_DefenderArmyText.setFont(m_Font);
	m_DefenderArmyText.setCharacterSize(m_CharacterSize);
	m_DefenderArmyText.setString(m_TotalSoldiers);
	m_DefenderArmyText.setFillColor(m_TextColor);
	m_DefenderArmyText.setPosition(m_DefenderArmyTextPosition);

	m_DefenderArmyStrengthTextPosition = sf::Vector2f(m_PositionX + m_SpriteSize, m_PositionY + m_SpriteSize * 2);
	m_DefenderArmyPosition = sf::Vector2f(m_DefenderArmyStrengthTextPosition.x - m_SpriteSize, m_DefenderArmyStrengthTextPosition.y);
	m_DefenderArmyStrengthText.setFont(m_Font);
	m_DefenderArmyStrengthText.setCharacterSize(m_CharacterSize);
	m_DefenderArmyStrengthText.setFillColor(m_TextColor);
	m_DefenderArmyStrengthText.setPosition(m_DefenderArmyStrengthTextPosition);

	m_AttackerAlliesTextPosition = sf::Vector2f(m_AttackersTextPosition.x - m_SizeX * 0.25f, m_AttackersTextPosition.y);
	m_AttackerAlliesText.setFont(m_Font);
	m_AttackerAlliesText.setCharacterSize(m_CharacterSize);
	m_AttackerAlliesText.setString(m_Allies);
	m_AttackerAlliesText.setFillColor(m_TextColor);
	m_AttackerAlliesText.setPosition(m_AttackerAlliesTextPosition);

	m_DefenderAlliesTextPosition = sf::Vector2f(m_SizeX * 0.1f, m_DefendersTextPosition.y);
	m_DefenderAlliesText.setFont(m_Font);
	m_DefenderAlliesText.setCharacterSize(m_CharacterSize);
	m_DefenderAlliesText.setString(m_Allies);
	m_DefenderAlliesText.setFillColor(m_TextColor);
	m_DefenderAlliesText.setPosition(m_DefenderAlliesTextPosition);
}

void WarWindow::update()
{
	if (m_Visible)
	{
		clickButton();
	}

	handleWindow();
}

void WarWindow::render()
{
	if (m_Visible)
	{
		m_Window->draw(m_WindowShape);
		m_Window->draw(m_WarscoreShape);
		m_Window->draw(m_WarscoreProgressShape);

		if (m_AttackerCharacterIDs.front() == CharacterManager::get().getPlayerCharacterID()
		 || m_DefenderCharacterIDs.front() == CharacterManager::get().getPlayerCharacterID())
		{
			m_Window->draw(m_SurrenderShape);
			m_Window->draw(m_WhitePeaceShape);
			m_Window->draw(m_EnforceDemandsShape);
			m_Window->draw(m_SendShape);

			m_Window->draw(m_SurrenderText);
			m_Window->draw(m_WhitePeaceText);
			m_Window->draw(m_EnforceDemandsText);
			m_Window->draw(m_SendText);
		}

		m_Window->draw(m_WarNameText);
		m_Window->draw(m_DurationText);
		m_Window->draw(m_WarscoreText);
		m_Window->draw(m_WarscoreAmountText);
		m_Window->draw(m_AttackersText);
		m_Window->draw(m_AttackerArmyText);
		m_Window->draw(m_AttackerArmyStrengthText);
		m_Window->draw(m_DefendersText);
		m_Window->draw(m_DefenderArmyText);
		m_Window->draw(m_DefenderArmyStrengthText);
		m_Window->draw(m_AttackerAlliesText);
		m_Window->draw(m_DefenderAlliesText);

		unsigned int sizeMultiplier = 2;
		for (unsigned int index = 0; index < m_AttackerCharacterSprites.size(); index++)
		{
			if (index == 0)
			{
				sizeMultiplier = 4;
			}
			else
			{
				sizeMultiplier = 2;
			}
			m_Window->draw(m_AttackerCharacterShapes[index]);
			updateSprite(m_AttackerCharacterSprites[index], m_AttackerCharacterTextures[index], m_AttackerCharacterPositions[index], m_SpriteSize * sizeMultiplier);
		}
		for (unsigned int index = 0; index < m_DefenderCharacterSprites.size(); index++)
		{
			if (index == 0)
			{
				sizeMultiplier = 4;
			}
			else
			{
				sizeMultiplier = 2;
			}
			m_Window->draw(m_DefenderCharacterShapes[index]);
			updateSprite(m_DefenderCharacterSprites[index], m_DefenderCharacterTextures[index], m_DefenderCharacterPositions[index], m_SpriteSize * sizeMultiplier);
		}
		updateSprite(m_AttackerArmySprite, m_ArmyTexture, m_AttackerArmyPosition);
		updateSprite(m_DefenderArmySprite, m_ArmyTexture, m_DefenderArmyPosition);
	}
}

void WarWindow::onDayChange()
{
	if (m_Visible)
	{
		updateInfo();
	}
}

void WarWindow::updateInfo()
{
	m_War = WarManager::get().getWarAgainst(m_AttackerCharacterIDs.front(), m_DefenderCharacterIDs.front());

	if (m_War != nullptr)
	{
		Character& attackerMain = CharacterManager::get().getCharacter(m_AttackerCharacterIDs.front());

		std::stringstream stream;
		stream << attackerMain.m_KingdomName << m_CasusBelli << Map::get().getRegionById(m_War->m_WargoalRegion).m_RegionName;
		m_WarNameText.setString(stream.str());
		stream.str(std::string());
		stream.clear();

		stream << m_WarScore;
		m_WarscoreText.setString(stream.str());
		stream.str(std::string());
		stream.clear();

		//CharacterID playerCharacterID = CharacterManager::get().getPlayerCharacterID();
		//if (m_AttackerCharacterIDs.front() == playerCharacterID)
		//{
			int warscore = m_War->getWarscore(m_AttackerCharacterIDs.front());
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
				m_WarscoreAmountText.setFillColor(m_PositiveColor);
				stream << m_PositiveSign << warscore;
			}
			else
			{
				m_WarscoreAmountText.setFillColor(m_NegativeColor);
				stream << warscore;
			}
			m_WarscoreProgressShape.setSize(sf::Vector2f(m_WarscoreShape.getSize().x * 0.5f + m_WarscoreShape.getSize().x * 0.005f * warscore, m_WarscoreProgressShape.getSize().y));

			/*}
			else if (m_DefenderCharacterIDs.front() == playerCharacterID)
			{
				int warscore = m_War->getWarscore(m_DefenderCharacterIDs.front());
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
					m_WarscoreAmountText.setFillColor(m_PositiveColor);
					stream << m_PositiveSign << warscore;
				}
				else
				{
					m_WarscoreText.setFillColor(m_NegativeColor);
					stream << warscore;
				}
				m_WarscoreProgressShape.setSize(sf::Vector2f(m_WarscoreShape.getSize().x * 0.5f + m_WarscoreShape.getSize().x * 0.005f * warscore, m_WarscoreProgressShape.getSize().y));
			}*/
		stream << m_PercentSign;
		m_WarscoreAmountText.setString(stream.str());
		stream.str(std::string());
		stream.clear();

		int yearDiff = Time::m_GameDate.m_Date.m_Year - m_WarStartDate.m_Year;
		int monthDiff = Time::m_GameDate.m_Date.m_Month - m_WarStartDate.m_Month;
		if (yearDiff > 0)
		{
			stream << m_WarStarted << yearDiff;
			if (yearDiff == 1)
			{
				stream << m_YearAgo;
			}
			else
			{
				stream << m_YearsAgo;
			}
		}
		else if (monthDiff > 0)
		{
			stream << m_WarStarted << monthDiff;
			if (monthDiff == 1)
			{
				stream << m_MonthAgo;
			}
			else
			{
				stream << m_MonthsAgo;
			}
		}
		else
		{
			int dayDiff = Time::m_GameDate.m_Date.m_Day - m_WarStartDate.m_Day;
			stream << m_WarStarted << dayDiff;
			if (dayDiff == 1)
			{
				stream << m_DayAgo;
			}
			else
			{
				stream << m_DaysAgo;
			}
		}
		m_DurationText.setString(stream.str());
		stream.str(std::string());
		stream.clear();

		unsigned int totalArmy = 0;
		unsigned int totalRaisedArmy = 0;
		for (CharacterID attackerID : m_AttackerCharacterIDs)
		{
			Character& attacker = CharacterManager::get().getCharacter(attackerID);
			totalArmy += attacker.m_MaxArmySize;
			totalRaisedArmy += attacker.m_RaisedArmySize;
		}
		stream << totalRaisedArmy << m_DashSign << totalArmy;
		m_AttackerArmyStrengthText.setString(stream.str());
		totalArmy = 0;
		totalRaisedArmy = 0;
		stream.str(std::string());
		stream.clear();

		for (CharacterID defenderID : m_DefenderCharacterIDs)
		{
			Character& defender = CharacterManager::get().getCharacter(defenderID);
			totalArmy += defender.m_MaxArmySize;
			totalRaisedArmy += defender.m_RaisedArmySize;
		}
		stream << totalRaisedArmy << m_DashSign << totalArmy;
		m_DefenderArmyStrengthText.setString(stream.str());
		totalArmy = 0;
		totalRaisedArmy = 0;
		stream.str(std::string());
		stream.clear();
	}
	else
	{
		closeWindow();
	}
}

void WarWindow::handleWindow()
{
	InputHandler::setWarWindowOpen(m_Visible);
	if (m_Visible && (InputHandler::getLeftMouseReleased() || InputHandler::getRightMouseReleased()))
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (!m_WindowShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			if (InputHandler::getLeftMouseReleased())
			{
				InputHandler::setLeftMouseReleased(false);
			}
			if (InputHandler::getRightMouseReleased())
			{
				InputHandler::setRightMouseReleased(false);
			}
			closeWindow();
		}
	}
	if (m_Visible && (InputHandler::getPlayerUnitSelected() || InputHandler::getEscapePressed() || InputHandler::getCharacterWindowOpen() || InputHandler::getRegionWindowOpen()))
	{
		closeWindow();
	}
}

void WarWindow::openWindow(CharacterID mainAttackerID, CharacterID mainDefenderID, Date startDate)
{
	if (!m_Visible)
	{
		m_War = WarManager::get().getWarAgainst(mainAttackerID, mainDefenderID);
		if (m_War != nullptr)
		{
			m_Visible = true;
			m_WarStartDate = startDate;
			unsigned int index = 0;
			float sizeMultiplier = 0.0f;
			float xOffset = 0.0f;
			float yOffset = 0.0f;
			for (CharacterID attackerID : m_War->m_Attackers)
			{
				Character& attacker = CharacterManager::get().getCharacter(attackerID);
				m_AttackerCharacterIDs.push_back(attackerID);
				if (attackerID == mainAttackerID)
				{
					sizeMultiplier = 4.0f;
					xOffset = 0.0f;
					yOffset = 0.0f;
					m_AttackerCharacterPositions.push_back(m_AttackerPosition);
				}
				else
				{
					sizeMultiplier = 2.0f;
					xOffset = m_SpriteSize * sizeMultiplier + m_OutlineThickness;
					yOffset = xOffset * 0.5f;
					int i = index - 1;
					if (index > 6)
					{
						i = index % 7;
						yOffset *= 3;
					}
					m_AttackerCharacterPositions.push_back(m_AttackerAlliesTextPosition + sf::Vector2f(xOffset * i, yOffset));
				}
				sf::RectangleShape attackerShape;
				attackerShape.setFillColor(sf::Color::Transparent);
				attackerShape.setOutlineColor(attacker.m_RegionColor);
				attackerShape.setOutlineThickness(m_OutlineThickness * 0.25f * sizeMultiplier);
				attackerShape.setSize(sf::Vector2f(m_SpriteSize * sizeMultiplier, m_SpriteSize * sizeMultiplier));
				attackerShape.setPosition(m_AttackerCharacterPositions[index]);
				m_AttackerCharacterShapes.push_back(attackerShape);
				sf::Sprite attackerSprite;
				m_AttackerCharacterSprites.push_back(attackerSprite);
				m_AttackerCharacterTextures.push_back(m_YoungFemaleCharacterTexture);
				index++;
			}
			index = 0;
			for (CharacterID defenderID : m_War->m_Defenders)
			{
				Character& defender = CharacterManager::get().getCharacter(defenderID);
				m_DefenderCharacterIDs.push_back(defenderID);
				if (defenderID == mainDefenderID)
				{
					sizeMultiplier = 4.0f;
					xOffset = 0.0f;
					yOffset = 0.0f;
					m_DefenderCharacterPositions.push_back(m_DefenderPosition);
				}
				else
				{
					sizeMultiplier = 2.0f;
					xOffset = m_SpriteSize * sizeMultiplier + m_OutlineThickness;
					yOffset = xOffset * 0.5f;
					int i = index - 1;
					if (index > 6)
					{
						i = index % 7;
						yOffset *= 3;
					}
					m_DefenderCharacterPositions.push_back(m_DefenderAlliesTextPosition + sf::Vector2f(xOffset * i, yOffset));
				}
				sf::RectangleShape defenderShape;
				defenderShape.setFillColor(sf::Color::Transparent);
				defenderShape.setOutlineColor(defender.m_RegionColor);
				defenderShape.setOutlineThickness(m_OutlineThickness * 0.25f * sizeMultiplier);
				defenderShape.setSize(sf::Vector2f(m_SpriteSize * sizeMultiplier, m_SpriteSize * sizeMultiplier));
				defenderShape.setPosition(m_DefenderCharacterPositions[index]);
				m_DefenderCharacterShapes.push_back(defenderShape);
				sf::Sprite defenderSprite;
				m_DefenderCharacterSprites.push_back(defenderSprite);
				m_DefenderCharacterTextures.push_back(m_YoungMaleCharacterTexture);
				index++;
			}
			updateInfo();
			m_WindowShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));
			m_DaySubscriptionHandle = Time::m_GameDate.subscribeToDayChange([](void* data) { WarWindow& warWindow = *static_cast<WarWindow*>(data); warWindow.onDayChange(); }, static_cast<void*>(this));
			InputHandler::setWarWindowOpen(true);
		}	
	}
}

void WarWindow::closeWindow()
{
	if (m_Visible)
	{
		Time::m_GameDate.unsubscribeToDayChange(m_DaySubscriptionHandle);

		m_DefenderCharacterIDs.clear();
		m_DefenderCharacterShapes.clear();
		m_DefenderCharacterSprites.clear();
		m_DefenderCharacterTextures.clear();
		m_DefenderCharacterPositions.clear();

		m_AttackerCharacterIDs.clear();
		m_AttackerCharacterShapes.clear();
		m_AttackerCharacterSprites.clear();
		m_AttackerCharacterTextures.clear();
		m_AttackerCharacterPositions.clear();

		m_WindowShape.setSize(sf::Vector2f());
		m_Visible = false;
		InputHandler::setWarWindowOpen(false);
	}
}

void WarWindow::sendPeaceOffer(PeaceType type)
{
	Character& playerCharacter = CharacterManager::get().getPlayerCharacter();
	CharacterID enemy = INVALID_CHARACTER_ID;
	if (m_AttackerCharacterIDs.front() == playerCharacter.m_CharacterID)
	{
		enemy = m_DefenderCharacterIDs.front();
	}
	else if (m_DefenderCharacterIDs.front() == playerCharacter.m_CharacterID)
	{
		enemy = m_AttackerCharacterIDs.front();
	}
	if (!WarManager::get().getWarHandlesOfCharacter(playerCharacter.m_CharacterID).empty())
	{
		CharacterManager::get().sendPeaceOffer(playerCharacter.m_CharacterID, enemy, type);

		if (Game::m_BattleSound.getStatus() == sf::SoundSource::Playing && WarManager::get().getWarHandlesOfCharacter(playerCharacter.m_CharacterID).empty())
		{
			Game::m_BattleSound.stop();
			Game::m_Sound.play();
		}
	}
}

void WarWindow::clickButton()
{
	if (InputHandler::getLeftMouseReleased()
	&& (m_AttackerCharacterIDs.front() == CharacterManager::get().getPlayerCharacterID()
	 || m_DefenderCharacterIDs.front() == CharacterManager::get().getPlayerCharacterID()))
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (m_SurrenderShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
			m_CurrentPeaceType = PeaceType::Surrender;
			m_SurrenderShape.setFillColor(m_OwnerColor);
			m_WhitePeaceShape.setFillColor(sf::Color::Transparent);
			m_EnforceDemandsShape.setFillColor(sf::Color::Transparent);
			m_SendShape.setFillColor(m_OwnerColor);
			return;
		}
		else if (m_WhitePeaceShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
			m_CurrentPeaceType = PeaceType::White_Peace;
			m_SurrenderShape.setFillColor(sf::Color::Transparent);
			m_WhitePeaceShape.setFillColor(m_OwnerColor);
			m_EnforceDemandsShape.setFillColor(sf::Color::Transparent);
			m_SendShape.setFillColor(m_OwnerColor);
			return;
		}
		else if (m_EnforceDemandsShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
			m_CurrentPeaceType = PeaceType::Enforce_Demands;
			m_SurrenderShape.setFillColor(sf::Color::Transparent);
			m_WhitePeaceShape.setFillColor(sf::Color::Transparent);
			m_EnforceDemandsShape.setFillColor(m_OwnerColor);
			m_SendShape.setFillColor(m_OwnerColor);
			return;
		}
		else if (m_SendShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
			if (m_CurrentPeaceType == PeaceType::Surrender || m_CurrentPeaceType == PeaceType::White_Peace || m_CurrentPeaceType == PeaceType::Enforce_Demands)
			{
				sendPeaceOffer(m_CurrentPeaceType);
				m_SurrenderShape.setFillColor(sf::Color::Transparent);
				m_WhitePeaceShape.setFillColor(sf::Color::Transparent);
				m_EnforceDemandsShape.setFillColor(sf::Color::Transparent);
				m_SendShape.setFillColor(sf::Color::Transparent);
				closeWindow();
				return;
			}
		}
	}
	else if (InputHandler::getRightMouseReleased())
	{
		CharacterWindow& characterWindow = *UIManager::get().m_CharacterWindow;
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		for (unsigned int index = 0; index < m_AttackerCharacterShapes.size(); index++)
		{
			if (m_AttackerCharacterShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				InputHandler::setRightMouseReleased(false);
				characterWindow.m_CurrentCharacterID = m_AttackerCharacterIDs[index];
				characterWindow.checkIfPlayerCharacter();
				characterWindow.updateInfo();
				characterWindow.openWindow();
				return;
			}
		}
		for (unsigned int index = 0; index < m_DefenderCharacterShapes.size(); index++)
		{
			if (m_DefenderCharacterShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				InputHandler::setRightMouseReleased(false);
				characterWindow.m_CurrentCharacterID = m_DefenderCharacterIDs[index];
				characterWindow.checkIfPlayerCharacter();
				characterWindow.updateInfo();
				characterWindow.openWindow();
				return;
			}
		}
	}
}

void WarWindow::updateSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, unsigned int spriteSize)
{
	sprite.setTexture(texture, true);
	sprite.setPosition(position);

	sf::FloatRect localSize = sprite.getLocalBounds();

	sprite.setScale(spriteSize / localSize.width, spriteSize / localSize.height);

	m_Window->draw(sprite);
}