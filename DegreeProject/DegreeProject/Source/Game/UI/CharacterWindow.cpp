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

	m_FertilityTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Fertility.png");
	m_BeautifulTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Beautiful.png");
	m_UglyTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Ugly.png");
	m_SterileTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Sterile.png");
	m_PregnantTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Pregnant.png");

	m_PregnantPosition = sf::Vector2f(m_SizeX * 0.5f, m_SizeY * 0.42f);

	m_MaleChildTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/BabyMale.png");
	m_FemaleChildTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/BabyFemale.png");

	m_MotherTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Mother.png");
	m_FatherTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Father.png");

	m_FatherPosition = sf::Vector2f(m_SizeX * 0.4f, m_SizeY * 0.55f);
	m_MotherPosition = sf::Vector2f(m_SizeX * 0.4f, m_SizeY * 0.60f);

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
	m_FatherName.setStyle(m_Style);
	m_FatherName.setPosition(sf::Vector2f(m_FatherPosition.x + m_SizeX * 0.1f, m_FatherPosition.y));

	m_MotherName.setFont(m_Font);
	m_MotherName.setCharacterSize(m_CharacterSize);
	m_MotherName.setStyle(m_Style);
	m_MotherName.setPosition(sf::Vector2f(m_MotherPosition.x + m_SizeX * 0.1f, m_MotherPosition.y));

	m_MarriedTexture = AssetHandler::get().getTextureAtPath("Assets/Graphics/Married.png");
	m_MarriedPosition = sf::Vector2f(m_SizeX * 0.4f, m_SizeY * 0.42f);

	m_SpouseName.setFont(m_Font);
	m_SpouseName.setCharacterSize(m_CharacterSize);
	m_SpouseName.setStyle(m_Style);
	m_SpouseName.setPosition(sf::Vector2f(m_MarriedPosition.x + m_SizeX * 0.1f, m_MarriedPosition.y));

	m_WindowShape.setFillColor(m_FillColor);
	m_WindowShape.setOutlineThickness(m_OutlineThickness);
	m_WindowShape.setPosition(sf::Vector2f(m_OutlineThickness, m_OutlineThickness));

	m_CharacterNameText.setFont(m_Font);
	m_CharacterNameText.setCharacterSize(m_CharacterSize);
	m_CharacterNameText.setStyle(m_Style);
	m_CharacterNameText.setPosition(sf::Vector2f(m_SizeX * 0.25f, m_SizeY * 0.025f));

	m_RealmNameText.setFont(m_Font);
	m_RealmNameText.setCharacterSize(m_CharacterSize);
	m_RealmNameText.setStyle(m_Style);
	m_RealmNameText.setPosition(sf::Vector2f(m_SizeX * 0.1f, m_SizeY * 0.1f));

	m_CharacterAgeText.setFont(m_Font);
	m_CharacterAgeText.setCharacterSize(m_CharacterSize);
	m_CharacterAgeText.setStyle(m_Style);
	m_CharacterAgeText.setPosition(sf::Vector2f(m_SizeX * 0.2f, m_SizeY * 0.4f));

	m_ArmyText.setFont(m_Font);
	m_ArmyText.setCharacterSize(m_CharacterSize);
	m_ArmyText.setStyle(m_Style);
	m_ArmyText.setPosition(sf::Vector2f(m_SizeX * 0.2f, m_SizeY * 0.2f));

	m_GoldText.setFont(m_Font);
	m_GoldText.setCharacterSize(m_CharacterSize);
	m_GoldText.setStyle(m_Style);
	m_GoldText.setPosition(sf::Vector2f(m_SizeX * 0.2f, m_SizeY * 0.3f));

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
			updateSprite(m_DeadSprite, m_DeadTexture, m_DeadPosition, m_SpriteSize);
		}
		updateSprite(m_GoldSprite, m_GoldTexture, m_GoldPosition, m_SpriteSize);
		m_Window->draw(m_GoldText);
		m_Window->draw(m_CharacterNameText);
		updateSprite(m_ArmySprite, m_ArmyTexture, m_ArmyPosition, m_SpriteSize);
		m_Window->draw(m_ArmyText);
		updateSprite(m_AgeSprite, m_AgeTexture, m_AgePosition, m_SpriteSize);
		m_Window->draw(m_CharacterAgeText);
		if (m_Gender == Gender::Male)
		{
			updateSprite(m_CharacterSprite, m_MaleCharacterTexture, m_CharacterPosition, m_SpriteSize * 2);
		}
		else
		{
			updateSprite(m_CharacterSprite, m_FemaleCharacterTexture, m_CharacterPosition, m_SpriteSize * 2);
			if (m_Pregnant)
			{
				updateSprite(m_Pregnantprite, m_PregnantTexture, m_PregnantPosition, m_SpriteSize);
			}
		}
		if (m_SpouseID != INVALID_CHARACTER_ID)
		{
			updateSprite(m_MarriedSprite, m_MarriedTexture, m_MarriedPosition, m_SpriteSize);
			if (m_ShowSpouseName)
			{
				m_Window->draw(m_SpouseName);
			}
		}
		m_Window->draw(m_FatherShape);
		if (m_FatherID != INVALID_CHARACTER_ID)
		{
			updateSprite(m_FatherSprite, m_FatherTexture, m_FatherPosition, m_SpriteSize);
			if (m_ShowFatherName)
			{
				m_Window->draw(m_FatherName);
			}
		}
		m_Window->draw(m_MotherShape);
		if (m_MotherID != INVALID_CHARACTER_ID)
		{
			updateSprite(m_MotherSprite, m_MotherTexture, m_MotherPosition, m_SpriteSize);
			if (m_ShowMotherName)
			{
				m_Window->draw(m_MotherName);
			}
		}
		for (unsigned int index = 0; index < m_ChildrenShapes.size(); index++)
		{
			m_Window->draw(m_ChildrenShapes[index]);
			updateSprite(m_ChildrenSprites[index], m_ChildrenTextures[index], m_ChildrenShapes[index].getPosition(), m_SpriteSize);
			if (m_ShowChildrenNames.size() > 0 && m_ShowChildrenNames[index])
			{
				m_Window->draw(m_ChildrenNames[index]);
			}
		}
		for (unsigned int index = 0; index < m_AlliesShapes.size(); index++)
		{
			m_Window->draw(m_AlliesShapes[index]);
			updateSprite(m_AlliesSprites[index], m_AlliesTextures[index], m_AlliesShapes[index].getPosition(), m_SpriteSize);
		}
		for (unsigned int index = 0; index < m_WarShapes.size(); index++)
		{
			m_Window->draw(m_WarShapes[index]);
			updateSprite(m_WarSprites[index], m_WarTextures[index], m_WarShapes[index].getPosition(), m_SpriteSize);
		}
		for (unsigned int index = 0; index < m_TraitsSprites.size(); index++)
		{
			updateSprite(m_TraitsSprites[index], m_TraitsTextures[index], m_TraitsInfo[index].getPosition() - sf::Vector2f(0.0f, m_SpriteSize), m_SpriteSize);
			if (m_TraitsShowInfo[index])
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

void CharacterWindow::clearInfo()
{
	m_TraitsSprites.clear();
	m_TraitsTextures.clear();
	m_TraitsInfo.clear();

	m_ChildrenShapes.clear();
	m_ChildrenSprites.clear();
	m_ChildrenTextures.clear();
	m_ChildrenNames.clear();

	m_AlliesShapes.clear();
	m_AlliesSprites.clear();
	m_AlliesTextures.clear();
	m_AlliesPositions.clear();

	m_WarShapes.clear();
	m_WarSprites.clear();
	m_WarTextures.clear();
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

	if (m_Gender == Gender::Male)
	{
		stream << "Male";
		m_TraitsTextures.push_back(m_MaleGenderTexture);
	}
	else
	{
		stream << "Female";
		m_TraitsTextures.push_back(m_FemaleGenderTexture);
	}
	sf::Text genderInfo(stream.str(), m_Font, m_CharacterSize);
	genderInfo.setFillColor(m_OwnerColor);
	genderInfo.setPosition(sf::Vector2f(m_SpriteSize * (m_TraitsInfo.size()) + m_SizeX * 0.2f, m_SizeY * 0.42f + m_SpriteSize));
	m_TraitsInfo.push_back(genderInfo);
	m_TraitsShowInfo.push_back(false);
	m_TraitsSprites.push_back(sf::Sprite());
	stream.str(std::string());
	stream.clear();

	stream << "Fertility: " << std::fixed << std::setprecision(1) << m_CurrentCharacter->m_Fertility;
	sf::Text fertilityInfo(stream.str(), m_Font, m_CharacterSize);
	fertilityInfo.setFillColor(m_OwnerColor);
	fertilityInfo.setPosition(sf::Vector2f(m_SpriteSize * (m_TraitsInfo.size()) + m_SizeX * 0.2f, m_SizeY * 0.42f + m_SpriteSize));
	m_TraitsInfo.push_back(fertilityInfo);
	m_TraitsShowInfo.push_back(false);
	m_TraitsSprites.push_back(sf::Sprite());
	m_TraitsTextures.push_back(m_FertilityTexture);
	stream.str(std::string());
	stream.clear();

	if (m_CurrentCharacter->m_Spouse != INVALID_CHARACTER_ID)
	{
		sf::Sprite sprite;
		Character& spouse = CharacterManager::get()->getCharacter(m_CurrentCharacter->m_Spouse);
		stream << "Spouse: " << spouse.m_Name;
		sf::Text text(stream.str(), m_Font, m_CharacterSize);
		text.setFillColor(spouse.m_RegionColor);
		text.setPosition(sf::Vector2f(m_SpriteSize * (m_TraitsInfo.size()) + m_SizeX * 0.2f, m_SizeY * 0.42f + m_SpriteSize));
		m_TraitsSprites.push_back(sprite);
		m_TraitsTextures.push_back(m_MarriedTexture);
		m_TraitsInfo.push_back(text);
		m_TraitsShowInfo.push_back(false);
	}

	if (CharacterManager::get()->hasTrait(m_CurrentCharacterID, CharacterManager::get()->getTrait(m_PregnantTrait)))
	{
		sf::Sprite sprite;
		sf::Text text(m_PregnantTrait, m_Font, m_CharacterSize);
		text.setFillColor(m_OwnerColor);
		text.setPosition(sf::Vector2f(m_SpriteSize * (m_TraitsInfo.size()) + m_SizeX * 0.2f, m_SizeY * 0.42f + m_SpriteSize));
		m_TraitsSprites.push_back(sprite);
		m_TraitsTextures.push_back(m_PregnantTexture);
		m_TraitsInfo.push_back(text);
		m_TraitsShowInfo.push_back(false);
	}

	if (CharacterManager::get()->hasTrait(m_CurrentCharacterID, CharacterManager::get()->getTrait(m_BeautifulTrait)))
	{
		sf::Sprite sprite;
		sf::Text text(m_BeautifulTrait, m_Font, m_CharacterSize);
		text.setFillColor(m_OwnerColor);
		text.setPosition(sf::Vector2f(m_SpriteSize * (m_TraitsInfo.size()) + m_SizeX * 0.2f, m_SizeY * 0.42f + m_SpriteSize));
		m_TraitsSprites.push_back(sprite);
		m_TraitsTextures.push_back(m_BeautifulTexture);
		m_TraitsInfo.push_back(text);
		m_TraitsShowInfo.push_back(false);
	}

	if (CharacterManager::get()->hasTrait(m_CurrentCharacterID, CharacterManager::get()->getTrait(m_UglyTrait)))
	{
		sf::Sprite sprite;
		sf::Text text(m_UglyTrait, m_Font, m_CharacterSize);
		text.setFillColor(m_OwnerColor);
		text.setPosition(sf::Vector2f(m_SpriteSize * (m_TraitsInfo.size()) + m_SizeX * 0.2f, m_SizeY * 0.42f + m_SpriteSize));
		m_TraitsSprites.push_back(sprite);
		m_TraitsTextures.push_back(m_UglyTexture);
		m_TraitsInfo.push_back(text);
		m_TraitsShowInfo.push_back(false);
	}

	if (CharacterManager::get()->hasTrait(m_CurrentCharacterID, CharacterManager::get()->getTrait(m_SterileTrait)))
	{
		sf::Sprite sprite;
		sf::Text text(m_SterileTrait, m_Font, m_CharacterSize);
		text.setFillColor(m_OwnerColor);
		text.setPosition(sf::Vector2f(m_SpriteSize * (m_TraitsInfo.size()) + m_SizeX * 0.2f, m_SizeY * 0.42f + m_SpriteSize));
		m_TraitsSprites.push_back(sprite);
		m_TraitsTextures.push_back(m_SterileTexture);
		m_TraitsInfo.push_back(text);
		m_TraitsShowInfo.push_back(false);
	}
}

void CharacterWindow::updateChildren()
{
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
		m_ChildrenShapes[index].setSize(sf::Vector2f(m_SpriteSize, m_SpriteSize));
		m_ChildrenShapes[index].setPosition(sf::Vector2f(m_SizeX * 0.1f, m_SizeY * 0.05f * (index)+m_SizeY * 0.55f));

		m_ChildrenNames[index].setString(child.m_Name);
		m_ChildrenNames[index].setFont(m_Font);
		m_ChildrenNames[index].setCharacterSize(m_CharacterSize);
		m_ChildrenNames[index].setStyle(m_Style);
		m_ChildrenNames[index].setPosition(sf::Vector2f(m_SizeX * 0.2f, m_SizeY * 0.05f * (index)+m_SizeY * 0.55f));
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
}

void CharacterWindow::updateAllies()
{
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
		m_AlliesShapes[index].setSize(sf::Vector2f(m_SpriteSize, m_SpriteSize));
		m_AlliesShapes[index].setPosition(sf::Vector2f(m_SizeX * 0.2f, m_SizeY * 0.05f * (index)+m_SizeY * 0.55f));
	}
}

void CharacterWindow::updateWars()
{
	for (unsigned int index = 0; index < m_CurrentCharacter->m_CurrentWars.size(); index++)
	{
		War* war = WarManager::get().getWar(m_CurrentCharacter->m_CurrentWars[index]);
		if (war != nullptr)
		{
			CharacterID defenderID = war->getDefender();
			CharacterID attackerID = war->getAttacker();
			m_WarDefenders.push_back(defenderID);
			m_WarAttackers.push_back(attackerID);
			CharacterID opponentID;
			if (war->isAttacker(m_CurrentCharacterID))
			{
				opponentID = defenderID;
			}
			else
			{
				opponentID = attackerID;
			}
			Character& opponent = CharacterManager::get()->getCharacter(opponentID);
			m_WarShapes.push_back(sf::RectangleShape());
			m_WarSprites.push_back(sf::Sprite());
			if (opponent.m_Gender == Gender::Male)
			{
				m_WarTextures.push_back(m_MaleCharacterTexture);
			}
			else
			{
				m_WarTextures.push_back(m_FemaleCharacterTexture);
			}
			m_WarShapes[index].setOutlineColor(opponent.m_RegionColor);
			m_WarShapes[index].setOutlineThickness(m_OutlineThickness * 0.5f);
			m_WarShapes[index].setSize(sf::Vector2f(m_SpriteSize, m_SpriteSize));
			m_WarShapes[index].setPosition(sf::Vector2f(m_SizeX * 0.3f, m_SizeY * 0.05f * (index)+m_SizeY * 0.55f));
		}
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

		m_WindowShape.setOutlineColor(m_OwnerColor);
		m_Gender = m_CurrentCharacter->m_Gender;

		clearInfo();
		updateParents();
		updateTraits();
		updateChildren();
		updateAllies();
		updateWars();

		if (m_Gender == Gender::Male)
		{
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

		std::stringstream stream;
		stream << m_CurrentCharacter->m_RaisedArmySize << m_Dash << m_CurrentCharacter->m_MaxArmySize;
		m_ArmyText.setString(stream.str());
		m_ArmyText.setFillColor(m_OwnerColor);


		stream << std::fixed << std::setprecision(1) << m_CurrentCharacter->m_CurrentGold;
		m_GoldText.setString(stream.str());
		m_GoldText.setFillColor(m_OwnerColor);
		stream.str(std::string());
		stream.clear();

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
				m_CurrentCharacterID = WarManager::get().getWar(m_CurrentCharacter->m_CurrentWars[index])->getOpposingForce(m_CurrentCharacterID);
				checkIfPlayerCharacter();
				updateInfo();
				break;
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
		for (unsigned int index = 0; index < m_WarShapes.size(); index++)
		{
			if (m_WarShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				closeWindow();
				InputHandler::setCharacterWindowOpen(false);
				UIManager::get()->m_WarWindow->openWindow(m_WarAttackers[index], m_WarDefenders[index], Time::m_GameDate.m_Date);
				break;
			}
		}
	}
}

void CharacterWindow::declareWar()
{
	for (CharacterID& allyID : WarManager::get().getAlliances(m_PlayerCharacter->m_CharacterID))
	{
		if (allyID == m_CurrentCharacterID)
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

void CharacterWindow::offerPeace()
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