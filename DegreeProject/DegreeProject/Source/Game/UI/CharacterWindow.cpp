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
#include "Game/UI/WarWindow.h"
#include "Game/Systems/HeraldicShieldManager.h"
#include "Game/UI/RegionWindow.h"

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

	for (unsigned int index = 0; index < m_NumberOfButtons; index++)
	{
		sf::RectangleShape buttonShape(sf::Vector2f(m_SizeX * 0.2f, m_SizeY * 0.05f));
		buttonShape.setFillColor(sf::Color::Transparent);
		buttonShape.setOutlineThickness(m_OutlineThickness);
		buttonShape.setOutlineColor(m_DeclareWarColor);
		buttonShape.setPosition(sf::Vector2f(m_SizeX * 0.775f, m_SizeY * (0.52f + 0.1f * index)));

		sf::Text buttonText(m_ButtonStrings[index], m_Font, m_CharacterSize);
		buttonText.setPosition(sf::Vector2f(m_SizeX * 0.785f, m_SizeY * (0.528f + 0.1f * index)));
		buttonText.setFillColor(m_DeclareWarColor);

		if (index != 0 && index != m_NumberOfButtons - 1)
		{
			buttonShape.setOutlineColor(m_MakePeaceColor);
			buttonText.setFillColor(m_MakePeaceColor);
		}

		m_ButtonShapes.push_back(buttonShape);
		m_ButtonTexts.push_back(buttonText);
	}

	AssetHandler& assetHandler = AssetHandler::get();

	for (unsigned int index = 0; index < m_NumberOfRelations; index++)
	{
		sf::Sprite relationSprite;
		switch (index)
		{
			case 0:
			{
				m_RegionTexture = assetHandler.getTextureAtPath("Assets/Graphics/Castle.png");
				relationSprite.setTexture(m_RegionTexture);
				break;
			}
			case 1:
			{
				m_ChildTexture = assetHandler.getTextureAtPath("Assets/Graphics/BabyMale.png");
				relationSprite.setTexture(m_ChildTexture);
				break;
			}
			case 2:
			{
				m_AllianceTexture = assetHandler.getTextureAtPath("Assets/Graphics/Alliance.png");
				relationSprite.setTexture(m_AllianceTexture);
				break;
			}
			case 3:
			{
				m_WarTexture = assetHandler.getTextureAtPath("Assets/Graphics/War.png");
				relationSprite.setTexture(m_WarTexture);
				break;
			}
			case 4:
			{
				m_ParentTexture = assetHandler.getTextureAtPath("Assets/Graphics/Father.png");
				relationSprite.setTexture(m_ParentTexture);
				break;
			}
			default:
			{
				break;
			}
		}
		relationSprite.setPosition(sf::Vector2f(m_SizeX * 0.1f + (m_SizeX * 0.1f * index), m_SizeY * 0.5f));
		relationSprite.setScale(m_SpriteSize / relationSprite.getLocalBounds().width, m_SpriteSize / relationSprite.getLocalBounds().height);
		
		sf::Text relationText(m_DiplomacyStrings[index], m_Font, m_CharacterSize);
		relationText.setPosition(relationSprite.getPosition());
		m_DiplomacyTexts.push_back(relationText);
		m_DiplomacySprites.push_back(relationSprite);
	}

	m_MaleCharacterTexture = assetHandler.getTextureAtPath("Assets/Graphics/MalePortrait.jpg");
	m_FemaleCharacterTexture = assetHandler.getTextureAtPath("Assets/Graphics/FemalePortrait.jpg");

	m_GoldTexture = assetHandler.getTextureAtPath("Assets/Graphics/Coins.png");
	m_GoldPosition = sf::Vector2f(m_SizeX * 0.1f, m_SizeY * 0.32f);

	m_ArmyTexture = assetHandler.getTextureAtPath("Assets/Graphics/soldier unit.png");
	m_ArmyPosition = sf::Vector2f(m_SizeX * 0.1f, m_SizeY * 0.22f);

	m_AgeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Age.png");
	m_AgePosition = sf::Vector2f(m_SizeX * 0.1f, m_SizeY * 0.42f);

	m_DeadTexture = assetHandler.getTextureAtPath("Assets/Graphics/Dead.png");
	m_DeadPosition = sf::Vector2f(m_SizeX * 0.025f, m_SizeY * 0.025f);

	m_MaleGenderTexture = assetHandler.getTextureAtPath("Assets/Graphics/Male.png");
	m_FemaleGenderTexture = assetHandler.getTextureAtPath("Assets/Graphics/Female.png");

	m_FertilityTexture = assetHandler.getTextureAtPath("Assets/Graphics/Fertility.png");
	m_BeautifulTexture = assetHandler.getTextureAtPath("Assets/Graphics/Beautiful.png");
	m_UglyTexture = assetHandler.getTextureAtPath("Assets/Graphics/Ugly.png");
	m_SterileTexture = assetHandler.getTextureAtPath("Assets/Graphics/Sterile.png");

	m_PregnantTexture = assetHandler.getTextureAtPath("Assets/Graphics/Pregnant.png");
	m_PregnantPosition = sf::Vector2f(m_SizeX * 0.5f, m_SizeY * 0.42f);

	m_MaleChildTexture = assetHandler.getTextureAtPath("Assets/Graphics/BabyMale.png");
	m_FemaleChildTexture = assetHandler.getTextureAtPath("Assets/Graphics/BabyFemale.png");

	m_MotherTexture = assetHandler.getTextureAtPath("Assets/Graphics/Mother.png");
	m_FatherTexture = assetHandler.getTextureAtPath("Assets/Graphics/Father.png");

	m_FatherPosition = sf::Vector2f(m_SizeX * 0.5f, m_SizeY * 0.55f);
	m_MotherPosition = sf::Vector2f(m_SizeX * 0.5f, m_SizeY * 0.60f);

	m_FatherShape.setFillColor(sf::Color::Transparent);
	m_FatherShape.setOutlineThickness(m_OutlineThickness * 0.5f);
	m_FatherShape.setSize(sf::Vector2f(m_SpriteSize, m_SpriteSize));
	m_FatherShape.setPosition(sf::Vector2f(m_FatherPosition.x, m_FatherPosition.y));

	m_MotherShape.setFillColor(sf::Color::Transparent);
	m_MotherShape.setOutlineThickness(m_OutlineThickness * 0.5f);
	m_MotherShape.setSize(sf::Vector2f(m_SpriteSize, m_SpriteSize));
	m_MotherShape.setPosition(sf::Vector2f(m_MotherPosition.x, m_MotherPosition.y));

	m_FatherName.setFont(m_Font);
	m_FatherName.setCharacterSize(m_CharacterSize);
	m_FatherName.setPosition(sf::Vector2f(m_FatherPosition.x + m_SizeX * 0.1f, m_FatherPosition.y));

	m_MotherName.setFont(m_Font);
	m_MotherName.setCharacterSize(m_CharacterSize);
	m_MotherName.setPosition(sf::Vector2f(m_MotherPosition.x + m_SizeX * 0.1f, m_MotherPosition.y));

	m_MarriedTexture = assetHandler.getTextureAtPath("Assets/Graphics/Married.png");
	m_MarriedPosition = sf::Vector2f(m_SizeX * 0.4f, m_SizeY * 0.42f);

	m_SpouseName.setFont(m_Font);
	m_SpouseName.setCharacterSize(m_CharacterSize);
	m_SpouseName.setPosition(sf::Vector2f(m_MarriedPosition.x + m_SizeX * 0.1f, m_MarriedPosition.y));

	m_WindowShape.setFillColor(m_FillColor);
	m_WindowShape.setOutlineThickness(m_OutlineThickness);
	m_WindowShape.setPosition(sf::Vector2f(m_OutlineThickness, m_OutlineThickness));

	m_CharacterNameText.setFont(m_Font);
	m_CharacterNameText.setCharacterSize(m_CharacterSize);
	m_CharacterNameText.setPosition(sf::Vector2f(m_SizeX * 0.25f, m_SizeY * 0.025f));

	m_RealmNameText.setFont(m_Font);
	m_RealmNameText.setCharacterSize(m_CharacterSize);
	m_RealmNameText.setPosition(sf::Vector2f(m_SizeX * 0.1f, m_SizeY * 0.1f));

	m_CharacterAgeText.setFont(m_Font);
	m_CharacterAgeText.setCharacterSize(m_CharacterSize);
	m_CharacterAgeText.setPosition(sf::Vector2f(m_SizeX * 0.2f, m_SizeY * 0.4f));

	m_ArmyText.setFont(m_Font);
	m_ArmyText.setCharacterSize(m_CharacterSize);
	m_ArmyText.setPosition(sf::Vector2f(m_SizeX * 0.2f, m_SizeY * 0.2f));

	m_GoldText.setFont(m_Font);
	m_GoldText.setCharacterSize(m_CharacterSize);
	m_GoldText.setPosition(sf::Vector2f(m_SizeX * 0.2f, m_SizeY * 0.3f));

	m_ArmySprite.setTexture(m_ArmyTexture);
	m_ArmySprite.setPosition(m_ArmyPosition);
	m_ArmySprite.setScale(m_SpriteSize / m_ArmySprite.getLocalBounds().width, m_SpriteSize / m_ArmySprite.getLocalBounds().height);

	m_DeadSprite.setTexture(m_DeadTexture);
	m_DeadSprite.setPosition(m_DeadPosition);
	m_DeadSprite.setScale(m_SpriteSize / m_DeadSprite.getLocalBounds().width, m_SpriteSize / m_DeadSprite.getLocalBounds().height);

	m_GoldSprite.setTexture(m_GoldTexture);
	m_GoldSprite.setPosition(m_GoldPosition);
	m_GoldSprite.setScale(m_SpriteSize / m_GoldSprite.getLocalBounds().width, m_SpriteSize / m_GoldSprite.getLocalBounds().height);

	m_AgeSprite.setTexture(m_AgeTexture);
	m_AgeSprite.setPosition(m_AgePosition);
	m_AgeSprite.setScale(m_SpriteSize / m_AgeSprite.getLocalBounds().width, m_SpriteSize / m_AgeSprite.getLocalBounds().height);

	m_MarriedSprite.setTexture(m_MarriedTexture);
	m_MarriedSprite.setPosition(m_MarriedPosition);
	m_MarriedSprite.setScale(m_SpriteSize / m_MarriedSprite.getLocalBounds().width, m_SpriteSize / m_MarriedSprite.getLocalBounds().height);

	m_FatherSprite.setTexture(m_FatherTexture);
	m_FatherSprite.setPosition(m_FatherPosition);
	m_FatherSprite.setScale(m_SpriteSize / m_FatherSprite.getLocalBounds().width, m_SpriteSize / m_FatherSprite.getLocalBounds().height);

	m_MotherSprite.setTexture(m_MotherTexture);
	m_MotherSprite.setPosition(m_MotherPosition);
	m_MotherSprite.setScale(m_SpriteSize / m_MotherSprite.getLocalBounds().width, m_SpriteSize / m_MotherSprite.getLocalBounds().height);

	m_PregnantSprite.setTexture(m_PregnantTexture);
	m_PregnantSprite.setPosition(m_PregnantPosition);
	m_PregnantSprite.setScale(m_SpriteSize / m_PregnantSprite.getLocalBounds().width, m_SpriteSize / m_PregnantSprite.getLocalBounds().height);

	clearInfo();
}

void CharacterWindow::start()
{
	m_Window = Window::getWindow();

	m_PlayerCharacter = &CharacterManager::get()->getPlayerCharacter();
}

void CharacterWindow::update()
{
	clickOnMap();
	handleWindow();

	if (m_Visible)
	{
		clickButton();
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
			m_Window->draw(m_DeadSprite);
		}

		m_Window->draw(m_GoldSprite);
		m_Window->draw(m_GoldText);
		m_Window->draw(m_CharacterNameText);
		m_Window->draw(m_ArmySprite);
		m_Window->draw(m_ArmyText);
		m_Window->draw(m_AgeSprite);
		m_Window->draw(m_CharacterAgeText);

		m_Window->draw(m_CharacterSprite);

		if (m_Pregnant)
		{
			m_Window->draw(m_PregnantSprite);
		}

		if (m_SpouseID != INVALID_CHARACTER_ID)
		{
			m_Window->draw(m_MarriedSprite);
			if (m_ShowSpouseName)
			{
				m_Window->draw(m_SpouseName);
			}
		}

		m_Window->draw(m_FatherShape);
		if (m_FatherID != INVALID_CHARACTER_ID)
		{
			m_Window->draw(m_FatherSprite);
			if (m_ShowFatherName)
			{
				m_Window->draw(m_FatherName);
			}
		}

		m_Window->draw(m_MotherShape);
		if (m_MotherID != INVALID_CHARACTER_ID)
		{
			m_Window->draw(m_MotherSprite);
			if (m_ShowMotherName)
			{
				m_Window->draw(m_MotherName);
			}
		}

		for (unsigned int index = 0; index < m_NumberOfRelations; index++)
		{
			m_Window->draw(m_DiplomacySprites[index]);
			if (m_DiplomacyShowInfo.size() > 0 && m_DiplomacyShowInfo[index])
			{
				m_Window->draw(m_DiplomacyTexts[index]);
			}
		}

		for (unsigned int index = 0; index < m_ChildrenShapes.size(); index++)
		{
			m_Window->draw(m_ChildrenShapes[index]);
			m_Window->draw(m_ChildrenSprites[index]);
			if (m_ShowChildrenNames.size() > 0 && m_ShowChildrenNames[index])
			{
				m_Window->draw(m_ChildrenNames[index]);
			}
		}

		for (unsigned int index = 0; index < m_AlliesShapes.size(); index++)
		{
			m_Window->draw(m_AlliesShapes[index]);
			m_Window->draw(m_AlliesSprites[index]);
		}

		for (unsigned int index = 0; index < m_WarShapes.size(); index++)
		{
			m_Window->draw(m_WarShapes[index]);
			m_Window->draw(m_WarSprites[index]);
		}

		for (unsigned int index = 0; index < m_TraitsSprites.size(); index++)
		{
			m_Window->draw(m_TraitsSprites[index]);
			if (m_TraitsShowInfo.size() > 0 && m_TraitsShowInfo[index])
			{
				m_Window->draw(m_TraitsInfo[index]);
			}
		}

		if (!m_IsPlayerCharacter && m_CurrentCharacter->m_CharacterTitle != Title::Unlanded)
		{
			for (unsigned int index = 0; index < m_NumberOfButtons; index++)
			{
				m_Window->draw(m_ButtonShapes[index]);
				m_Window->draw(m_ButtonTexts[index]);
			}
		}

		for (unsigned int index = 0; index < m_OwnedRegionShields.size(); index++)
		{
			HeraldicShieldManager::renderShield(m_OwnedRegionShields[index], m_OwnedRegionShapes[index].getPosition());
			if (m_OwnedRegionsShowInfo.size() > 0 && m_OwnedRegionsShowInfo[index])
			{
				m_Window->draw(m_OwnedRegionsTexts[index]);
			}
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
			Map& map = Map::get();
			if (map.mapSquareDataContainsKey(mouseMapPosition))
			{
				size_t regionID = 0;
				for (auto& squareData : map.m_MapSquareData)
				{
					if (squareData.m_Position == mouseMapPosition)
					{
						regionID = squareData.m_RegionID;
					}
				}
				m_CurrentRegionID = regionID;
				m_CurrentCharacterID = map.getRegionById(m_CurrentRegionID).m_OwnerID;
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

void CharacterWindow::clearInfo()
{
	m_TraitsSprites.clear();
	m_TraitsInfo.clear();

	m_OwnedRegionShields.clear();
	m_OwnedRegionShapes.clear();
	m_OwnedRegionsTexts.clear();

	m_ChildrenShapes.clear();
	m_ChildrenSprites.clear();
	m_ChildrenNames.clear();

	m_AlliesShapes.clear();
	m_AlliesSprites.clear();
	m_AlliesPositions.clear();

	m_WarShapes.clear();
	m_WarSprites.clear();
	m_WarPositions.clear();
	m_WarDefenders.clear();
	m_WarAttackers.clear();
}

void CharacterWindow::updateParents()
{
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
}

void CharacterWindow::updateTraits()
{
	std::stringstream stream;
	
	sf::Sprite genderSprite;
	if (m_Gender == Gender::Male)
	{
		stream << m_Male;
		genderSprite.setTexture(m_MaleGenderTexture);
	}
	else
	{
		stream << m_Female;
		genderSprite.setTexture(m_FemaleGenderTexture);
	}
	sf::Text genderInfo(stream.str(), m_Font, m_CharacterSize);
	genderInfo.setFillColor(m_OwnerColor);
	genderInfo.setPosition(sf::Vector2f(m_SpriteSize * (m_TraitsInfo.size()) + m_SizeX * 0.2f, m_SizeY * 0.42f + m_SpriteSize));
	genderSprite.setPosition(genderInfo.getPosition() - sf::Vector2f(0.0f, m_SpriteSize));
	genderSprite.setScale(m_SpriteSize / genderSprite.getLocalBounds().width, m_SpriteSize / genderSprite.getLocalBounds().height);
	m_TraitsInfo.push_back(genderInfo);
	m_TraitsSprites.push_back(genderSprite);
	stream.str(std::string());
	stream.clear();

	stream << m_Fertility << std::fixed << std::setprecision(1) << m_CurrentCharacter->m_Fertility;
	sf::Text fertilityInfo(stream.str(), m_Font, m_CharacterSize);
	fertilityInfo.setFillColor(m_OwnerColor);
	fertilityInfo.setPosition(sf::Vector2f(m_SpriteSize * (m_TraitsInfo.size()) + m_SizeX * 0.2f, m_SizeY * 0.42f + m_SpriteSize));
	sf::Sprite fertilitySprite(m_FertilityTexture);
	fertilitySprite.setPosition(fertilityInfo.getPosition() - sf::Vector2f(0.0f, m_SpriteSize));
	fertilitySprite.setScale(m_SpriteSize / fertilitySprite.getLocalBounds().width, m_SpriteSize / fertilitySprite.getLocalBounds().height);
	m_TraitsInfo.push_back(fertilityInfo);
	m_TraitsSprites.push_back(fertilitySprite);
	stream.str(std::string());
	stream.clear();

	CharacterManager& characterManager = *CharacterManager::get();

	if (m_CurrentCharacter->m_Spouse != INVALID_CHARACTER_ID)
	{
		Character& spouse = characterManager.getCharacter(m_CurrentCharacter->m_Spouse);
		stream << m_Spouse << spouse.m_Name;
		sf::Text marriedInfo(stream.str(), m_Font, m_CharacterSize);
		marriedInfo.setFillColor(spouse.m_RegionColor);
		marriedInfo.setPosition(sf::Vector2f(m_SpriteSize * (m_TraitsInfo.size()) + m_SizeX * 0.2f, m_SizeY * 0.42f + m_SpriteSize));
		sf::Sprite marriedSprite(m_MarriedTexture);
		marriedSprite.setPosition(marriedInfo.getPosition() - sf::Vector2f(0.0f, m_SpriteSize));
		marriedSprite.setScale(m_SpriteSize / marriedSprite.getLocalBounds().width, m_SpriteSize / marriedSprite.getLocalBounds().height);
		m_TraitsSprites.push_back(marriedSprite);
		m_TraitsInfo.push_back(marriedInfo);
	}

	if (characterManager.hasTrait(m_CurrentCharacterID, characterManager.getTrait(m_PregnantTrait)))
	{
		sf::Text pregnantInfo(m_PregnantTrait, m_Font, m_CharacterSize);
		pregnantInfo.setFillColor(m_OwnerColor);
		pregnantInfo.setPosition(sf::Vector2f(m_SpriteSize * (m_TraitsInfo.size()) + m_SizeX * 0.2f, m_SizeY * 0.42f + m_SpriteSize));
		sf::Sprite pregnantSprite(m_PregnantTexture);
		pregnantSprite.setPosition(pregnantInfo.getPosition() - sf::Vector2f(0.0f, m_SpriteSize));
		pregnantSprite.setScale(m_SpriteSize / pregnantSprite.getLocalBounds().width, m_SpriteSize / pregnantSprite.getLocalBounds().height);
		m_TraitsSprites.push_back(pregnantSprite);
		m_TraitsInfo.push_back(pregnantInfo);
	}

	if (characterManager.hasTrait(m_CurrentCharacterID, characterManager.getTrait(m_BeautifulTrait)))
	{
		sf::Text beautifulInfo(m_BeautifulTrait, m_Font, m_CharacterSize);
		beautifulInfo.setFillColor(m_OwnerColor);
		beautifulInfo.setPosition(sf::Vector2f(m_SpriteSize * (m_TraitsInfo.size()) + m_SizeX * 0.2f, m_SizeY * 0.42f + m_SpriteSize));
		sf::Sprite beautifulSprite(m_BeautifulTexture);
		beautifulSprite.setPosition(beautifulInfo.getPosition() - sf::Vector2f(0.0f, m_SpriteSize));
		beautifulSprite.setScale(m_SpriteSize / beautifulSprite.getLocalBounds().width, m_SpriteSize / beautifulSprite.getLocalBounds().height);
		m_TraitsSprites.push_back(beautifulSprite);
		m_TraitsInfo.push_back(beautifulInfo);
	}

	if (characterManager.hasTrait(m_CurrentCharacterID, characterManager.getTrait(m_UglyTrait)))
	{
		sf::Text uglyInfo(m_UglyTrait, m_Font, m_CharacterSize);
		uglyInfo.setFillColor(m_OwnerColor);
		uglyInfo.setPosition(sf::Vector2f(m_SpriteSize * (m_TraitsInfo.size()) + m_SizeX * 0.2f, m_SizeY * 0.42f + m_SpriteSize));
		sf::Sprite uglySprite(m_UglyTexture);
		uglySprite.setPosition(uglyInfo.getPosition() - sf::Vector2f(0.0f, m_SpriteSize));
		uglySprite.setScale(m_SpriteSize / uglySprite.getLocalBounds().width, m_SpriteSize / uglySprite.getLocalBounds().height);
		m_TraitsSprites.push_back(uglySprite);
		m_TraitsInfo.push_back(uglyInfo);
	}

	if (characterManager.hasTrait(m_CurrentCharacterID, characterManager.getTrait(m_SterileTrait)))
	{
		sf::Text sterileInfo(m_SterileTrait, m_Font, m_CharacterSize);
		sterileInfo.setFillColor(m_OwnerColor);
		sterileInfo.setPosition(sf::Vector2f(m_SpriteSize * (m_TraitsInfo.size()) + m_SizeX * 0.2f, m_SizeY * 0.42f + m_SpriteSize));
		sf::Sprite sterileSprite(m_SterileTexture);
		sterileSprite.setPosition(sterileInfo.getPosition() - sf::Vector2f(0.0f, m_SpriteSize));
		sterileSprite.setScale(m_SpriteSize / sterileSprite.getLocalBounds().width, m_SpriteSize / sterileSprite.getLocalBounds().height);
		m_TraitsSprites.push_back(sterileSprite);
		m_TraitsInfo.push_back(sterileInfo);
	}
}

void CharacterWindow::updateChildren()
{
	CharacterManager& characterManager = *CharacterManager::get();
	unsigned int sizeChildren = m_CurrentCharacter->m_Children.size();
	for (unsigned int index = 0; index < sizeChildren; index++)
	{
		Character& child = characterManager.getCharacter(m_CurrentCharacter->m_Children[index]);
		sf::RectangleShape childShape(sf::Vector2f(m_SpriteSize, m_SpriteSize));
		childShape.setOutlineThickness(m_OutlineThickness * 0.5f);
		childShape.setFillColor(sf::Color::Transparent);
		childShape.setPosition(sf::Vector2f(m_SizeX * 0.2f, m_SizeY * 0.05f * index + m_SizeY * 0.55f));
		sf::Text childNameText(child.m_Name, m_Font, m_CharacterSize);
		childNameText.setPosition(childShape.getPosition());
		if (child.m_CharacterTitle != Title::Unlanded)
		{
			childShape.setOutlineColor(child.m_RegionColor);
			childNameText.setFillColor(child.m_RegionColor);
		}
		else if (m_CurrentCharacter->m_CharacterID == child.m_Mother)
		{
			Character& fatherChild = characterManager.getCharacter(child.m_Father);
			childShape.setOutlineColor(fatherChild.m_RegionColor);
			childNameText.setFillColor(fatherChild.m_RegionColor);
		}
		else
		{
			Character& motherChild = characterManager.getCharacter(child.m_Mother);
			childShape.setOutlineColor(motherChild.m_RegionColor);
			childNameText.setFillColor(motherChild.m_RegionColor);
		}
		sf::Sprite childSprite;
		if (child.m_Gender == Gender::Male)
		{
			childSprite.setTexture(m_MaleChildTexture);
		}
		else
		{
			childSprite.setTexture(m_MaleChildTexture);
		}
		childSprite.setPosition(childShape.getPosition());
		childSprite.setScale(m_SpriteSize / childSprite.getLocalBounds().width, m_SpriteSize / childSprite.getLocalBounds().height);
		m_ChildrenShapes.push_back(childShape);
		m_ChildrenSprites.push_back(childSprite);
		m_ChildrenNames.push_back(childNameText);
	}
}

void CharacterWindow::updateAllies()
{
	CharacterManager& characterManager = *CharacterManager::get();
	m_AlliesIDs = WarManager::get().getAlliances(m_CurrentCharacterID);
	unsigned int sizeAlliances = m_AlliesIDs.size();
	for (unsigned int index = 0; index < sizeAlliances; index++)
	{
		Character& ally = characterManager.getCharacter(m_AlliesIDs[index]);
		sf::RectangleShape allyShape(sf::Vector2f(m_SpriteSize, m_SpriteSize));
		allyShape.setOutlineColor(ally.m_RegionColor);
		allyShape.setOutlineThickness(m_OutlineThickness * 0.5f);
		allyShape.setFillColor(sf::Color::Transparent);
		allyShape.setPosition(sf::Vector2f(m_SizeX * 0.3f, m_SizeY * 0.05f * index + m_SizeY * 0.55f));
		sf::Sprite allySprite;
		if (ally.m_Gender == Gender::Male)
		{
			allySprite.setTexture(m_MaleCharacterTexture);
		}
		else
		{
			allySprite.setTexture(m_FemaleCharacterTexture);
		}
		allySprite.setPosition(allyShape.getPosition());
		allySprite.setScale(m_SpriteSize / allySprite.getLocalBounds().width, m_SpriteSize / allySprite.getLocalBounds().height);
		m_AlliesShapes.push_back(allyShape);
		m_AlliesSprites.push_back(allySprite);
	}
}

void CharacterWindow::updateWars()
{
	CharacterManager& characterManager = *CharacterManager::get();
	WarManager& warManager = WarManager::get();
	War* war = nullptr;
	unsigned int sizeWars = m_CurrentCharacter->m_CurrentWars.size();
	for (unsigned int index = 0; index < sizeWars; index++)
	{
		war = warManager.getWar(m_CurrentCharacter->m_CurrentWars[index]);
		if (war != nullptr)
		{
			CharacterID defenderID = war->getDefender();
			CharacterID attackerID = war->getAttacker();
			CharacterID opponentID;
			if (war->isAttacker(m_CurrentCharacterID))
			{
				opponentID = defenderID;
			}
			else
			{
				opponentID = attackerID;
			}
			Character& opponent = characterManager.getCharacter(opponentID);
			sf::RectangleShape warShape(sf::Vector2f(m_SpriteSize, m_SpriteSize));
			warShape.setOutlineColor(opponent.m_RegionColor);
			warShape.setOutlineThickness(m_OutlineThickness * 0.5f);
			warShape.setFillColor(sf::Color::Transparent);
			warShape.setPosition(sf::Vector2f(m_SizeX * 0.4f, m_SizeY * 0.05f * index + m_SizeY * 0.55f));
			sf::Sprite opponetSprite;
			if (opponent.m_Gender == Gender::Male)
			{
				opponetSprite.setTexture(m_MaleCharacterTexture);
			}
			else
			{
				opponetSprite.setTexture(m_FemaleCharacterTexture);
			}
			opponetSprite.setPosition(warShape.getPosition());
			opponetSprite.setScale(m_SpriteSize / opponetSprite.getLocalBounds().width, m_SpriteSize / opponetSprite.getLocalBounds().height);
			m_WarShapes.push_back(warShape);
			m_WarSprites.push_back(opponetSprite);
			m_WarDefenders.push_back(defenderID);
			m_WarAttackers.push_back(attackerID);
		}
	}
}

void CharacterWindow::updateInfo()
{
	if (m_CurrentCharacterID != INVALID_CHARACTER_ID)
	{
		CharacterManager& characterManager = *CharacterManager::get();
		m_PlayerCharacter = &characterManager.getPlayerCharacter();
		m_CurrentCharacter = &characterManager.getCharacter(m_CurrentCharacterID);
		m_FatherID = m_CurrentCharacter->m_Father;
		m_MotherID = m_CurrentCharacter->m_Mother;
		m_Dead = m_CurrentCharacter->m_Dead;
		m_Gender = m_CurrentCharacter->m_Gender;

		if (m_CurrentCharacter->m_CharacterTitle != Title::Unlanded)
		{
			m_OwnerColor = m_CurrentCharacter->m_RegionColor;
			m_RealmNameText.setString(m_CurrentCharacter->m_KingdomName);
			m_RealmNameText.setFillColor(m_OwnerColor);
		}
		else if (m_FatherID != INVALID_CHARACTER_ID)
		{
			Character& father = characterManager.getCharacter(m_FatherID);
			m_OwnerColor = father.m_RegionColor;
			m_RealmNameText.setString(father.m_KingdomName);
			m_RealmNameText.setFillColor(m_OwnerColor);
		}
		else
		{
			m_OwnerColor = m_CurrentCharacter->m_RegionColor;
		}

		m_WindowShape.setOutlineColor(m_OwnerColor);

		for (sf::Text& text : m_DiplomacyTexts)
		{
			text.setFillColor(m_OwnerColor);
		}

		clearInfo();
		updateParents();
		updateTraits();
		updateChildren();
		updateAllies();
		updateWars();

		Map& map = Map::get();
		unsigned int ownedRegionSize = m_CurrentCharacter->m_OwnedRegionIDs.size();
		for (unsigned int index = 0; index < ownedRegionSize; index++)
		{
			sf::RectangleShape regionShape(sf::Vector2f(m_SpriteSize, m_SpriteSize));
			regionShape.setFillColor(sf::Color::Transparent);
			regionShape.setPosition(sf::Vector2f(m_SizeX * 0.1f, m_SizeY * 0.05f * index + m_SizeY * 0.55f));

			MapRegion& mapRegion = map.getRegionById(m_CurrentCharacter->m_OwnedRegionIDs[index]);
			sf::Text regionNameText(mapRegion.m_RegionName, m_Font, m_CharacterSize);
			regionNameText.setPosition(regionShape.getPosition() + sf::Vector2f(0.0f, m_SpriteSize));
			regionNameText.setFillColor(m_OwnerColor);

			m_OwnedRegionsTexts.push_back(regionNameText);
			m_OwnedRegionShapes.push_back(regionShape);
			m_OwnedRegionShields.push_back(mapRegion.m_HeraldicShield);
		}

		std::stringstream stream;
		if (m_CurrentCharacter->m_CharacterTitle != Title::Unlanded)
		{
			if (m_Gender == Gender::Male)
			{
				stream << m_MaleTitles[(unsigned int)m_CurrentCharacter->m_CharacterTitle] << m_CurrentCharacter->m_Name;
				m_CharacterSprite.setTexture(m_MaleCharacterTexture, true);
			}
			else
			{
				stream << m_FemaleTitles[(unsigned int)m_CurrentCharacter->m_CharacterTitle] << m_CurrentCharacter->m_Name;
				m_CharacterSprite.setTexture(m_FemaleCharacterTexture, true);
			}
		}
		else
		{
			stream << m_CurrentCharacter->m_Name;
			if (m_Gender == Gender::Male)
			{
				m_CharacterSprite.setTexture(m_MaleCharacterTexture, true);
			}
			else
			{
				m_CharacterSprite.setTexture(m_FemaleCharacterTexture, true);
			}
		}
		m_CharacterSprite.setPosition(sf::Vector2f(m_SizeX * 0.1f, m_SizeY * 0.025f));
		m_CharacterSprite.setScale((m_SpriteSize * 2) / m_CharacterSprite.getLocalBounds().width, (m_SpriteSize * 2) / m_CharacterSprite.getLocalBounds().height);
		m_CharacterNameText.setString(stream.str());
		m_CharacterNameText.setFillColor(m_OwnerColor);
		stream.str(std::string());
		stream.clear();

		stream << m_CurrentCharacter->m_RaisedArmySize << m_Dash << m_CurrentCharacter->m_MaxArmySize;
		m_ArmyText.setString(stream.str());
		m_ArmyText.setFillColor(m_OwnerColor);
		stream.str(std::string());
		stream.clear();

		stream << std::fixed << std::setprecision(1) << m_CurrentCharacter->m_CurrentGold;
		m_GoldText.setString(stream.str());
		m_GoldText.setFillColor(m_OwnerColor);
		stream.str(std::string());
		stream.clear();

		stream << Time::m_GameDate.getAge(m_CurrentCharacter->m_Birthday);
		m_CharacterAgeText.setString(stream.str());
		m_CharacterAgeText.setFillColor(m_OwnerColor);
		stream.str(std::string());
		stream.clear();
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
	else if (InputHandler::getRegionWindowOpen() || InputHandler::getWarWindowOpen())
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
		InputHandler::setCharacterWindowOpen(true);
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
		InputHandler::setCharacterWindowOpen(false);
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
		for (unsigned int index = 0; index < m_OwnedRegionShapes.size(); index++)
		{
			if (index + 1 > m_OwnedRegionsShowInfo.size())
			{
				m_OwnedRegionsShowInfo.push_back(false);
			}
			if (m_OwnedRegionShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				m_OwnedRegionsShowInfo[index] = true;
			}
			else
			{
				m_OwnedRegionsShowInfo[index] = false;
			}
		}
		for (unsigned int index = 0; index < m_DiplomacySprites.size(); index++)
		{
			if (index + 1 > m_DiplomacyShowInfo.size())
			{
				m_DiplomacyShowInfo.push_back(false);
			}
			if (m_DiplomacySprites[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				m_DiplomacyShowInfo[index] = true;
			}
			else
			{
				m_DiplomacyShowInfo[index] = false;
			}
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
		for (unsigned int index = 0; index < m_TraitsSprites.size(); index++)
		{
			if (index + 1 > m_TraitsShowInfo.size())
			{
				m_TraitsShowInfo.push_back(false);
			}
			if (m_TraitsSprites[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				m_TraitsShowInfo[index] = true;
			}
			else
			{
				m_TraitsShowInfo[index] = false;
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
		else if (m_FatherSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			m_CurrentCharacterID = m_FatherID;
			checkIfPlayerCharacter();
			updateInfo();
		}
		else if (m_MotherSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			m_CurrentCharacterID = m_MotherID;
			checkIfPlayerCharacter();
			updateInfo();
		}
		CharacterManager& characterManager = *CharacterManager::get();
		for (unsigned int index = 0; index < m_ChildrenShapes.size(); index++)
		{
			if (m_ChildrenShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				m_CurrentCharacterID = characterManager.getCharacter(m_CurrentCharacterID).m_Children[index];
				checkIfPlayerCharacter();
				updateInfo();
				break;
			}
		}
		for (unsigned int index = 0; index < m_AlliesShapes.size(); index++)
		{
			if (m_AlliesShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				m_CurrentCharacterID = m_AlliesIDs[index];
				checkIfPlayerCharacter();
				updateInfo();
				break;
			}
		}
		WarManager& warManager = WarManager::get();
		for (unsigned int index = 0; index < m_WarShapes.size(); index++)
		{
			if (m_WarShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				m_CurrentCharacterID = warManager.getWar(m_CurrentCharacter->m_CurrentWars[index])->getOpposingForce(m_CurrentCharacterID);
				checkIfPlayerCharacter();
				updateInfo();
				break;
			}
		}
		UIManager& uiManager = *UIManager::get();
		for (unsigned int index = 0; index < m_OwnedRegionShapes.size(); index++)
		{
			if (m_OwnedRegionShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				closeWindow();
				RegionWindow& regionWindow = *uiManager.m_RegionWindow;
				regionWindow.m_CurrentMapRegion = &Map::get().getRegionById(m_CurrentCharacter->m_OwnedRegionIDs[index]);
				regionWindow.checkIfPlayerRegion();
				regionWindow.openWindow();
				regionWindow.updateInfo();
			}
		}
	}
	if (InputHandler::getLeftMouseReleased() && !m_IsPlayerCharacter)
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		for (unsigned int index = 0; index < m_NumberOfButtons; index++)
		{
			if (m_ButtonShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				switch (index)
				{
				case 0:
				{
					declareWar();
					break;
				}
				case 1:
				{
					offerPeace();
					break;
				}
				case 2:
				{
					proposeMarriage();
					break;
				}
				case 3:
				{
					proposeAlliance();
					break;
				}
				case 4:
				{
					assassinate();
					break;
				}
				default:
				{
					break;
				}
				}
			}
		}
		UIManager& uiManager = *UIManager::get();
		for (unsigned int index = 0; index < m_WarShapes.size(); index++)
		{
			if (m_WarShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				closeWindow();
				InputHandler::m_Inputs[LeftMouseReleased] = false;
				uiManager.m_WarWindow->openWindow(m_WarAttackers[index], m_WarDefenders[index], Time::m_GameDate.m_Date);
				break;
			}
		}
	}
}

void CharacterWindow::declareWar()
{
	UIManager& uiManager = *UIManager::get();
	for (CharacterID& allyID : WarManager::get().getAlliances(m_PlayerCharacter->m_CharacterID))
	{
		if (allyID == m_CurrentCharacterID)
		{
			uiManager.createUIEventElement(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, UIType::TriedToDeclareWarOnAlly);
			return;
		}
	}
	WarManager& warManager = WarManager::get();
	War* war = warManager.getWarAgainst(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID);
	if (war == nullptr)
	{
		int warHandle = WarManager::get().createWar(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, m_CurrentRegionID);

		m_PlayerCharacter->m_CurrentWars.push_back(warHandle);
		m_CurrentCharacter->m_CurrentWars.push_back(warHandle);

		AIManager& aiManager = AIManager::get();
		aiManager.getWarmindOfCharacter(m_CurrentCharacterID).m_Active = true;
		aiManager.getWarmindOfCharacter(m_CurrentCharacterID).m_Opponent = m_PlayerCharacter->m_CharacterID;
		aiManager.getAIDataofCharacter(m_CurrentCharacterID).m_CurrentAction = Action::War;
		aiManager.getAIDataofCharacter(m_CurrentCharacterID).m_LastAction = Action::War;

		uiManager.createUIEventElement(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, UIType::WarDeclaration);
		uiManager.createWarIcon(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID);

		CharacterManager::get()->callAllies(m_PlayerCharacter->m_CharacterID, warHandle);

		Game::m_Sound.pause();
		if (Game::m_BattleSound.getStatus() != sf::SoundSource::Playing)
		{
			Game::m_BattleSound.play();
		}
	}
}

void CharacterWindow::offerPeace()
{
	WarManager& warManager = WarManager::get();
	War* war = warManager.getWarAgainst(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID);
	if (war != nullptr)
	{
		CharacterManager::get()->sendPeaceOffer(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, PeaceType::Enforce_Demands);

		if (Game::m_BattleSound.getStatus() == sf::SoundSource::Playing && m_PlayerCharacter->m_CurrentWars.size() == 0)
		{
			Game::m_BattleSound.stop();
			Game::m_Sound.play();
		}
	}
}

void CharacterWindow::proposeMarriage()
{
	CharacterManager::get()->marry(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID);
}

void CharacterWindow::proposeAlliance()
{
	CharacterManager::get()->sendAllianceOffer(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID);
}

void CharacterWindow::assassinate()
{
	CharacterManager& characterManager = *CharacterManager::get();
	if (characterManager.chancePerPercent(0.5f))
	{
		characterManager.killCharacter(m_CurrentCharacterID);
		UIManager::get()->createUIEventElement(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, UIType::AssassinationSuccess);
	}
	else
	{
		UIManager::get()->createUIEventElement(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, UIType::AssassinationFailure);
	}
}

bool CharacterWindow::checkIfPlayerCharacter()
{
	return m_IsPlayerCharacter = m_CurrentCharacterID == m_PlayerCharacter->m_CharacterID;
}