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
#include "Game/Map/Map.h"
#include <sstream>
#include "Game/WarManager.h"
#include "Game/AI/AIManager.h"
#include "Game/Data/AIData.h"
#include "Game/UI/UIManager.h"

WarWindow::WarWindow(UIID id, sf::Font font, Vector2D, Vector2D size)
{
	m_OwnedUIWindow = id;
	m_Font = font;
	m_SizeX = size.x;
	m_SizeY = size.y;
}

void WarWindow::start()
{
	Time::m_GameDate.subscribeToMonthChange(std::bind(&WarWindow::onMonthChange, this));

	m_Window = Window::getWindow();

	AssetHandler& assethandler = AssetHandler::get();

	Character& playerCharacter = CharacterManager::get()->getPlayerCharacter();

	m_MaleCharacterTexture = assethandler.getTextureAtPath("Assets/Graphics/MalePortrait.jpg");
	m_FemaleCharacterTexture = assethandler.getTextureAtPath("Assets/Graphics/FemalePortrait.jpg");
	m_YoungMaleCharacterTexture = assethandler.getTextureAtPath("Assets/Graphics/Unit.png");
	m_YoungFemaleCharacterTexture = assethandler.getTextureAtPath("Assets/Graphics/Harriet.jpg");

	m_ArmyTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/soldier unit.png");
	m_AttackerArmyPosition = sf::Vector2f(m_SpriteSize, m_SpriteSize);
	m_DefenderArmyPosition = sf::Vector2f(m_SizeX - m_SpriteSize, m_SpriteSize);

	m_PositionX = (int)m_OutlineThickness;
	m_PositionY = (int)(m_Window->getSize().y - m_OutlineThickness - m_SizeY);

	m_OwnerColor = playerCharacter.m_RegionColor;

	m_WindowShape.setFillColor(m_FillColor);
	m_WindowShape.setOutlineThickness(m_OutlineThickness);
	m_WindowShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));
	m_WindowShape.setOutlineColor(m_OwnerColor);

	m_AttackerPosition = Vector2D(m_PositionX + m_SizeX - m_SpriteSize * 4, m_PositionY - m_SpriteSize * 4 - m_OutlineThickness * 2);
	m_AttackerShape.setFillColor(sf::Color::Transparent);
	m_AttackerShape.setOutlineThickness(m_OutlineThickness);
	m_AttackerShape.setSize(sf::Vector2f(m_SpriteSize * 4, m_SpriteSize * 4));
	m_AttackerShape.setOutlineColor(m_AttackerColor);

	m_DefenderPosition = Vector2D(m_PositionX, m_PositionY - m_SpriteSize * 4 - m_OutlineThickness * 2);
	m_DefenderShape.setFillColor(sf::Color::Transparent);
	m_DefenderShape.setOutlineThickness(m_OutlineThickness);
	m_DefenderShape.setSize(sf::Vector2f(m_SpriteSize * 4, m_SpriteSize * 4));
	m_DefenderShape.setOutlineColor(m_DefenderColor);

	m_WarscorePosition = Vector2D(m_PositionX + m_SizeX * 0.5f - (m_SizeX / 4) * 0.5f, m_PositionY + m_SizeY * 0.4f);
	m_WarscoreShape.setFillColor(m_AttackerColor);
	m_WarscoreShape.setOutlineThickness(m_OutlineThickness);
	m_WarscoreShape.setSize(sf::Vector2f(m_SizeX / 4, m_SpriteSize));
	m_WarscoreShape.setOutlineColor(m_OwnerColor);

	m_WarscoreProgressPosition = Vector2D(m_PositionX + m_SizeX * 0.5f - (m_SizeX / 4) * 0.5f, m_PositionY + m_SizeY * 0.4f);
	m_WarscoreProgressShape.setFillColor(m_DefenderColor);
	m_WarscoreProgressShape.setOutlineThickness(m_OutlineThickness);
	m_WarscoreProgressShape.setSize(sf::Vector2f((m_SizeX / 4) * 0.5f, m_SpriteSize));
	m_WarscoreProgressShape.setOutlineColor(m_OwnerColor);

	m_SurrenderPosition = Vector2D(m_PositionX + m_SizeX * 0.5f - m_OutlineThickness - (m_SizeX / 12) * 1.5f, m_PositionY + m_SizeY * 0.4f + m_SpriteSize * 2);
	m_SurrenderShape.setFillColor(sf::Color::Transparent);
	m_SurrenderShape.setOutlineThickness(m_OutlineThickness);
	m_SurrenderShape.setSize(sf::Vector2f(m_SizeX / 12, m_SpriteSize));
	m_SurrenderShape.setOutlineColor(m_OwnerColor);

	m_WhitePeacePosition = Vector2D(m_PositionX + m_SizeX * 0.5f - (m_SizeX / 12) * 0.5f, m_PositionY + m_SizeY * 0.4f + m_SpriteSize * 2);
	m_WhitePeaceShape.setFillColor(sf::Color::Transparent);
	m_WhitePeaceShape.setOutlineThickness(m_OutlineThickness);
	m_WhitePeaceShape.setSize(sf::Vector2f(m_SizeX / 12, m_SpriteSize));
	m_WhitePeaceShape.setOutlineColor(m_OwnerColor);

	m_EnforceDemandsPosition = Vector2D(m_PositionX + m_SizeX * 0.5f + m_OutlineThickness + (m_SizeX / 12) * 0.5f, m_PositionY + m_SizeY * 0.4f + m_SpriteSize * 2);
	m_EnforceDemandsShape.setFillColor(sf::Color::Transparent);
	m_EnforceDemandsShape.setOutlineThickness(m_OutlineThickness);
	m_EnforceDemandsShape.setSize(sf::Vector2f(m_SizeX / 12, m_SpriteSize));
	m_EnforceDemandsShape.setOutlineColor(m_OwnerColor);

	m_SendPosition = Vector2D(m_PositionX + m_SizeX * 0.5f - (m_SizeX / 12) * 0.5f, m_PositionY + m_SizeY * 0.4f + m_SpriteSize * 4);
	m_SendShape.setFillColor(sf::Color::Transparent);
	m_SendShape.setOutlineThickness(m_OutlineThickness);
	m_SendShape.setSize(sf::Vector2f(m_SizeX / 12, m_SpriteSize));
	m_SendShape.setOutlineColor(m_OwnerColor);

	m_WarNameTextPosition = Vector2D(m_WarscorePosition.x, m_WarscorePosition.y - m_SpriteSize * 3.5f);
	m_WarNameText.setFont(m_Font);
	m_WarNameText.setCharacterSize(m_CharacterSize);
	m_WarNameText.setStyle(m_Style);
	m_WarNameText.setFillColor(m_TextColor);
	m_WarNameText.setString("Kingdom of Sicily's war for Lazium");

	m_DurationTextPosition = Vector2D(m_WarscorePosition.x, m_WarscorePosition.y - m_SpriteSize * 2.5f);
	m_DurationText.setFont(m_Font);
	m_DurationText.setCharacterSize(m_CharacterSize * 0.75f);
	m_DurationText.setStyle(m_Style);
	m_DurationText.setFillColor(m_TextColor);
	m_DurationText.setString(m_WarStarted + "13" + m_Month + m_Plural + m_Ago);

	m_WarscoreTextPosition = Vector2D(m_WarscorePosition.x, m_WarscorePosition.y - m_SpriteSize * 1.5f);
	m_WarscoreText.setFont(m_Font);
	m_WarscoreText.setCharacterSize(m_CharacterSize);
	m_WarscoreText.setStyle(m_Style);
	m_WarscoreText.setFillColor(m_TextColor);
	m_WarscoreText.setString(m_WarScore + "+51%");

	m_SurrenderTextPosition = Vector2D(m_SurrenderPosition.x, m_SurrenderPosition.y);
	m_SurrenderText.setFont(m_Font);
	m_SurrenderText.setCharacterSize(m_CharacterSize);
	m_SurrenderText.setStyle(m_Style);
	m_SurrenderText.setString(m_Surrender);
	m_SurrenderText.setFillColor(m_TextColor);

	m_WhitePeaceTextPosition = Vector2D(m_WhitePeacePosition.x, m_WhitePeacePosition.y);
	m_WhitePeaceText.setFont(m_Font);
	m_WhitePeaceText.setCharacterSize(m_CharacterSize);
	m_WhitePeaceText.setStyle(m_Style);
	m_WhitePeaceText.setString(m_WhitePeace);
	m_WhitePeaceText.setFillColor(m_TextColor);

	m_EnforceDemandsTextPosition = Vector2D(m_EnforceDemandsPosition.x, m_EnforceDemandsPosition.y);
	m_EnforceDemandsText.setFont(m_Font);
	m_EnforceDemandsText.setCharacterSize(m_CharacterSize);
	m_EnforceDemandsText.setStyle(m_Style);
	m_EnforceDemandsText.setString(m_EnforceDemands);
	m_EnforceDemandsText.setFillColor(m_TextColor);

	m_SendTextPosition = Vector2D(m_SendPosition.x, m_SendPosition.y);
	m_SendText.setFont(m_Font);
	m_SendText.setCharacterSize(m_CharacterSize);
	m_SendText.setStyle(m_Style);
	m_SendText.setString(m_Send);
	m_SendText.setFillColor(m_TextColor);

	m_AttackerArmyTextPosition = Vector2D(m_AttackerPosition.x, m_PositionY);
	m_AttackerArmyText.setFont(m_Font);
	m_AttackerArmyText.setCharacterSize(m_CharacterSize);
	m_AttackerArmyText.setStyle(m_Style);
	m_AttackerArmyText.setString(m_TotalSoldiers);
	m_AttackerArmyText.setFillColor(m_TextColor);

	m_AttackerArmyStrengthTextPosition;
	m_AttackerArmyStrengthText.setFont(m_Font);
	m_AttackerArmyStrengthText.setCharacterSize(m_CharacterSize);
	m_AttackerArmyStrengthText.setStyle(m_Style);
	m_AttackerArmyStrengthText.setFillColor(m_TextColor);
	m_AttackerArmyStrengthText.setString("270");

	m_DefenderArmyTextPosition = Vector2D(m_PositionX, m_PositionY);
	m_DefenderArmyText.setFont(m_Font);
	m_DefenderArmyText.setCharacterSize(m_CharacterSize);
	m_DefenderArmyText.setStyle(m_Style);
	m_DefenderArmyText.setString(m_TotalSoldiers);
	m_DefenderArmyText.setFillColor(m_TextColor);

	m_DefenderArmyStrengthTextPosition;
	m_DefenderArmyStrengthText.setFont(m_Font);
	m_DefenderArmyStrengthText.setCharacterSize(m_CharacterSize);
	m_DefenderArmyStrengthText.setStyle(m_Style);
	m_DefenderArmyStrengthText.setFillColor(m_TextColor);
	m_AttackerArmyStrengthText.setString("210");

	m_AttackerAlliesTextPosition;
	m_AttackerAlliesText.setFont(m_Font);
	m_AttackerAlliesText.setCharacterSize(m_CharacterSize);
	m_AttackerAlliesText.setStyle(m_Style);
	m_AttackerAlliesText.setString(m_Allies);
	m_AttackerAlliesText.setFillColor(m_TextColor);

	m_DefenderAlliesTextPosition;
	m_DefenderAlliesText.setFont(m_Font);
	m_DefenderAlliesText.setCharacterSize(m_CharacterSize);
	m_DefenderAlliesText.setStyle(m_Style);
	m_DefenderAlliesText.setString(m_Allies);
	m_DefenderAlliesText.setFillColor(m_TextColor);

	openWindow();
}

void WarWindow::update()
{
	handleWindow();

	if (m_Visible)
	{
		clickButton();

		m_WindowShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_PositionX, m_PositionY)));
		
		m_AttackerShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_AttackerPosition.x, m_AttackerPosition.y)));

		m_DefenderShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_DefenderPosition.x, m_DefenderPosition.y)));

		m_WarscoreShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_WarscorePosition.x, m_WarscorePosition.y)));
		
		m_WarscoreProgressShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_WarscoreProgressPosition.x, m_WarscoreProgressPosition.y)));
		
		m_SurrenderShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_SurrenderPosition.x, m_SurrenderPosition.y)));
		
		m_WhitePeaceShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_WhitePeacePosition.x, m_WhitePeacePosition.y)));
		
		m_EnforceDemandsShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_EnforceDemandsPosition.x, m_EnforceDemandsPosition.y)));
		
		m_SendShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_SendPosition.x, m_SendPosition.y)));

		m_WarNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_WarNameTextPosition.x, m_WarNameTextPosition.y)));

		m_DurationText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_DurationTextPosition.x, m_DurationTextPosition.y)));

		m_WarscoreText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_WarscoreTextPosition.x, m_WarscoreTextPosition.y)));

		m_SurrenderText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_SurrenderTextPosition.x, m_SurrenderTextPosition.y)));

		m_WhitePeaceText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_WhitePeaceTextPosition.x, m_WhitePeaceTextPosition.y)));

		m_EnforceDemandsText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_EnforceDemandsTextPosition.x, m_EnforceDemandsTextPosition.y)));

		m_SendText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_SendTextPosition.x, m_SendTextPosition.y)));

		m_AttackerArmyText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_AttackerArmyTextPosition.x, m_AttackerArmyTextPosition.y)));

		m_AttackerArmyStrengthText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_AttackerArmyStrengthTextPosition.x, m_AttackerArmyStrengthTextPosition.y)));

		m_DefenderArmyText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_DefenderArmyTextPosition.x, m_DefenderArmyTextPosition.y)));

		m_DefenderArmyStrengthText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_DefenderArmyStrengthTextPosition.x, m_DefenderArmyStrengthTextPosition.y)));

		m_DefenderAlliesText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_AttackerAlliesTextPosition.x, m_AttackerAlliesTextPosition.y)));

		m_AttackerAlliesText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i(m_DefenderAlliesTextPosition.x, m_DefenderAlliesTextPosition.y)));
	}
}

void WarWindow::render()
{
	if (m_Visible)
	{
		m_Window->draw(m_WindowShape);
		m_Window->draw(m_AttackerShape);
		m_Window->draw(m_DefenderShape);
		m_Window->draw(m_WarscoreShape);
		m_Window->draw(m_WarscoreProgressShape);
		m_Window->draw(m_SurrenderShape);
		m_Window->draw(m_WhitePeaceShape);
		m_Window->draw(m_EnforceDemandsShape);
		m_Window->draw(m_SendShape);

		m_Window->draw(m_WarNameText);
		m_Window->draw(m_SurrenderText);
		m_Window->draw(m_WhitePeaceText);
		m_Window->draw(m_EnforceDemandsText);
		m_Window->draw(m_WarscoreText);
		m_Window->draw(m_DefenderAlliesText);
		m_Window->draw(m_DurationText);
		m_Window->draw(m_AttackerAlliesText);
		m_Window->draw(m_SendText);
		m_Window->draw(m_AttackerArmyText);
		m_Window->draw(m_AttackerArmyStrengthText);
		m_Window->draw(m_DefenderArmyText);
		m_Window->draw(m_DefenderArmyStrengthText);
	}
}

void WarWindow::onMonthChange()
{
	updateInfo();
}

void WarWindow::updateInfo()
{
	
}

void WarWindow::handleWindow()
{
	InputHandler::setWarWindowOpen(m_Visible);
	if (m_Visible && (InputHandler::getPlayerUnitSelected() || InputHandler::getEscapePressed()))
	{
		closeWindow();
	}
	else if (InputHandler::getCharacterWindowOpen() || InputHandler::getRegionWindowOpen())
	{
		closeWindow();
	}
}

void WarWindow::openWindow()
{
	m_Visible = true;
	m_WindowShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));
}

void WarWindow::closeWindow()
{
	m_Open = false;
	m_Visible = false;
	m_WindowShape.setSize(sf::Vector2f());
}

void WarWindow::clickButton()
{
	if (InputHandler::getRightMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getMousePosition();
	}
}

void WarWindow::updateSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, int spriteSize)
{
	sprite.setTexture(texture, true);
	sprite.setPosition(Window::getWindow()->mapPixelToCoords(sf::Vector2i((int)position.x, (int)position.y)));

	sf::FloatRect localSize = sprite.getLocalBounds();

	sprite.setScale(spriteSize / localSize.width, spriteSize / localSize.height);

	m_Window->draw(sprite);
}