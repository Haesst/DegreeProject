#include "Game/UI/StatBar.h"
#include <SFML/Graphics.hpp>
#include "Engine/Window.h"
#include "Engine/AssetHandler.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/Systems/UnitManager.h"
#include "Game/Data/Character.h"
#include "Game/Data/Unit.h"
#include <sstream>
#include <iomanip>

StatBar::StatBar(UIID id, sf::Font font, Vector2D, Vector2D size)
{
	m_OwnedUIWindow = id;
	m_Font = font;
	m_SizeX = size.x;
	m_SizeY = size.y;
}

void StatBar::start()
{
	m_DaySubscriptionHandle = Time::m_GameDate.subscribeToDayChange([](void* data) { StatBar& statBar = *static_cast<StatBar*>(data); statBar.onDayChange(); }, static_cast<void*>(this));

	m_Window = Window::getWindow();

	m_CoinTexture = AssetHandler::get().loadImageFromFile("Assets/Graphics/Coins.png");
	m_ArmyTexture = AssetHandler::get().loadImageFromFile("Assets/Graphics/soldier unit.png");

	m_CoinPosition = { m_SizeX * 0.1f + m_Window->getSize().x - m_SizeX - m_OutlineThickness, m_SizeY * 0.3f };
	m_ArmyPosition = { m_SizeX * 0.5f + m_Window->getSize().x - m_SizeX - m_OutlineThickness, m_SizeY * 0.3f };

	float positionX = m_Window->getSize().x - m_SizeX + m_OutlineThickness;
	float positionY = m_OutlineThickness;

	m_OwnerColor = CharacterManager::get().getPlayerCharacter().m_RegionColor;

	setShape(m_WindowShape, m_FillColor, m_OwnerColor, m_OutlineThickness, { m_SizeX, m_SizeY }, { positionX, positionY });

	setText(m_CurrentGoldText, m_Font, m_CharacterSize, m_OwnerColor, { positionX + m_SizeX * 0.2f, positionY });
	setText(m_CurrentIncomeText, m_Font, (unsigned int)(m_CharacterSize * 0.5f), m_OwnerColor, { positionX + m_SizeX * 0.2f, positionY * 4 });
	setText(m_CurrentMaxArmyText, m_Font, m_CharacterSize, m_OwnerColor, { positionX + m_SizeX * 0.6f, positionY });
	setText(m_MaxArmyText, m_Font, (unsigned int)(m_CharacterSize * 0.5f), m_OwnerColor, { positionX + m_SizeX * 0.625f, positionY * 4 });

	setSprite(m_CoinSprite, m_CoinTexture, m_CoinPosition);
	setSprite(m_ArmySprite, m_ArmyTexture, m_ArmyPosition);

	updateStats();
}

void StatBar::render()
{
	m_Window->draw(m_WindowShape);
	m_Window->draw(m_CoinSprite);
	m_Window->draw(m_ArmySprite);
	m_Window->draw(m_CurrentGoldText);
	m_Window->draw(m_CurrentIncomeText);
	m_Window->draw(m_MaxArmyText);
	m_Window->draw(m_CurrentMaxArmyText);
}

void StatBar::onDayChange()
{
	updateStats();
}

void StatBar::updateStats()
{
	Character& character = CharacterManager::get().getPlayerCharacter();

	std::stringstream stream;
	stream << std::fixed << std::setprecision(1) << character.m_CurrentGold;
	m_CurrentGoldText.setString(stream.str());
	stream.str(std::string());
	stream.clear();

	float income = character.m_Income;
	if (income >= 0.0f)
	{
		stream << m_PositiveSign << std::fixed << std::setprecision(1) << character.m_Income;
	}
	else
	{
		stream << std::fixed << std::setprecision(1) << character.m_Income;
	}
	m_CurrentIncomeText.setString(stream.str());
	stream.str(std::string());
	stream.clear();

	stream << character.m_MaxArmySize;
	m_MaxArmyText.setString(stream.str());
	stream.str(std::string());
	stream.clear();

	stream << UnitManager::get().getUnitOfCharacter(character.m_CharacterID).m_RepresentedForce;
	m_CurrentMaxArmyText.setString(stream.str());
	stream.str(std::string());
	stream.clear();
}

void StatBar::updateOwnerColor(sf::Color& newColor)
{
	m_OwnerColor = newColor;
	m_WindowShape.setOutlineColor(m_OwnerColor);
	m_CurrentGoldText.setFillColor(m_OwnerColor);
	m_CurrentIncomeText.setFillColor(m_OwnerColor);
	m_MaxArmyText.setFillColor(m_OwnerColor);
	m_CurrentMaxArmyText.setFillColor(m_OwnerColor);
}

void StatBar::setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position)
{
	text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setFillColor(fillColor);
	text.setPosition(position);
}

void StatBar::setSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position)
{
	sprite.setTexture(texture, true);
	sprite.setScale(m_SpriteSize / sprite.getLocalBounds().width, m_SpriteSize / sprite.getLocalBounds().height);
	sprite.setPosition(position);
}

void StatBar::setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position)
{
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThickness);
	shape.setSize(size);
	shape.setPosition(position);
}