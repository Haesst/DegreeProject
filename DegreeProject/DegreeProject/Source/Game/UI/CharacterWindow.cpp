#include "Game/UI/CharacterWindow.h"
#include <SFML/Graphics.hpp>
#include "Engine/Window.h"
#include "Engine/InputHandler.h"
#include "Engine/AssetHandler.h"
#include "Game/Data/Character.h"
#include "Game/Map/Map.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/Game.h"
#include <iomanip>
#include <sstream>
#include "Game/AI/AIManager.h"
#include "Game/WarManager.h"
#include "Game/Data/AIData.h"
#include "Game/UI/UIManager.h"

CharacterWindow::CharacterWindow(UIID id, sf::Font font, Vector2D, Vector2D size)
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

	m_AlliesShapes.clear();
	m_AlliesSprites.clear();
	m_AlliesTextures.clear();
	m_AlliesPositions.clear();

	m_WarShapes.clear();
	m_WarSprites.clear();
	m_WarTextures.clear();
	m_WarPositions.clear();
}

void CharacterWindow::start()
{
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

	m_DeadTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Dead.png");
	m_DeadPosition = sf::Vector2f(m_SizeX * 0.025f, m_SizeY * 0.025f);

	m_MaleGenderTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Male.png");
	m_FemaleGenderTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Female.png");
	m_GenderPosition = sf::Vector2f(m_SizeX * 0.3f, m_SizeY * 0.42f);

	m_PregnantTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Pregnant.png");
	m_PregnantPosition = sf::Vector2f(m_SizeX * 0.5f, m_SizeY * 0.42f);

	m_MaleChildTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/BabyMale.png");
	m_FemaleChildTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/BabyFemale.png");

	m_MotherTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Mother.png");
	m_FatherTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Father.png");

	m_FatherPosition = sf::Vector2f(m_SizeX * 0.4f, m_SizeY * 0.52f);
	m_MotherPosition = sf::Vector2f(m_SizeX * 0.4f, m_SizeY * 0.57f);

	m_FatherShape.setFillColor(sf::Color::Transparent);
	m_FatherShape.setOutlineThickness(m_OutlineThickness * 0.5f);
	m_FatherShape.setSize(sf::Vector2f(m_SpriteSize / 2, m_SpriteSize / 2));

	m_MotherShape.setFillColor(sf::Color::Transparent);
	m_MotherShape.setOutlineThickness(m_OutlineThickness * 0.5f);
	m_MotherShape.setSize(sf::Vector2f(m_SpriteSize / 2, m_SpriteSize / 2));

	m_FatherName.setFont(m_Font);
	m_FatherName.setCharacterSize(m_CharacterSize / 2);
	m_FatherName.setStyle(m_Style);

	m_MotherName.setFont(m_Font);
	m_MotherName.setCharacterSize(m_CharacterSize / 2);
	m_MotherName.setStyle(m_Style);

	m_MarriedTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Married.png");
	m_MarriedPosition = sf::Vector2f(m_SizeX * 0.4f, m_SizeY * 0.42f);

	m_SpouseName.setFont(m_Font);
	m_SpouseName.setCharacterSize(m_CharacterSize / 2);
	m_SpouseName.setStyle(m_Style);

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

	m_AssassinateShape.setFillColor(sf::Color::Transparent);
	m_AssassinateShape.setOutlineColor(m_DeclareWarColor);
	m_AssassinateShape.setOutlineThickness(m_OutlineThickness);
	m_AssassinateShape.setSize(sf::Vector2f(m_SizeX * 0.2f, m_SizeY * 0.05f));

	m_AssassinateText.setFont(m_Font);
	m_AssassinateText.setCharacterSize((int)(m_CharacterSize * 0.5f));
	m_AssassinateText.setStyle(m_Style);
	m_AssassinateText.setString(m_Assassinate);
	m_AssassinateText.setFillColor(m_DeclareWarColor);

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

void CharacterWindow::update()
{
	clickOnMap();
	handleWindow();

	if (m_Visible)
	{
		clickButton();

		m_WindowShape.setPosition(sf::Vector2f((int)m_OutlineThickness, (int)m_OutlineThickness));

		m_DeclareWarShape.setPosition(sf::Vector2f((int)(m_SizeX * 0.775f), (int)(m_SizeY * 0.2f)));
		m_DeclareWarText.setPosition(sf::Vector2f((int)(m_SizeX * 0.785f), (int)(m_SizeY * 0.208f)));

		m_MakePeaceShape.setPosition(sf::Vector2f((int)(m_SizeX * 0.775f), (int)(m_SizeY * 0.3f)));
		m_MakePeaceText.setPosition(sf::Vector2f((int)(m_SizeX * 0.785f), (int)(m_SizeY * 0.308f)));

		m_AllianceShape.setPosition(sf::Vector2f((int)(m_SizeX * 0.775f), (int)(m_SizeY * 0.4f)));
		m_AllianceText.setPosition(sf::Vector2f((int)(m_SizeX * 0.785f), (int)(m_SizeY * 0.408f)));

		m_MarriageShape.setPosition(sf::Vector2f((int)(m_SizeX * 0.775f), (int)(m_SizeY * 0.5f)));
		m_MarriageText.setPosition(sf::Vector2f((int)(m_SizeX * 0.785f), (int)(m_SizeY * 0.508f)));

		m_AssassinateShape.setPosition(sf::Vector2f((int)(m_SizeX * 0.775f), (int)(m_SizeY * 0.6f)));
		m_AssassinateText.setPosition(sf::Vector2f((int)(m_SizeX * 0.785f), (int)(m_SizeY * 0.608f)));

		m_CharacterNameText.setPosition(sf::Vector2f((int)(m_SizeX * 0.25f), (int)(m_SizeY * 0.025f)));

		m_RealmNameText.setPosition(sf::Vector2f((int)(m_SizeX * 0.1f), (int)(m_SizeY * 0.1f)));

		m_ArmyText.setPosition(sf::Vector2f((int)(m_SizeX * 0.2f), (int)(m_SizeY * 0.2f)));

		m_GoldText.setPosition(sf::Vector2f((int)(m_SizeX * 0.2f), (int)(m_SizeY * 0.3f)));

		m_CharacterAgeText.setPosition(sf::Vector2f((int)(m_SizeX * 0.2f), (int)(m_SizeY * 0.4f)));

		for (unsigned int index = 0; index < m_ChildrenShapes.size(); index++)
		{
			m_ChildrenShapes[index].setPosition(sf::Vector2f((int)(m_SizeX * 0.1f), (int)(m_SizeY * 0.05f * (index + 1) + m_SizeY * 0.45f)));
			m_ChildrenNames[index].setPosition(sf::Vector2f((int)(m_SizeX * 0.2f), (int)(m_SizeY * 0.05f * (index + 1) + m_SizeY * 0.45f)));
		}

		for (unsigned int index = 0; index < m_AlliesShapes.size(); index++)
		{
			m_AlliesShapes[index].setPosition(sf::Vector2f((int)(m_SizeX * 0.2f), (int)(m_SizeY * 0.05f * (index + 1) + m_SizeY * 0.45f)));
		}

		for (unsigned int index = 0; index < m_WarShapes.size(); index++)
		{
			m_WarShapes[index].setPosition(sf::Vector2f((int)(m_SizeX * 0.3f), (int)(m_SizeY * 0.05f * (index + 1) + m_SizeY * 0.45f)));
		}

		m_SpouseName.setPosition(sf::Vector2f((int)(m_MarriedPosition.x + m_SizeX * 0.1f), (int)(m_MarriedPosition.y)));

		m_FatherShape.setPosition(sf::Vector2f((int)(m_FatherPosition.x), (int)(m_FatherPosition.y)));
		m_FatherName.setPosition(sf::Vector2f((int)(m_FatherPosition.x + m_SizeX * 0.1f), (int)(m_FatherPosition.y)));

		m_MotherShape.setPosition(sf::Vector2f((int)(m_MotherPosition.x), (int)(m_MotherPosition.y)));
		m_MotherName.setPosition(sf::Vector2f((int)(m_MotherPosition.x + m_SizeX * 0.1f), (int)(m_MotherPosition.y)));
	}
}

void CharacterWindow::render()
{
	if (m_Visible)
	{
		m_Window->draw(m_WindowShape);
		m_Window->draw(m_RealmNameText);
		if (m_Dead)
		{
			updateSprite(m_DeadSprite, m_DeadTexture, m_DeadPosition, m_SpriteSize / 2);
		}
		updateSprite(m_GoldSprite, m_GoldTexture, m_GoldPosition, m_SpriteSize / 2);
		m_Window->draw(m_GoldText);
		m_Window->draw(m_CharacterNameText);
		updateSprite(m_ArmySprite, m_ArmyTexture, m_ArmyPosition, m_SpriteSize / 2);
		m_Window->draw(m_ArmyText);
		updateSprite(m_AgeSprite, m_AgeTexture, m_AgePosition, m_SpriteSize / 2);
		m_Window->draw(m_CharacterAgeText);
		if (m_Gender == Gender::Male)
		{
			updateSprite(m_CharacterSprite, m_MaleCharacterTexture, m_CharacterPosition);
			updateSprite(m_GenderSprite, m_MaleGenderTexture, m_GenderPosition, m_SpriteSize / 2);
		}
		else
		{
			updateSprite(m_CharacterSprite, m_FemaleCharacterTexture, m_CharacterPosition);
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
		m_Window->draw(m_FatherShape);
		if (m_FatherID != INVALID_CHARACTER_ID)
		{
			updateSprite(m_FatherSprite, m_FatherTexture, m_FatherPosition, m_SpriteSize / 2);
			if (m_ShowFatherName)
			{
				m_Window->draw(m_FatherName);
			}
		}
		m_Window->draw(m_MotherShape);
		if (m_MotherID != INVALID_CHARACTER_ID)
		{
			updateSprite(m_MotherSprite, m_MotherTexture, m_MotherPosition, m_SpriteSize / 2);
			if (m_ShowMotherName)
			{
				m_Window->draw(m_MotherName);
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
		for (unsigned int index = 0; index < m_AlliesShapes.size(); index++)
		{
			m_Window->draw(m_AlliesShapes[index]);
			updateSprite(m_AlliesSprites[index], m_AlliesTextures[index], sf::Vector2f(m_SizeX * 0.2f, m_SizeY * 0.05f * (index + 1) + m_SizeY * 0.45f), m_SpriteSize / 2);
		}
		for (unsigned int index = 0; index < m_WarShapes.size(); index++)
		{
			m_Window->draw(m_WarShapes[index]);
			updateSprite(m_WarSprites[index], m_WarTextures[index], sf::Vector2f(m_SizeX * 0.3f, m_SizeY * 0.05f * (index + 1) + m_SizeY * 0.45f), m_SpriteSize / 2);
		}
		if (!m_IsPlayerCharacter)
		{
			if (m_CurrentCharacter->m_CharacterTitle != Title::Unlanded)
			{
				m_Window->draw(m_DeclareWarShape);
				m_Window->draw(m_DeclareWarText);
				m_Window->draw(m_MakePeaceShape);
				m_Window->draw(m_MakePeaceText);
				m_Window->draw(m_AllianceShape);
				m_Window->draw(m_AllianceText);
			}
			m_Window->draw(m_MarriageShape);
			m_Window->draw(m_MarriageText);
			m_Window->draw(m_AssassinateShape);
			m_Window->draw(m_AssassinateText);
		}
	}
}

void CharacterWindow::clickOnMap()
{
	if (InputHandler::getRightMouseClicked() && !InputHandler::getPlayerUnitSelected())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
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
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (!m_WindowShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			closeWindow();
		}
	}
}

void CharacterWindow::onDayChange()
{
	if (m_Visible)
	{
		updateInfo();
	}
}

void CharacterWindow::updateInfo()
{
	if (m_CurrentCharacterID != INVALID_CHARACTER_ID)
	{
		m_PlayerCharacter = &CharacterManager::get()->getPlayerCharacter();
		m_CurrentCharacter = &CharacterManager::get()->getCharacter(m_CurrentCharacterID);
		m_FatherID = m_CurrentCharacter->m_Father;
		m_MotherID = m_CurrentCharacter->m_Mother;
		m_Dead = m_CurrentCharacter->m_Dead;

		if (m_CurrentCharacter->m_CharacterTitle != Title::Unlanded)
		{
			m_OwnerColor = m_CurrentCharacter->m_RegionColor;
			m_RealmNameText.setString(m_CurrentCharacter->m_KingdomName);
			m_RealmNameText.setFillColor(m_OwnerColor);
		}
		else if (m_FatherID != INVALID_CHARACTER_ID)
		{
			Character& father = CharacterManager::get()->getCharacter(m_FatherID);
			m_OwnerColor = father.m_RegionColor;
			m_RealmNameText.setString(father.m_KingdomName);
			m_RealmNameText.setFillColor(m_OwnerColor);
		}
		else
		{
			m_OwnerColor = m_CurrentCharacter->m_RegionColor;
		}

		if (m_FatherID != INVALID_CHARACTER_ID)
		{
			Character& father = CharacterManager::get()->getCharacter(m_FatherID);
			m_FatherName.setFillColor(father.m_RegionColor);
			m_FatherName.setString(father.m_Name);
			m_FatherShape.setOutlineColor(father.m_RegionColor);
		}
		else
		{
			m_FatherShape.setOutlineColor(m_OwnerColor);
		}

		if (m_MotherID != INVALID_CHARACTER_ID)
		{
			Character& mother = CharacterManager::get()->getCharacter(m_MotherID);
			m_MotherName.setFillColor(mother.m_RegionColor);
			m_MotherName.setString(mother.m_Name);
			m_MotherShape.setOutlineColor(mother.m_RegionColor);
		}
		else
		{
			m_MotherShape.setOutlineColor(m_OwnerColor);
		}

		m_Gender = m_CurrentCharacter->m_Gender;

		m_SpouseID = m_CurrentCharacter->m_Spouse;
		if (m_SpouseID != INVALID_CHARACTER_ID)
		{
			Character& spouse = CharacterManager::get()->getCharacter(m_SpouseID);
			m_SpouseName.setString(spouse.m_Name);
			m_SpouseName.setFillColor(spouse.m_RegionColor);
		}

		m_WindowShape.setOutlineColor(m_OwnerColor);

		m_ChildrenShapes.clear();
		m_ChildrenSprites.clear();
		m_ChildrenTextures.clear();
		m_ChildrenNames.clear();
		for (unsigned int index = 0; index < m_CurrentCharacter->m_Children.size(); index++)
		{
			Character& child = CharacterManager::get()->getCharacter(m_CurrentCharacter->m_Children[index]);
			m_ChildrenShapes.push_back(sf::RectangleShape());
			m_ChildrenSprites.push_back(sf::Sprite());
			m_ChildrenNames.push_back(sf::Text());
			if (child.m_Gender == Gender::Male)
			{
				m_ChildrenTextures.push_back(m_MaleChildTexture);
			}
			else
			{
				m_ChildrenTextures.push_back(m_FemaleChildTexture);
			}
			m_ChildrenShapes[index].setOutlineThickness(m_OutlineThickness * 0.5f);
			m_ChildrenShapes[index].setSize(sf::Vector2f(m_SpriteSize / 2, m_SpriteSize / 2));
			m_ChildrenNames[index].setString(child.m_Name);
			m_ChildrenNames[index].setFont(m_Font);
			m_ChildrenNames[index].setCharacterSize(m_CharacterSize / 2);
			m_ChildrenNames[index].setStyle(m_Style);
			if (child.m_CharacterTitle != Title::Unlanded)
			{
				m_ChildrenShapes[index].setOutlineColor(child.m_RegionColor);
				m_ChildrenNames[index].setFillColor(child.m_RegionColor);
			}
			else if (m_CurrentCharacter->m_CharacterID == child.m_Mother)
			{
				Character& fatherChild = CharacterManager::get()->getCharacter(child.m_Father);
				m_ChildrenShapes[index].setOutlineColor(fatherChild.m_RegionColor);
				m_ChildrenNames[index].setFillColor(fatherChild.m_RegionColor);
			}
			else
			{
				Character& motherChild = CharacterManager::get()->getCharacter(child.m_Mother);
				m_ChildrenShapes[index].setOutlineColor(motherChild.m_RegionColor);
				m_ChildrenNames[index].setFillColor(motherChild.m_RegionColor);
			}
		}

		m_AlliesShapes.clear();
		m_AlliesSprites.clear();
		m_AlliesTextures.clear();
		m_AlliesPositions.clear();
		for (unsigned int index = 0; index < WarManager::get().getAlliances(m_CurrentCharacterID).size(); index++)
		{
			Character& ally = CharacterManager::get()->getCharacter(WarManager::get().getAlliances(m_CurrentCharacterID)[index]);
			m_AlliesShapes.push_back(sf::RectangleShape());
			m_AlliesSprites.push_back(sf::Sprite());
			if (ally.m_Gender == Gender::Male)
			{
				m_AlliesTextures.push_back(m_MaleCharacterTexture);
			}
			else
			{
				m_AlliesTextures.push_back(m_FemaleCharacterTexture);
			}
			m_AlliesShapes[index].setOutlineColor(ally.m_RegionColor);
			m_AlliesShapes[index].setOutlineThickness(m_OutlineThickness * 0.5f);
			m_AlliesShapes[index].setSize(sf::Vector2f(m_SpriteSize / 2, m_SpriteSize / 2));
		}

		//m_WarShapes.clear();
		//m_WarSprites.clear();
		//m_WarTextures.clear();
		//m_WarPositions.clear();
		//for (unsigned int index = 0; index < m_CurrentCharacter->m_CurrentWars.size(); index++)
		//{
		//	Character& ally = CharacterManager::get()->getCharacter(WarManager::get().getAlliances(m_CurrentCharacterID)[index]);
		//	m_AlliesShapes.push_back(sf::RectangleShape());
		//	m_AlliesSprites.push_back(sf::Sprite());
		//	if (ally.m_Gender == Gender::Male)
		//	{
		//		m_AlliesTextures.push_back(m_MaleCharacterTexture);
		//	}
		//	else
		//	{
		//		m_AlliesTextures.push_back(m_FemaleCharacterTexture);
		//	}
		//	m_AlliesShapes[index].setOutlineColor(ally.m_RegionColor);
		//	m_AlliesShapes[index].setOutlineThickness(m_OutlineThickness * 0.5f);
		//	m_AlliesShapes[index].setSize(sf::Vector2f(m_SpriteSize / 2, m_SpriteSize / 2));
		//}

		if (m_Gender == Gender::Male)
		{
			m_Pregnant = false;
			if (m_CurrentCharacter->m_CharacterTitle != Title::Unlanded)
			{
				m_CharacterNameText.setString(m_MaleTitles[(unsigned int)m_CurrentCharacter->m_CharacterTitle] + m_CurrentCharacter->m_Name);
			}
			else
			{
				m_CharacterNameText.setString(m_CurrentCharacter->m_Name);
			}
		}
		else
		{
			m_Pregnant = CharacterManager::get()->hasTrait(m_CurrentCharacterID, CharacterManager::get()->getTrait(m_PregnantTrait));
			if (m_CurrentCharacter->m_CharacterTitle != Title::Unlanded)
			{
				m_CharacterNameText.setString(m_FemaleTitles[(unsigned int)m_CurrentCharacter->m_CharacterTitle] + m_CurrentCharacter->m_Name);
			}
			else
			{
				m_CharacterNameText.setString(m_CurrentCharacter->m_Name);
			}
		}
		m_CharacterNameText.setFillColor(m_OwnerColor);

		m_ArmyText.setString(std::to_string(m_CurrentCharacter->m_RaisedArmySize) + m_Dash + std::to_string(m_CurrentCharacter->m_MaxArmySize));
		m_ArmyText.setFillColor(m_OwnerColor);

		std::stringstream stream;
		stream << std::fixed << std::setprecision(1) << m_CurrentCharacter->m_CurrentGold;
		m_GoldText.setString(stream.str());
		m_GoldText.setFillColor(m_OwnerColor);

		m_CharacterAgeText.setString(std::to_string(Time::m_GameDate.getAge(m_CurrentCharacter->m_Birthday)));
		m_CharacterAgeText.setFillColor(m_OwnerColor);
	}
}

void CharacterWindow::handleWindow()
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

void CharacterWindow::openWindow()
{
	if (!m_Visible)
	{
		m_WindowShape.setSize(sf::Vector2f(m_SizeX, m_SizeY));
		m_DaySubscriptionHandle = Time::m_GameDate.subscribeToDayChange([](void* data) { CharacterWindow& characterWindow = *static_cast<CharacterWindow*>(data); characterWindow.onDayChange(); }, static_cast<void*>(this));
		m_Visible = true;
	}
}

void CharacterWindow::closeWindow()
{
	if (m_Visible)
	{
		Time::m_GameDate.unsubscribeToDayChange(m_DaySubscriptionHandle);
		m_WindowShape.setSize(sf::Vector2f());
		m_Open = false;
		m_Visible = false;
	}
}

void CharacterWindow::clickButton()
{
	if (InputHandler::getLeftMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (m_MarriedSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			m_ShowSpouseName = true;
		}
		else
		{
			m_ShowSpouseName = false;
		}
		if (m_FatherSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			m_ShowFatherName = true;
		}
		else
		{
			m_ShowFatherName = false;
		}
		if (m_MotherSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			m_ShowMotherName = true;
		}
		else
		{
			m_ShowMotherName = false;
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
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (m_MarriedSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			m_CurrentCharacterID = m_SpouseID;
			checkIfPlayerCharacter();
			updateInfo();
		}
		if (m_FatherSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			m_CurrentCharacterID = m_FatherID;
			checkIfPlayerCharacter();
			updateInfo();
		}
		if (m_MotherSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			m_CurrentCharacterID = m_MotherID;
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
		for (unsigned int index = 0; index < m_AlliesShapes.size(); index++)
		{
			if (m_AlliesShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				m_CurrentCharacterID = WarManager::get().getAlliances(m_CurrentCharacterID)[index];
				checkIfPlayerCharacter();
				updateInfo();
				break;
			}
		}
		for (unsigned int index = 0; index < m_WarShapes.size(); index++)
		{
			if (m_WarShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				//UIManager::get()->m_WarWindow->openWindow()
				break;
			}
		}
	}
	if (InputHandler::getLeftMouseReleased() && !m_IsPlayerCharacter)
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		if (m_DeclareWarShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			for (auto& ally : WarManager::get().getAlliances(m_PlayerCharacter->m_CharacterID))
			{
				if (ally == m_CurrentCharacterID)
				{
					UIManager::get()->createUIEventElement(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, UIType::TriedToDeclareWarOnAlly);
					return;
				}
			}
			War* war = WarManager::get().getWarAgainst(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID);
			if (war == nullptr)
			{
				int warHandle = WarManager::get().createWar(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, m_CurrentRegionID);

				m_PlayerCharacter->m_CurrentWars.push_back(warHandle);
				m_CurrentCharacter->m_CurrentWars.push_back(warHandle);
				AIManager::get().getWarmindOfCharacter(m_CurrentCharacterID).m_Active = true;
				AIManager::get().getWarmindOfCharacter(m_CurrentCharacterID).m_Opponent = m_PlayerCharacter->m_CharacterID;
				AIManager::get().getAIDataofCharacter(m_CurrentCharacterID).m_CurrentAction = Action::War;
				AIManager::get().getAIDataofCharacter(m_CurrentCharacterID).m_LastAction = Action::War;

				UIManager::get()->createUIEventElement(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, UIType::WarDeclaration);
				UIManager::get()->createWarIcon(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID);

				CharacterManager::get()->callAllies(m_PlayerCharacter->m_CharacterID, warHandle);

				Game::m_Sound.pause();
				if (Game::m_BattleSound.getStatus() != sf::SoundSource::Playing)
				{
					Game::m_BattleSound.play();
				}
			}
		}
		else if (m_MakePeaceShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			if (!m_PlayerCharacter->m_CurrentWars.empty())
			{
				CharacterManager::get()->sendPeaceOffer(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, PeaceType::Enforce_Demands);

				if (Game::m_BattleSound.getStatus() == sf::SoundSource::Playing && m_PlayerCharacter->m_CurrentWars.size() == 0)
				{
					Game::m_BattleSound.stop();
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
			CharacterManager::get()->sendAllianceOffer(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID);
		}
		else if (m_AssassinateShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			if (CharacterManager::get()->chancePerPercent(0.5f))
			{
				CharacterManager::get()->killCharacter(m_CurrentCharacterID);
				UIManager::get()->createUIEventElement(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, UIType::AssassinationSuccess);
			}
			else
			{
				UIManager::get()->createUIEventElement(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, UIType::AssassinationFailure);
			}
		}
	}
}

bool CharacterWindow::checkIfPlayerCharacter()
{
	return m_IsPlayerCharacter = m_CurrentCharacterID == m_PlayerCharacter->m_CharacterID;
}

void CharacterWindow::updateSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, int spriteSize)
{
	sprite.setTexture(texture, true);
	sprite.setPosition(position);

	sf::FloatRect localSize = sprite.getLocalBounds();

	sprite.setScale(spriteSize / localSize.width, spriteSize / localSize.height);

	m_Window->draw(sprite);
}