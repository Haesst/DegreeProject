#include "Game/UI/WarWindow.h"
#include <string>
#include "Engine/Time.h"
#include "Engine/Window.h"
#include "Engine/AssetHandler.h"
#include "Engine/InputHandler.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/Data/Character.h"
#include "Game/UI/UIManager.h"
#include "Game/UI/CharacterWindow.h"
#include <sstream>
#include "Game/DiplomacyManager.h"
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

	m_ArmyTexture = assethandler.getTextureAtPath("Assets/Graphics/soldier unit.png");

	m_PositionX = m_OutlineThickness;
	m_PositionY = m_Window->getSize().y - m_OutlineThickness - m_SizeY;

	m_OwnerColor = playerCharacter.m_RegionColor;

	setShape(m_WindowShape, m_FillColor, m_OwnerColor, m_OutlineThickness, { m_SizeX, m_SizeY }, { m_PositionX, m_PositionY });
	
	m_AttackerPosition = { m_PositionX + m_SizeX - m_SpriteSize * 4, m_PositionY - m_SpriteSize * 4 - m_OutlineThickness * 2 };
	m_DefenderPosition = { m_PositionX, m_PositionY - m_SpriteSize * 4 - m_OutlineThickness * 2 };
	
	m_WarscorePosition = { m_PositionX + m_SizeX * 0.5f - (m_SizeX * 0.25f) * 0.5f, m_PositionY + m_SizeY * 0.4f };
	setShape(m_WarscoreShape, m_AttackerColor, m_OwnerColor, m_OutlineThickness, { m_SizeX * 0.25f, m_SpriteSize }, m_WarscorePosition);
	setShape(m_WarscoreProgressShape, m_DefenderColor, m_FillColor, 0.0f, { (m_SizeX * 0.25f) * 0.5f, m_SpriteSize }, m_WarscorePosition);

	m_WarNameTextPosition = { m_WarscorePosition.x, m_WarscorePosition.y - m_SpriteSize * 3.5f };
	setText(m_WarNameText, m_Font, m_CharacterSize, m_TextColor, m_WarNameTextPosition);

	m_DurationTextPosition = { m_WarscorePosition.x, m_WarscorePosition.y - m_SpriteSize * 2.5f };
	setText(m_DurationText, m_Font, (unsigned int)(m_CharacterSize * 0.75f), m_TextColor, m_DurationTextPosition);

	m_WarscoreTextPosition = { m_WarscorePosition.x, m_WarscorePosition.y - m_SpriteSize * 1.5f };
	setText(m_WarscoreText, m_Font, m_CharacterSize, m_TextColor, m_WarscoreTextPosition);

	m_WarscoreAmountTextPosition = { m_WarscorePosition.x + 110, m_WarscorePosition.y - m_SpriteSize * 1.5f };
	setText(m_WarscoreAmountText, m_Font, m_CharacterSize, m_PositiveColor, m_WarscoreAmountTextPosition);

	m_AttackersTextPosition = { m_AttackerPosition.x - m_PositionX, m_PositionY };
	setText(m_AttackersText, m_Font, m_CharacterSize, m_AttackerColor, m_AttackersTextPosition, m_Attackers);

	m_AttackerArmyTextPosition = { m_AttackerPosition.x - m_PositionX, m_PositionY + m_SpriteSize };
	setText(m_AttackerArmyText, m_Font, m_CharacterSize, m_TextColor, m_AttackerArmyTextPosition, m_TotalSoldiers);

	m_AttackerArmyStrengthTextPosition = { m_AttackerPosition.x - m_PositionX + m_SpriteSize, m_PositionY + m_SpriteSize * 2 };
	setText(m_AttackerArmyStrengthText, m_Font, m_CharacterSize, m_TextColor, m_AttackerArmyStrengthTextPosition);

	m_DefendersTextPosition = { m_PositionX, m_PositionY };
	setText(m_DefendersText, m_Font, m_CharacterSize, m_DefenderColor, m_DefendersTextPosition, m_Defenders);

	m_DefenderArmyTextPosition = { m_PositionX, m_PositionY + m_SpriteSize };
	setText(m_DefenderArmyText, m_Font, m_CharacterSize, m_TextColor, m_DefenderArmyTextPosition, m_TotalSoldiers);

	m_DefenderArmyStrengthTextPosition = { m_PositionX + m_SpriteSize, m_PositionY + m_SpriteSize * 2 };
	setText(m_DefenderArmyStrengthText, m_Font, m_CharacterSize, m_TextColor, m_DefenderArmyStrengthTextPosition);

	m_AttackerAlliesTextPosition = { m_AttackersTextPosition.x - m_SizeX * 0.25f, m_AttackersTextPosition.y };
	setText(m_AttackerAlliesText, m_Font, m_CharacterSize, m_TextColor, m_AttackerAlliesTextPosition, m_Allies);

	m_DefenderAlliesTextPosition = { m_SizeX * 0.1f, m_DefendersTextPosition.y };
	setText(m_DefenderAlliesText, m_Font, m_CharacterSize, m_TextColor, m_DefenderAlliesTextPosition, m_Allies);

	m_SurrenderPosition = { m_PositionX + m_SizeX * 0.5f - m_OutlineThickness - (m_SizeX / 12) * 1.5f, m_PositionY + m_SizeY * 0.4f + m_SpriteSize * 2 };
	setShape(m_SurrenderShape, m_TransparentColor, m_OwnerColor, m_OutlineThickness, { m_SizeX / 12, m_SpriteSize }, m_SurrenderPosition);
	setText(m_SurrenderText, m_Font, m_CharacterSize, m_TextColor, m_SurrenderPosition, m_Surrender);

	m_WhitePeacePosition = { m_PositionX + m_SizeX * 0.5f - (m_SizeX / 12) * 0.5f, m_PositionY + m_SizeY * 0.4f + m_SpriteSize * 2 };
	setShape(m_WhitePeaceShape, m_TransparentColor, m_OwnerColor, m_OutlineThickness, { m_SizeX / 12, m_SpriteSize }, m_WhitePeacePosition);
	setText(m_WhitePeaceText, m_Font, m_CharacterSize, m_TextColor, m_WhitePeacePosition, m_WhitePeace);

	m_EnforceDemandsPosition = { m_PositionX + m_SizeX * 0.5f + m_OutlineThickness + (m_SizeX / 12) * 0.5f, m_PositionY + m_SizeY * 0.4f + m_SpriteSize * 2 };
	setShape(m_EnforceDemandsShape, m_TransparentColor, m_OwnerColor, m_OutlineThickness, { m_SizeX / 12, m_SpriteSize }, m_EnforceDemandsPosition);
	setText(m_EnforceDemandsText, m_Font, m_CharacterSize, m_TextColor, m_EnforceDemandsPosition, m_EnforceDemands);

	m_SendPosition = { m_PositionX + m_SizeX * 0.5f - (m_SizeX / 12) * 0.5f, m_PositionY + m_SizeY * 0.4f + m_SpriteSize * 4 };
	setShape(m_SendShape, m_TransparentColor, m_OwnerColor, m_OutlineThickness, { m_SizeX / 12, m_SpriteSize }, m_SendPosition);
	setText(m_SendText, m_Font, m_CharacterSize, m_TextColor, m_SendPosition, m_Send);

	m_AttackerArmyPosition = { m_AttackerArmyStrengthTextPosition.x - m_SpriteSize, m_AttackerArmyStrengthTextPosition.y };
	setSprite(m_AttackerArmySprite, m_ArmyTexture, m_AttackerArmyPosition);

	m_DefenderArmyPosition = { m_DefenderArmyStrengthTextPosition.x - m_SpriteSize, m_DefenderArmyStrengthTextPosition.y };
	setSprite(m_DefenderArmySprite, m_ArmyTexture, m_DefenderArmyPosition);
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

		for (unsigned int index = 0; index < m_AttackerCharacterSprites.size(); index++)
		{
			m_Window->draw(m_AttackerCharacterShapes[index]);
			m_Window->draw(m_AttackerCharacterSprites[index]);
		}
		for (unsigned int index = 0; index < m_DefenderCharacterSprites.size(); index++)
		{
			m_Window->draw(m_DefenderCharacterShapes[index]);
			m_Window->draw(m_DefenderCharacterSprites[index]);
		}

		m_Window->draw(m_AttackerArmySprite);
		m_Window->draw(m_DefenderArmySprite);
	}
}

void WarWindow::onDayChange()
{
	if (m_Visible)
	{
		updateInfo();
	}
}

void WarWindow::setWarscore(CharacterID& characterID, std::stringstream& stream, int& warscore)
{
	DiplomacyManager& warManager = DiplomacyManager::get();

	warscore = warManager.getWarscore(m_War->getHandle(), characterID);
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
}

void WarWindow::updateInfo()
{
	m_War = DiplomacyManager::get().getWarAgainst(m_AttackerCharacterIDs.front(), m_DefenderCharacterIDs.front());
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

		int warscore = 0;
		CharacterID playerCharacterID = CharacterManager::get().getPlayerCharacterID();
		if (m_DefenderCharacterIDs.front() == playerCharacterID)
		{
			setWarscore(m_DefenderCharacterIDs.front(), stream, warscore);
			m_WarscoreProgressShape.setSize({ m_WarscoreShape.getSize().x * 0.5f + m_WarscoreShape.getSize().x * 0.005f * warscore, m_WarscoreProgressShape.getSize().y });
		}
		else if (m_AttackerCharacterIDs.front() == playerCharacterID)
		{
			setWarscore(m_AttackerCharacterIDs.front(), stream, warscore);
			m_WarscoreProgressShape.setSize({ m_WarscoreShape.getSize().x * 0.5f + m_WarscoreShape.getSize().x * 0.005f * warscore, m_WarscoreProgressShape.getSize().y });
		}
		else
		{
			setWarscore(m_AttackerCharacterIDs.front(), stream, warscore);
			m_WarscoreProgressShape.setSize({ m_WarscoreShape.getSize().x * 0.5f + m_WarscoreShape.getSize().x * 0.005f * -warscore, m_WarscoreProgressShape.getSize().y });
		}
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
		if (InputHandler::getLeftMouseReleased())
		{
			InputHandler::setLeftMouseReleased(false);
		}
		if (InputHandler::getRightMouseReleased())
		{
			InputHandler::setRightMouseReleased(false);
		}
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (!m_WindowShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y)
		 && !m_AttackerCharacterShapes.front().getGlobalBounds().contains(mousePosition.x, mousePosition.y)
		 && !m_DefenderCharacterShapes.front().getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			closeWindow();
		}
	}
	if (m_Visible && (InputHandler::getPlayerUnitSelected() || InputHandler::getEscapePressed() || InputHandler::getCharacterWindowOpen() || InputHandler::getRegionWindowOpen()))
	{
		closeWindow();
	}
}

void WarWindow::updateWarParticipants(CharacterID& mainParticipantID, CharacterID& participantID, std::vector<CharacterID>& participantsIDs, std::vector<sf::RectangleShape>& participantsShapes, std::vector<sf::Sprite>& participantsSprites, sf::Texture& mainParticipantTexture, sf::Texture& participantTexture, sf::Vector2f& mainParticipantPosition, sf::Vector2f& participantPosition, unsigned int& index)
{
	Character& participant = CharacterManager::get().getCharacter(participantID);

	participantsIDs.push_back(participantID);

	float sizeMultiplier = 0.0f;
	float xOffset = 0.0f;
	float yOffset = 0.0f;
	sf::Vector2f shapeSize;
	float outlineThickness = 0.0f;

	sf::RectangleShape shape;
	sf::Sprite sprite;
	if (participantID == mainParticipantID)
	{
		sizeMultiplier = 4.0f;
		xOffset = 0.0f;
		yOffset = 0.0f;
		shapeSize = { m_SpriteSize * sizeMultiplier, m_SpriteSize * sizeMultiplier };
		outlineThickness = m_OutlineThickness * 0.25f * sizeMultiplier;
		setShape(shape, m_TransparentColor, participant.m_RegionColor, outlineThickness, shapeSize, mainParticipantPosition);
		setSprite(sprite, mainParticipantTexture, shape.getPosition(), (unsigned int)(m_SpriteSize * sizeMultiplier));
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
		shapeSize = { m_SpriteSize * sizeMultiplier, m_SpriteSize * sizeMultiplier };
		outlineThickness = m_OutlineThickness * 0.25f * sizeMultiplier;
		sf::Vector2f shapePosition = { participantPosition.x + xOffset * i, participantPosition.y + yOffset };
		setShape(shape, m_TransparentColor, participant.m_RegionColor, outlineThickness, shapeSize, shapePosition);
		setSprite(sprite, participantTexture, shape.getPosition(), (unsigned int)(m_SpriteSize * sizeMultiplier));
	}
	participantsShapes.push_back(shape);
	participantsSprites.push_back(sprite);
}

void WarWindow::openWindow(CharacterID mainAttackerID, CharacterID mainDefenderID, Date startDate)
{
	if (!m_Visible)
	{
		m_War = DiplomacyManager::get().getWarAgainst(mainAttackerID, mainDefenderID);
		if (m_War != nullptr)
		{
			m_Visible = true;
			m_WarStartDate = startDate;
			unsigned int index = 0;
			for (CharacterID attackerID : m_War->m_Attackers)
			{
				updateWarParticipants(mainAttackerID, attackerID, m_AttackerCharacterIDs, m_AttackerCharacterShapes, m_AttackerCharacterSprites, m_FemaleCharacterTexture, m_YoungFemaleCharacterTexture, m_AttackerPosition, m_AttackerAlliesTextPosition, index);
				index++;
			}
			index = 0;
			for (CharacterID defenderID : m_War->m_Defenders)
			{
				updateWarParticipants(mainDefenderID, defenderID, m_DefenderCharacterIDs, m_DefenderCharacterShapes, m_DefenderCharacterSprites, m_MaleCharacterTexture, m_YoungMaleCharacterTexture, m_DefenderPosition, m_DefenderAlliesTextPosition, index);
				index++;
			}
			updateInfo();
			m_WindowShape.setSize({ m_SizeX, m_SizeY });
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

		m_AttackerCharacterIDs.clear();
		m_AttackerCharacterShapes.clear();
		m_AttackerCharacterSprites.clear();

		m_WindowShape.setSize({  });
		m_Visible = false;
		InputHandler::setWarWindowOpen(false);
	}
}

void WarWindow::setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position)
{
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThickness);
	shape.setSize(size);
	shape.setPosition(position);
}

void WarWindow::setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string)
{
	text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setFillColor(fillColor);
	text.setPosition(position);
	text.setString(string);
}

void WarWindow::setSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, unsigned int spriteSize)
{
	sprite.setTexture(texture, true);
	sprite.setScale(spriteSize / sprite.getLocalBounds().width, spriteSize / sprite.getLocalBounds().height);
	sprite.setPosition(position);
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
	if (!DiplomacyManager::get().getWarHandlesOfCharacter(playerCharacter.m_CharacterID).empty())
	{
		CharacterManager::get().sendPeaceOffer(playerCharacter.m_CharacterID, enemy, type);

		if (Game::m_BattleSound.getStatus() == sf::SoundSource::Playing && DiplomacyManager::get().getWarHandlesOfCharacter(playerCharacter.m_CharacterID).empty())
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