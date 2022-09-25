#include <Game/UI/StatBar.h>
#include <SFML/Graphics.hpp>
#include <Engine/Window.h>
#include <Engine/AssetHandler.h>
#include <Game/Systems/CharacterManager.h>
#include <Game/Systems/UnitManager.h>
#include <Game/Data/Character.h>
#include <Game/Data/Unit.h>
#include <sstream>
#include <iomanip>

StatBar::StatBar(UIID id, sf::Font font, Vector2D position, Vector2D size) : UIWindow(id, font, position, size)
{
	m_CharacterSize = 30;
	m_OutlineThickness = 10.0f;
	m_PositionX = m_Window->getSize().x - m_SizeX - m_OutlineThickness;
	m_PositionY = m_OutlineThickness;
	m_Visible = true;
}

void StatBar::start()
{
	UIWindow::start();

	Time::m_GameDate.subscribeToDayChange([](void* data) { StatBar& statBar = *static_cast<StatBar*>(data); statBar.onDayChange(); }, static_cast<void*>(this));
	AssetHandler& assetHandler = AssetHandler::get();
	m_OutlineColor = CharacterManager::get().getPlayerCharacter().m_RegionColor;
	m_TextFillColor = m_OutlineColor;
	setShape(m_WindowShape, m_WindowFillColor, m_OutlineColor, m_OutlineThickness, { m_SizeX, m_SizeY }, { m_PositionX, m_PositionY });
	for (unsigned int index = 0; index < 4; index++)
	{
		float xOffset = 0.2f;
		float yOffset = 1.0f;
		unsigned int characterSize = m_CharacterSize;
		if (index % 2 != 0)
		{
			yOffset = 4.0f;
			characterSize /= 2;
		}
		if (index > 1)
		{
			xOffset += 0.4f;
		}	
		if (index > 2)
		{
			xOffset += 0.025f;
		}
		m_Texts.push_back(sf::Text());
		setText(m_Texts[index], m_Font, characterSize, m_TextFillColor, { m_PositionX + m_SizeX * xOffset, m_PositionY * yOffset });
	}
	m_Textures.push_back(assetHandler.loadImageFromFile("Assets/Graphics/Coins.png"));
	m_Textures.push_back(assetHandler.loadImageFromFile("Assets/Graphics/soldier unit.png"));
	for (unsigned int index = 0; index < m_Textures.size(); index++)
	{
		m_Sprites.push_back(sf::Sprite());
		sf::Vector2f spritePosition = { m_SizeX * 0.1f + m_SizeX * 0.4f * index + m_Window->getSize().x - m_SizeX - m_OutlineThickness, m_SizeY * 0.3f };
		setSprite(m_Sprites[index], m_Textures[index], spritePosition);
	}
	updateStats();
}

void StatBar::render()
{
	UIWindow::render();
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
	m_Texts[0].setString(stream.str());
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
	m_Texts[1].setString(stream.str());
	stream.str(std::string());
	stream.clear();

	stream << UnitManager::get().getUnitOfCharacter(character.m_CharacterID).m_RepresentedForce;
	m_Texts[2].setString(stream.str());
	stream.str(std::string());
	stream.clear();

	stream << character.m_MaxArmySize;
	m_Texts[3].setString(stream.str());
	stream.str(std::string());
	stream.clear();
}

void StatBar::updateOwnerColor(sf::Color& newColor)
{
	m_TextFillColor = newColor;
	m_OutlineColor = m_TextFillColor;
	m_WindowShape.setOutlineColor(m_OutlineColor);
	for (sf::Text& text : m_Texts)
	{
		text.setFillColor(m_TextFillColor);
	}
}
