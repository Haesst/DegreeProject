#pragma once

#include <SFML/Graphics.hpp>
#include "Engine/Window.h"
#include "Engine/InputHandler.h"
#include "Engine/AssetHandler.h"
#include "Game/Data/Character.h"
#include "Game/Map/Map.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/Game.h"
#include <iomanip>
#include "Game/AI/AIManager.h"
#include "Game/WarManager.h"

class CharacterWindow
{
public:
	sf::RectangleShape m_WindowShape;
	sf::RectangleShape m_DeclareWarShape;
	sf::RectangleShape m_MakePeaceShape;
	sf::RectangleShape m_MarriageShape;
	sf::RectangleShape m_AllianceShape;
	std::vector<sf::RectangleShape> m_ChildrenShapes = std::vector<sf::RectangleShape>();
	std::vector<sf::Sprite> m_ChildrenSprites = std::vector<sf::Sprite>();
	std::vector<sf::Texture> m_ChildrenTextures = std::vector<sf::Texture>();
	sf::Texture m_MaleBabyTexture;
	sf::Texture m_FemaleBabyTexture;
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Red;
	sf::Color m_DeclareWarColor = sf::Color(210, 32, 60);
	sf::Color m_MakePeaceColor = sf::Color(67, 175, 17);
	float m_SizeX = 600.0f;
	float m_SizeY = 1060.0f;
	float m_OutlineThickness = 10.0f;
	bool m_Visible = false;
	sf::Text m_RealmNameText;
	sf::Text m_GoldText;
	sf::Text m_ArmyText;
	sf::Text m_CharacterNameText;
	sf::Text m_CharacterAgeText;
	sf::Font m_Font;
	sf::Text m_DeclareWarText;
	sf::Text m_MakePeaceText;
	sf::Text m_MarriageText;
	sf::Text m_AllianceText;
	std::vector<sf::Text> m_ChildrenNames = std::vector<sf::Text>();
	std::vector<bool> m_ShowChildrenNames = std::vector<bool>();
	unsigned int m_CurrentRegionID = 0;
	CharacterID m_CurrentCharacterID = INVALID_CHARACTER_ID;
	const std::string m_DeclareWar = "Declare War";
	const std::string m_MakePeace = "Make Peace";
	const std::string m_Marriage = "Marriage";
	const std::string m_Alliance = "Alliance";
	const std::string m_Dash = "/";
	const char* m_PregnantTrait = "Pregnant";
	std::string m_MaleTitles[(unsigned int)Title::Baron + 1];
	std::string m_FemaleTitles[(unsigned int)Title::Baron + 1];
	int m_CharacterSize = 50;
#pragma warning(push)
#pragma warning(disable: 26812)
	sf::Text::Style m_Style = sf::Text::Regular;
#pragma warning(pop)
	bool m_Open = false;

	std::unordered_map<int, int> m_PlayerWars;

	sf::RenderWindow* m_Window = nullptr;
	Character* m_PlayerCharacter = nullptr;
	bool m_IsPlayerCharacter = false;

	sf::Sound m_BattleSound;
	sf::SoundBuffer m_BattleSoundBuffer;
	float m_Volume = 5.0f;
	UIID m_OwnedUIWindow = INVALID_UI_ID;
	unsigned int m_CurrentWars = 0;

	sf::Texture m_MaleCharacterTexture;
	sf::Texture m_FemaleCharacterTexture;
	sf::Sprite m_CharacterSprite;
	const static int m_SpriteSize = 64;
	sf::Vector2f m_CharacterPosition = sf::Vector2f();

	sf::Texture m_GoldTexture;
	sf::Sprite m_GoldSprite;
	sf::Vector2f m_GoldPosition = sf::Vector2f();

	sf::Texture m_ArmyTexture;
	sf::Sprite m_ArmySprite;
	sf::Vector2f m_ArmyPosition = sf::Vector2f();

	sf::Texture m_AgeTexture;
	sf::Sprite m_AgeSprite;
	sf::Vector2f m_AgePosition = sf::Vector2f();

	sf::Texture m_MaleGenderTexture;
	sf::Texture m_FemaleGenderTexture;
	sf::Sprite m_GenderSprite;
	sf::Vector2f m_GenderPosition = sf::Vector2f();

	sf::Texture m_PregnantTexture;
	sf::Sprite m_Pregnantprite;
	sf::Vector2f m_PregnantPosition = sf::Vector2f();

	sf::Texture m_MarriedTexture;
	sf::Sprite m_MarriedSprite;
	sf::Vector2f m_MarriedPosition = sf::Vector2f();
	sf::Text m_SpouseName;

	Gender m_Gender = Gender::Male;
	CharacterID m_SpouseID = INVALID_CHARACTER_ID;
	bool m_ShowSpouseName = false;
	bool m_Pregnant = false;

	CharacterWindow(UIID id, sf::Font font, Vector2D, Vector2D size)
	{
		m_OwnedUIWindow = id;
		m_Font = font;
		m_SizeX = size.x;
		m_SizeY = size.y;

		m_MaleTitles[(unsigned int)Title::Emperor] = "Emperor ";
		m_MaleTitles[(unsigned int)Title::King] = "King ";
		m_MaleTitles[(unsigned int)Title::Duke] = "Duke ";
		m_MaleTitles[(unsigned int)Title::Count] = "Count ";
		m_MaleTitles[(unsigned int)Title::Baron] = "Baron ";

		m_FemaleTitles[(unsigned int)Title::Emperor] = "Empress ";
		m_FemaleTitles[(unsigned int)Title::King] = "Queen ";
		m_FemaleTitles[(unsigned int)Title::Duke] = "Duchess ";
		m_FemaleTitles[(unsigned int)Title::Count] = "Countess ";
		m_FemaleTitles[(unsigned int)Title::Baron] = "Baroness ";

		m_ChildrenShapes.clear();
		m_ChildrenSprites.clear();
		m_ChildrenTextures.clear();
		m_ChildrenNames.clear();
		m_ShowChildrenNames.clear();
	}

	void start()
	{
		Time::m_GameDate.subscribeToMonthChange(std::bind(&CharacterWindow::onMonthChange, this));

		m_Window = Window::getWindow();

		m_MaleCharacterTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/MalePortrait.jpg");
		m_FemaleCharacterTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/FemalePortrait.jpg");
		m_CharacterPosition = sf::Vector2f(m_SizeX * 0.1f, m_SizeY * 0.025f);

		m_GoldTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Coins.png");
		m_GoldPosition = sf::Vector2f(m_SizeX * 0.1f, m_SizeY * 0.32f);

		m_ArmyTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/soldier unit.png");
		m_ArmyPosition = sf::Vector2f(m_SizeX * 0.1f, m_SizeY * 0.22f);

		m_AgeTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Age.png");
		m_AgePosition = sf::Vector2f(m_SizeX * 0.1f, m_SizeY * 0.42f);

		m_MaleGenderTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Male.png");
		m_FemaleGenderTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Female.png");
		m_GenderPosition = sf::Vector2f(m_SizeX * 0.3f, m_SizeY * 0.42f);

		m_PregnantTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Pregnant.png");
		m_PregnantPosition = sf::Vector2f(m_SizeX * 0.5f, m_SizeY * 0.42f);

		m_MaleBabyTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/BabyMale.png");
		m_FemaleBabyTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/BabyFemale.png");

		m_MarriedTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Married.png");
		m_MarriedPosition = sf::Vector2f(m_SizeX * 0.4f, m_SizeY * 0.42f);

		m_SpouseName.setFont(m_Font);
		m_SpouseName.setCharacterSize(m_CharacterSize / 2);
		m_SpouseName.setStyle(m_Style);

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

		m_MarriageShape.setFillColor(sf::Color::Transparent);
		m_MarriageShape.setOutlineColor(m_MakePeaceColor);
		m_MarriageShape.setOutlineThickness(m_OutlineThickness);
		m_MarriageShape.setSize(sf::Vector2f(m_SizeX * 0.2f, m_SizeY * 0.05f));

		m_MarriageText.setFont(m_Font);
		m_MarriageText.setCharacterSize((int)(m_CharacterSize * 0.5f));
		m_MarriageText.setStyle(m_Style);
		m_MarriageText.setString(m_Marriage);
		m_MarriageText.setFillColor(m_MakePeaceColor);

		m_AllianceShape.setFillColor(sf::Color::Transparent);
		m_AllianceShape.setOutlineColor(m_MakePeaceColor);
		m_AllianceShape.setOutlineThickness(m_OutlineThickness);
		m_AllianceShape.setSize(sf::Vector2f(m_SizeX * 0.2f, m_SizeY * 0.05f));

		m_AllianceText.setFont(m_Font);
		m_AllianceText.setCharacterSize((int)(m_CharacterSize * 0.5f));
		m_AllianceText.setStyle(m_Style);
		m_AllianceText.setString(m_Alliance);
		m_AllianceText.setFillColor(m_MakePeaceColor);

		m_CharacterNameText.setFont(m_Font);
		m_CharacterNameText.setCharacterSize(m_CharacterSize);
		m_CharacterNameText.setStyle(m_Style);

		m_RealmNameText.setFont(m_Font);
		m_RealmNameText.setCharacterSize(m_CharacterSize);
		m_RealmNameText.setStyle(m_Style);

		m_CharacterAgeText.setFont(m_Font);
		m_CharacterAgeText.setCharacterSize(m_CharacterSize);
		m_CharacterAgeText.setStyle(m_Style);

		m_ArmyText.setFont(m_Font);
		m_ArmyText.setCharacterSize(m_CharacterSize);
		m_ArmyText.setStyle(m_Style);

		m_GoldText.setFont(m_Font);
		m_GoldText.setCharacterSize(m_CharacterSize);
		m_GoldText.setStyle(m_Style);
	}

	void update()
	{
		clickOnMap();
		handleWindow();

		if (m_Visible)
		{
			clickButton();

			m_WindowShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)m_OutlineThickness, (int)m_OutlineThickness)));

			m_DeclareWarShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.775f), (int)(m_SizeY * 0.2f))));
			m_DeclareWarText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.785f), (int)(m_SizeY * 0.208f))));

			m_MakePeaceShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.775f), (int)(m_SizeY * 0.3f))));
			m_MakePeaceText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.785f), (int)(m_SizeY * 0.308f))));

			m_AllianceShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.775f), (int)(m_SizeY * 0.4f))));
			m_AllianceText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.785f), (int)(m_SizeY * 0.408f))));

			m_MarriageShape.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.775f), (int)(m_SizeY * 0.5f))));
			m_MarriageText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.785f), (int)(m_SizeY * 0.508f))));

			m_CharacterNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.25f), (int)(m_SizeY * 0.025f))));

			m_RealmNameText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.1f), (int)(m_SizeY * 0.1f))));

			m_ArmyText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.2f), (int)(m_SizeY * 0.2f))));

			m_GoldText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.2f), (int)(m_SizeY * 0.3f))));

			m_CharacterAgeText.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.2f), (int)(m_SizeY * 0.4f))));

			for (unsigned int index = 0; index < m_ChildrenShapes.size(); index++)
			{
				m_ChildrenShapes[index].setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.1f), (int)(m_SizeY * 0.05f * (index + 1) + m_SizeY * 0.45f))));
				m_ChildrenNames[index].setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_SizeX * 0.2f), (int)(m_SizeY * 0.05f * (index + 1) + m_SizeY * 0.45f))));
			}

			m_SpouseName.setPosition(m_Window->mapPixelToCoords(sf::Vector2i((int)(m_MarriedPosition.x + m_SizeX * 0.1f), (int)(m_MarriedPosition.y))));
		}
	}

	void render()
	{
		if (m_Visible)
		{
			m_Window->draw(m_WindowShape);
			if (CharacterManager::get()->getCharacter(m_CurrentCharacterID).m_Gender == Gender::Male)
			{
				updateSprite(m_CharacterSprite, m_MaleCharacterTexture, m_CharacterPosition);
			}
			else
			{
				updateSprite(m_CharacterSprite, m_FemaleCharacterTexture, m_CharacterPosition);
			}
			m_Window->draw(m_RealmNameText);
			updateSprite(m_GoldSprite, m_GoldTexture, m_GoldPosition, m_SpriteSize / 2);
			m_Window->draw(m_GoldText);
			m_Window->draw(m_CharacterNameText);
			updateSprite(m_ArmySprite, m_ArmyTexture, m_ArmyPosition, m_SpriteSize / 2);
			m_Window->draw(m_ArmyText);
			updateSprite(m_AgeSprite, m_AgeTexture, m_AgePosition, m_SpriteSize / 2);
			m_Window->draw(m_CharacterAgeText);
			if (m_Gender == Gender::Male)
			{
				updateSprite(m_GenderSprite, m_MaleGenderTexture, m_GenderPosition, m_SpriteSize / 2);
			}
			else
			{
				updateSprite(m_GenderSprite, m_FemaleGenderTexture, m_GenderPosition, m_SpriteSize / 2);
				if (m_Pregnant)
				{
					updateSprite(m_Pregnantprite, m_PregnantTexture, m_PregnantPosition, m_SpriteSize / 2);
				}
			}
			if (m_SpouseID != INVALID_CHARACTER_ID)
			{
				updateSprite(m_MarriedSprite, m_MarriedTexture, m_MarriedPosition, m_SpriteSize / 2);
				if (m_ShowSpouseName)
				{
					m_Window->draw(m_SpouseName);
				}
			}
			for (unsigned int index = 0; index < m_ChildrenShapes.size(); index++)
			{
				m_Window->draw(m_ChildrenShapes[index]);
				updateSprite(m_ChildrenSprites[index], m_ChildrenTextures[index], sf::Vector2f(m_SizeX * 0.1f, m_SizeY * 0.05f * (index + 1) + m_SizeY * 0.45f), m_SpriteSize / 2);
				if (m_ShowChildrenNames.size() > 0 && m_ShowChildrenNames[index])
				{
					m_Window->draw(m_ChildrenNames[index]);
				}
			}
			if (!m_IsPlayerCharacter)
			{
				m_Window->draw(m_DeclareWarShape);
				m_Window->draw(m_DeclareWarText);
				m_Window->draw(m_MakePeaceShape);
				m_Window->draw(m_MakePeaceText);
				m_Window->draw(m_AllianceShape);
				m_Window->draw(m_AllianceText);
				m_Window->draw(m_MarriageShape);
				m_Window->draw(m_MarriageText);
			}
		}
	}

	void clickOnMap()
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
					m_CurrentRegionID = regionID;
					m_CurrentCharacterID = Map::get().getRegionById(m_CurrentRegionID).m_OwnerID;
					checkIfPlayerCharacter();
					updateInfo();
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

	void onMonthChange()
	{
		updateInfo();
	}

	void updateInfo()
	{
		if (m_CurrentCharacterID != INVALID_CHARACTER_ID)
		{
			m_PlayerCharacter = &CharacterManager::get()->getPlayerCharacter();
			Character& character = CharacterManager::get()->getCharacter(m_CurrentCharacterID);
			
			if (character.m_CharacterTitle != Title::Unlanded)
			{
				m_OwnerColor = character.m_RegionColor;
				m_RealmNameText.setString(character.m_KingdomName);
				m_RealmNameText.setFillColor(m_OwnerColor);
			}
			else if (character.m_Father != INVALID_CHARACTER_ID)
			{
				Character& father = CharacterManager::get()->getCharacter(character.m_Father);
				m_OwnerColor = father.m_RegionColor;
				m_RealmNameText.setString(father.m_KingdomName);
				m_RealmNameText.setFillColor(m_OwnerColor);
			}
			else
			{
				m_OwnerColor = character.m_RegionColor;
			}

			m_Gender = character.m_Gender;
			m_SpouseID = character.m_Spouse;
			if (m_SpouseID != INVALID_CHARACTER_ID)
			{
				Character& spouse = CharacterManager::get()->getCharacter(character.m_Spouse);
				m_SpouseName.setString(spouse.m_Name);
				m_SpouseName.setFillColor(spouse.m_RegionColor);
			}

			m_WindowShape.setOutlineColor(m_OwnerColor);

			m_ChildrenShapes.clear();
			m_ChildrenSprites.clear();
			m_ChildrenTextures.clear();
			m_ChildrenNames.clear();
			for (unsigned int index = 0; index < character.m_Children.size(); index++)
			{
				Character& child = CharacterManager::get()->getCharacter(character.m_Children[index]);
				m_ChildrenShapes.push_back(sf::RectangleShape());
				m_ChildrenSprites.push_back(sf::Sprite());
				m_ChildrenNames.push_back(sf::Text());
				if (child.m_Gender == Gender::Male)
				{
					m_ChildrenTextures.push_back(m_MaleBabyTexture);
				}
				else
				{
					m_ChildrenTextures.push_back(m_FemaleBabyTexture);
				}
				m_ChildrenShapes[index].setFillColor(sf::Color::Transparent);
				m_ChildrenShapes[index].setOutlineThickness(m_OutlineThickness * 0.5f);
				m_ChildrenShapes[index].setSize(sf::Vector2f(m_SpriteSize / 2, m_SpriteSize / 2));
				m_ChildrenNames[index].setString(child.m_Name);
				m_ChildrenNames[index].setFont(m_Font);
				m_ChildrenNames[index].setCharacterSize(m_CharacterSize / 2);
				m_ChildrenNames[index].setStyle(m_Style);
				if (character.m_CharacterID == child.m_Mother)
				{
					Character& father = CharacterManager::get()->getCharacter(child.m_Father);
					m_ChildrenShapes[index].setOutlineColor(father.m_RegionColor);
					m_ChildrenNames[index].setFillColor(father.m_RegionColor);
				}
				else
				{
					Character& mother = CharacterManager::get()->getCharacter(child.m_Mother);
					m_ChildrenShapes[index].setOutlineColor(mother.m_RegionColor);
					m_ChildrenNames[index].setFillColor(mother.m_RegionColor);
				}
			}

			if (m_Gender == Gender::Male)
			{
				m_Pregnant = false;
				if (character.m_CharacterTitle != Title::Unlanded)
				{
					m_CharacterNameText.setString(m_MaleTitles[(unsigned int)character.m_CharacterTitle] + character.m_Name);
				}
				else
				{
					m_CharacterNameText.setString(character.m_Name);
				}
			}
			else
			{
				m_Pregnant = CharacterManager::get()->hasTrait(character.m_CharacterID, CharacterManager::get()->getTrait(m_PregnantTrait));
				if (character.m_CharacterTitle != Title::Unlanded)
				{
					m_CharacterNameText.setString(m_FemaleTitles[(unsigned int)character.m_CharacterTitle] + character.m_Name);
				}
				else
				{
					m_CharacterNameText.setString(character.m_Name);
				}
			}
			m_CharacterNameText.setFillColor(m_OwnerColor);

			m_ArmyText.setString(std::to_string(character.m_RaisedArmySize) + m_Dash + std::to_string(character.m_MaxArmySize));
			m_ArmyText.setFillColor(m_OwnerColor);

			std::stringstream stream;
			stream << std::fixed << std::setprecision(1) << character.m_CurrentGold;
			m_GoldText.setString(stream.str());
			m_GoldText.setFillColor(m_OwnerColor);

			m_CharacterAgeText.setString(std::to_string(Time::m_GameDate.getAge(character.m_Birthday)));
			m_CharacterAgeText.setFillColor(m_OwnerColor);
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
		if (InputHandler::getLeftMouseReleased())
		{
			Vector2D mousePosition = InputHandler::getMousePosition();
			if (m_MarriedSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				m_ShowSpouseName = true;
			}
			else
			{
				m_ShowSpouseName = false;
			}
			for (unsigned int index = 0; index < m_ChildrenShapes.size(); index++)
			{
				if (index + 1 > m_ShowChildrenNames.size())
				{
					m_ShowChildrenNames.push_back(false);
				}
				if (m_ChildrenShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
				{
					m_ShowChildrenNames[index] = true;
				}
				else
				{
					m_ShowChildrenNames[index] = false;
				}
			}
		}
		if (InputHandler::getRightMouseReleased())
		{
			Vector2D mousePosition = InputHandler::getMousePosition();
			if (m_MarriedSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				m_CurrentCharacterID = m_SpouseID;
				checkIfPlayerCharacter();
				updateInfo();
			}
			for (unsigned int index = 0; index < m_ChildrenShapes.size(); index++)
			{
				if (m_ChildrenShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
				{
					m_CurrentCharacterID = CharacterManager::get()->getCharacter(m_CurrentCharacterID).m_Children[index];
					checkIfPlayerCharacter();
					updateInfo();
					break;
				}
			}
		}
		if (InputHandler::getLeftMouseReleased() && !m_IsPlayerCharacter)
		{
			Vector2D mousePosition = InputHandler::getMousePosition();
			if (m_DeclareWarShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				if (m_PlayerWars.find(m_CurrentCharacterID) == m_PlayerWars.end())
				{
					int warHandle = WarManager::get().createWar(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, m_CurrentRegionID);
					m_PlayerWars.insert(std::pair(m_CurrentCharacterID, warHandle));
					m_CurrentWars++;

					CharacterManager::get()->getCharacter(m_CurrentCharacterID).m_CurrentWars.push_back(warHandle);
					AIManager::get().GetWarmindOfCharacter(m_CurrentCharacterID).m_Active = true;
					AIManager::get().GetWarmindOfCharacter(m_CurrentCharacterID).m_Opponent = m_PlayerCharacter->m_CharacterID;

					Game::m_Sound.pause();
					if (m_BattleSound.getStatus() != sf::SoundSource::Playing)
					{
						m_BattleSound.play();
					}
				}
			}
			else if (m_MakePeaceShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				if (!m_PlayerWars.empty() && m_PlayerWars.find(m_CurrentCharacterID) != m_PlayerWars.end())
				{
					WarManager::get().endWar(m_PlayerWars.at(m_CurrentCharacterID), WarManager::get().getWar(m_PlayerWars.at(m_CurrentCharacterID))->m_Attacker);
					m_PlayerWars.erase(m_CurrentCharacterID);
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
			else if (m_MarriageShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				CharacterManager::get()->marry(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID);
			}
			else if (m_AllianceShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{

			}
		}
	}

	bool checkIfPlayerCharacter()
	{
		return m_IsPlayerCharacter = m_CurrentCharacterID == m_PlayerCharacter->m_CharacterID;
	}

	void updateSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, int spriteSize = m_SpriteSize)
	{
		sprite.setTexture(texture, true);
		sprite.setPosition(Window::getWindow()->mapPixelToCoords(sf::Vector2i((int)position.x, (int)position.y)));

		sf::FloatRect localSize = sprite.getLocalBounds();

		sprite.setScale(spriteSize / localSize.width, spriteSize / localSize.height);

		m_Window->draw(sprite);
	}
};