#pragma once

#include <SFML/Graphics.hpp>
#include "Engine/Window.h"
#include "Engine/InputHandler.h"
#include "Engine/AssetHandler.h"
#include "Game/Data/Character.h"
#include "Game/Map/Map.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/Game.h"

class CharacterWindow
{
public:
	sf::RectangleShape m_WindowShape;
	sf::RectangleShape m_DeclareWarShape;
	sf::RectangleShape m_MakePeaceShape;
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Red;
	sf::Color m_DeclareWarColor = sf::Color(210, 32, 60);
	sf::Color m_MakePeaceColor = sf::Color(67, 175, 17);
	float m_SizeX = 600.0f;
	float m_SizeY = 1060.0f;
	float m_OutlineThickness = 10.0f;
	bool m_Visible = false;
	sf::Text m_OwnerNameText;
	sf::Text m_TaxText;
	sf::Text m_RegionNameText;
	sf::Text m_OwnerCharacterText;
	sf::Font m_Font;
	sf::Text m_DeclareWarText;
	sf::Text m_MakePeaceText;
	int m_RegionTax = 0;
	unsigned int m_CurrentRegionID = 0;
	std::string m_RegionName = "";
	std::string m_OwnerKingdomName = "";
	std::string m_OwnerCharacterName = "";
	std::string m_OwnerTitle = "";
	std::string m_DeclareWar = "Declare War";
	std::string m_MakePeace = "Make Peace";
	std::vector<std::string> m_Titles;
	int m_CharacterSize = 50;
#pragma warning(push)
#pragma warning(disable: 26812)
	sf::Text::Style m_Style = sf::Text::Regular;
#pragma warning(pop)
	bool m_Open = false;

	sf::RenderWindow* m_Window = nullptr;
	CharacterWindow* m_CharacterWindows = nullptr;
	Character* m_PlayerCharacter = nullptr;
	MapRegion* m_CurrentMapRegion = nullptr;
	bool m_PlayerRegion = false;

	const std::string m_RegionString = "Region: ";
	const std::string m_TaxString = "Tax: ";

	sf::Sound m_BattleSound;
	sf::SoundBuffer m_BattleSoundBuffer;
	float m_Volume = 5.0f;
	UIID m_OwnedUIWindow = INVALID_UI_ID;
	unsigned int m_CurrentWars = 0;

	sf::Texture m_CharacterTexture;
	sf::Sprite m_CharacterSprite;
	int m_SpriteSize = 64;
	sf::Vector2f m_CharacterPosition = sf::Vector2f();

	CharacterWindow(UIID id, sf::Font font)
	{
		m_OwnedUIWindow = id;
		m_Font = font;
		m_Titles.push_back("Emperor ");
		m_Titles.push_back("King ");
		m_Titles.push_back("Duke ");
		m_Titles.push_back("Count ");
		m_Titles.push_back("Baron ");
		m_Window = Window::getWindow();
		m_CharacterTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Unit.png");
		m_CharacterPosition = sf::Vector2f(m_SizeX * 0.1f, m_SizeY * 0.025f);
	}

	void start()
	{
		m_BattleSound = AssetHandler::get().loadAudioFile("Assets/Audio/battle.wav", m_BattleSoundBuffer);
		m_BattleSound.setLoop(true);
		m_BattleSound.setVolume(m_Volume);

		m_PlayerCharacter = &CharacterManager::get()->getPlayerCharacter();

		m_WindowShape.setFillColor(m_FillColor);
		m_WindowShape.setOutlineThickness(m_OutlineThickness);

		m_DeclareWarShape.setFillColor(sf::Color::Transparent);
		m_DeclareWarShape.setOutlineColor(m_DeclareWarColor);
		m_DeclareWarShape.setOutlineThickness(m_OutlineThickness);
		m_DeclareWarShape.setSize(sf::Vector2f(m_SizeX * 0.2f, m_SizeY * 0.05f));

		m_DeclareWarText.setFont(m_Font);
		m_DeclareWarText.setCharacterSize((int)(m_CharacterSize * 0.5f));
		m_DeclareWarText.setStyle(m_Style);
		m_DeclareWarText.setString(m_DeclareWar);
		m_DeclareWarText.setFillColor(m_DeclareWarColor);

		m_MakePeaceShape.setFillColor(sf::Color::Transparent);
		m_MakePeaceShape.setOutlineColor(m_MakePeaceColor);
		m_MakePeaceShape.setOutlineThickness(m_OutlineThickness);
		m_MakePeaceShape.setSize(sf::Vector2f(m_SizeX * 0.2f, m_SizeY * 0.05f));

		m_MakePeaceText.setFont(m_Font);
		m_MakePeaceText.setCharacterSize((int)(m_CharacterSize * 0.5f));
		m_MakePeaceText.setStyle(m_Style);
		m_MakePeaceText.setString(m_MakePeace);
		m_MakePeaceText.setFillColor(m_MakePeaceColor);

		m_OwnerCharacterText.setFont(m_Font);
		m_OwnerCharacterText.setCharacterSize(m_CharacterSize);
		m_OwnerCharacterText.setStyle(m_Style);

		m_OwnerNameText.setFont(m_Font);
		m_OwnerNameText.setCharacterSize(m_CharacterSize);
		m_OwnerNameText.setStyle(m_Style);

		m_RegionNameText.setFont(m_Font);
		m_RegionNameText.setCharacterSize(m_CharacterSize);
		m_RegionNameText.setStyle(m_Style);

		m_TaxText.setFont(m_Font);
		m_TaxText.setCharacterSize(m_CharacterSize);
		m_TaxText.setStyle(m_Style);
	}

	void update()
	{
		updateInfo();
		handleWindow();

		if (m_Visible)
		{
			clickButton();

			m_WindowShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)m_OutlineThickness, (int)m_OutlineThickness)));
			m_WindowShape.setOutlineColor(m_OwnerColor);

			m_DeclareWarShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.775f), (int)(m_SizeY * 0.032f))));
			m_DeclareWarText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.785f), (int)(m_SizeY * 0.04f))));

			m_MakePeaceShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.775f), (int)(m_SizeY * 0.112f))));
			m_MakePeaceText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.785f), (int)(m_SizeY * 0.12f))));

			m_OwnerCharacterText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.25f), (int)(m_SizeY * 0.025f))));
			m_OwnerCharacterText.setString(m_OwnerTitle + m_OwnerCharacterName);
			m_OwnerCharacterText.setFillColor(m_OwnerColor);

			m_OwnerNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.1f), (int)(m_SizeY * 0.1f))));
			m_OwnerNameText.setString(m_OwnerKingdomName);
			m_OwnerNameText.setFillColor(m_OwnerColor);

			m_RegionNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.1f), (int)(m_SizeY * 0.2f))));
			m_RegionNameText.setString(m_RegionString + m_RegionName);
			m_RegionNameText.setFillColor(m_OwnerColor);

			m_TaxText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.1f), (int)(m_SizeY * 0.3f))));
			m_TaxText.setString(m_TaxString + std::to_string(m_RegionTax));
			m_TaxText.setFillColor(m_OwnerColor);
		}
	}

	void render()
	{
		if (m_Visible)
		{
			m_Window->draw(m_WindowShape);
			updateSprite(m_CharacterSprite, m_CharacterTexture, m_CharacterPosition);
			m_Window->draw(m_OwnerNameText);
			m_Window->draw(m_RegionNameText);
			m_Window->draw(m_TaxText);
			m_Window->draw(m_OwnerCharacterText);
			if (!m_PlayerRegion)
			{
				m_Window->draw(m_DeclareWarShape);
				m_Window->draw(m_DeclareWarText);
				m_Window->draw(m_MakePeaceShape);
				m_Window->draw(m_MakePeaceText);
			}
		}
	}

	void updateInfo()
	{
		if (InputHandler::getRightMouseClicked() && !InputHandler::getPlayerUnitSelected())
		{
			Vector2D mousePosition = InputHandler::getMousePosition();
			if (!m_WindowShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				Vector2DInt mouseMapPosition = InputHandler::getMouseMapPosition();
				if (Map::get().mapSquareDataContainsKey(mouseMapPosition))
				{
					size_t regionID = 0;
					for (auto& squareData : Map::get().m_MapSquareData)
					{
						if (squareData.m_Position == mouseMapPosition)
						{
							regionID = squareData.m_RegionID;
						}
					}

					if (m_CurrentRegionID != regionID)
					{
						m_CurrentRegionID = regionID;
						checkIfPlayerRegion(m_CurrentRegionID);
						m_CurrentMapRegion = &Map::get().getRegionById(m_CurrentRegionID);
						m_RegionTax = m_CurrentMapRegion->m_RegionTax;
						m_RegionName = m_CurrentMapRegion->m_RegionName;
						Character& character = CharacterManager::get()->getCharacter(m_CurrentMapRegion->m_OwnerID);
						m_OwnerKingdomName = character.m_KingdomName;
						m_OwnerCharacterName = character.m_Name;
						m_OwnerTitle = m_Titles[(unsigned int)character.m_CharacterTitle];
						m_OwnerColor = character.m_RegionColor;
					}
					m_Open = true;
				}
				else
				{
					closeWindow();
				}
			}
			else
			{
				m_Open = false;
			}
		}
		else if (InputHandler::getLeftMouseClicked() && !InputHandler::getPlayerUnitSelected() && m_Visible)
		{
			Vector2D mousePosition = InputHandler::getMousePosition();
			if (!m_WindowShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				closeWindow();
			}
		}
	}

	void handleWindow()
	{
		InputHandler::setCharacterWindowOpen(m_Visible);
		if (InputHandler::getRightMouseReleased() && !InputHandler::getPlayerUnitSelected() && m_Open && !m_Visible)
		{
			openWindow();
		}
		else if (m_Visible && (InputHandler::getPlayerUnitSelected() || InputHandler::getEscapePressed()))
		{
			closeWindow();
		}
		else if (InputHandler::getRegionWindowOpen())
		{
			closeWindow();
		}
	}

	void openWindow()
	{
		m_Visible = true;
		m_WindowShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));
	}

	void closeWindow()
	{
		m_Open = false;
		m_Visible = false;
		m_WindowShape.setSize(sf::Vector2f());
	}

	void clickButton()
	{
		if (InputHandler::getLeftMouseReleased() && !m_PlayerRegion)
		{
			Vector2D mousePosition = InputHandler::getMousePosition();
			if (m_DeclareWarShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				//EntityID warTarget = m_CurrentMapRegion->m_OwnerID;
				//m_PlayerCharacter->declareWar(warTarget, characterWindow.m_CurrentRegionID);

				m_CurrentWars++;
				Game::m_Sound.pause();
				if (m_BattleSound.getStatus() != sf::SoundSource::Playing)
				{
					m_BattleSound.play();
				}
			}
			else if (m_MakePeaceShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				//EntityManager* entityManager = &EntityManager::get();

				//WarManager* warManager = &WarManager::get();
				// War* war = warManager->getWarAgainst(*m_PlayerCharacter, target);
				// 
				// if (war->getHandle() != -1)
				// {
				// 	characterSystem->makePeace(*m_PlayerCharacter, target, war->getHandle());
				// }
				if (m_CurrentWars > 0)
				{
					m_CurrentWars--;
				}
				if (m_BattleSound.getStatus() == sf::SoundSource::Playing && m_CurrentWars == 0)
				{
					m_BattleSound.stop();
					Game::m_Sound.play();
				}
			}
		}
	}

	bool checkIfPlayerRegion(CharacterID currentRegionID)
	{
		for (unsigned int ownedRegionID : m_PlayerCharacter->m_OwnedRegionIDs)
		{
			if (currentRegionID == ownedRegionID)
			{
				m_PlayerRegion = true;
				break;
			}
			else
			{
				m_PlayerRegion = false;
			}
		}
		return m_PlayerRegion;
	}

	void updateSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position)
	{
		sprite.setTexture(texture, true);
		sprite.setPosition(Window::getWindow()->mapPixelToCoords(sf::Vector2i((int)position.x, (int)position.y)));

		sf::FloatRect localSize = sprite.getLocalBounds();

		sprite.setScale(m_SpriteSize / localSize.width, m_SpriteSize / localSize.height);

		m_Window->draw(sprite);
	}
};