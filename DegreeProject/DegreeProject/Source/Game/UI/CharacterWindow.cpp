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
#include "Game/DiplomacyManager.h"
#include "Game/Data/AIData.h"
#include "Game/UI/UIManager.h"
#include "Game/UI/WarWindow.h"
#include "Game/Systems/HeraldicShieldManager.h"
#include "Game/UI/RegionWindow.h"
#include "Game/UI/FamilyTreeWindow.h"
#include "Game/Systems/UnitManager.h"
#include "Game/Data/Unit.h"

CharacterWindow::CharacterWindow(UIID id, sf::Font font, Vector2D, Vector2D size)
{
	m_OwnedUIWindow = id;
	m_Font = font;
	m_SizeX = size.x;
	m_SizeY = size.y;

	setShape(m_WindowShape, m_FillColor, m_OwnerColor, m_OutlineThickness, {  }, { m_OutlineThickness, m_OutlineThickness });

	unsigned int numberOfButtons = m_ButtonStrings.size();
	for (unsigned int index = 0; index < numberOfButtons; index++)
	{
		sf::RectangleShape buttonShape;
		setShape(buttonShape, m_TransparentColor, m_DeclareWarColor, m_OutlineThickness, { m_SpriteSize * 4.5f, m_SpriteSize * 1.5f }, { m_SizeX - m_SpriteSize * 5.0f, m_SpriteSize * 19.0f + m_SpriteSize * 3.0f * index });

		sf::Text buttonText;
		setText(buttonText, m_Font, m_CharacterSize, m_DeclareWarColor, { buttonShape.getPosition().x + m_SpriteSize * 0.2f, buttonShape.getPosition().y + m_SpriteSize * 0.25f }, m_ButtonStrings[index]);
		if (index != 0 && index != numberOfButtons - 2 && index != numberOfButtons - 1)
		{
			buttonShape.setOutlineColor(m_MakePeaceColor);
			buttonText.setFillColor(m_MakePeaceColor);
		}
		m_ButtonShapes.push_back(buttonShape);
		m_ButtonTexts.push_back(buttonText);
	}

	AssetHandler& assetHandler = AssetHandler::get();

	m_DiplomacyTextures.push_back(assetHandler.getTextureAtPath("Assets/Graphics/Castle.png"));
	m_DiplomacyTextures.push_back(assetHandler.getTextureAtPath("Assets/Graphics/BabyMale.png"));
	m_DiplomacyTextures.push_back(assetHandler.getTextureAtPath("Assets/Graphics/Alliance.png"));
	m_DiplomacyTextures.push_back(assetHandler.getTextureAtPath("Assets/Graphics/War.png"));
	m_DiplomacyTextures.push_back(assetHandler.getTextureAtPath("Assets/Graphics/Father.png"));
	m_DiplomacyTextures.push_back(assetHandler.getTextureAtPath("Assets/Graphics/Peace.png"));
	unsigned int numberOfRelations = m_DiplomacyStrings.size();
	for (unsigned int index = 0; index < numberOfRelations; index++)
	{
		sf::Sprite relationSprite;
		setSprite(relationSprite, m_DiplomacyTextures[index], { m_SizeX * 0.1f + (m_SizeX * 0.1f * index), m_SpriteSize * 10 });
		m_DiplomacySprites.push_back(relationSprite);

		sf::Text relationText;
		setText(relationText, m_Font, m_CharacterSize, m_OwnerColor, { relationSprite.getPosition().x, relationSprite.getPosition().y + m_SpriteSize }, m_DiplomacyStrings[index]);
		m_DiplomacyTexts.push_back(relationText);
	}

	m_TraitTextures.push_back(assetHandler.getTextureAtPath("Assets/Graphics/Male.png"));
	m_TraitTextures.push_back(assetHandler.getTextureAtPath("Assets/Graphics/Fertility.png"));
	m_TraitTextures.push_back(assetHandler.getTextureAtPath("Assets/Graphics/Beautiful.png"));
	m_TraitTextures.push_back(assetHandler.getTextureAtPath("Assets/Graphics/Ugly.png"));
	m_TraitTextures.push_back(assetHandler.getTextureAtPath("Assets/Graphics/Sterile.png"));
	m_TraitTextures.push_back(assetHandler.getTextureAtPath("Assets/Graphics/Married.png"));
	m_TraitTextures.push_back(assetHandler.getTextureAtPath("Assets/Graphics/Pregnant.png"));
	m_TraitTextures.push_back(assetHandler.getTextureAtPath("Assets/Graphics/Female.png"));
	for (unsigned int index = 0; index < m_NumberOfTraits; index++)
	{
		sf::Sprite traitSprite;
		m_TraitPositions.push_back({ m_SpriteSize * index + m_SizeX * 0.1f, m_SpriteSize * 8 });
		setSprite(traitSprite, m_TraitTextures[index], m_TraitPositions[index]);
		m_TraitSprites.push_back(traitSprite);

		sf::Text traitText;
		setText(traitText, m_Font, m_CharacterSize, m_OwnerColor, { m_TraitPositions[index].x, m_TraitPositions[index].y + m_SpriteSize }, m_TraitStrings[index]);
		m_TraitTexts.push_back(traitText);

		m_TraitsEnabled.push_back(false);
		m_TraitsShowText.push_back(false);
	}

	setText(m_RealmNameText, m_Font, m_CharacterSize, m_OwnerColor, { m_SizeX * 0.25f, m_SpriteSize * 2 });

	m_MaleCharacterTexture = assetHandler.getTextureAtPath("Assets/Graphics/MalePortrait.jpg");
	m_FemaleCharacterTexture = assetHandler.getTextureAtPath("Assets/Graphics/FemalePortrait.jpg");
	setText(m_CharacterNameText, m_Font, (unsigned int)(m_CharacterSize * 1.5f), m_OwnerColor, { m_SizeX * 0.25f, m_SpriteSize * 0.75f });

	m_MaleChildTexture = assetHandler.getTextureAtPath("Assets/Graphics/BabyMale.png");
	m_FemaleChildTexture = assetHandler.getTextureAtPath("Assets/Graphics/BabyFemale.png");

	m_GoldTexture = assetHandler.getTextureAtPath("Assets/Graphics/Coins.png");
	m_GoldPosition = { m_SizeX * 0.1f, m_SpriteSize * 4 };
	setText(m_GoldText, m_Font, m_CharacterSize, m_OwnerColor, { m_GoldPosition.x + m_SpriteSize * 1.5f, m_GoldPosition.y });
	setSprite(m_GoldSprite, m_GoldTexture, m_GoldPosition);

	m_ArmyTexture = assetHandler.getTextureAtPath("Assets/Graphics/soldier unit.png");
	m_ArmyPosition = { m_SizeX * 0.1f, m_SpriteSize * 6 };
	setText(m_ArmyText, m_Font, m_CharacterSize, m_OwnerColor, { m_ArmyPosition.x + m_SpriteSize * 1.5f, m_ArmyPosition.y });
	setSprite(m_ArmySprite, m_ArmyTexture, m_ArmyPosition);

	m_AgeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Age.png");
	m_AgePosition = { m_SizeX - m_SpriteSize * 3, m_SpriteSize };
	setText(m_CharacterAgeText, m_Font, m_CharacterSize, m_OwnerColor, { m_AgePosition.x + m_SpriteSize * 1.5f, m_AgePosition.y });
	setSprite(m_AgeSprite, m_AgeTexture, m_AgePosition);

	m_DeadTexture = assetHandler.getTextureAtPath("Assets/Graphics/Dead.png");
	m_DeadPosition = { m_AgePosition.x - m_SpriteSize * 1.5f, m_AgePosition.y };
	m_BirthPosition = { m_DeadPosition.x, m_DeadPosition.y + m_SpriteSize * 2};
	setSprite(m_DeadSprite, m_DeadTexture, m_DeadPosition);
	setText(m_DeathDate, m_Font, m_CharacterSize, m_OwnerColor, { m_DeadPosition.x, m_DeadPosition.y + m_SpriteSize });
	setText(m_Birthday, m_Font, m_CharacterSize, m_OwnerColor, { m_BirthPosition.x, m_BirthPosition.y + m_SpriteSize });

	m_FatherTexture = assetHandler.getTextureAtPath("Assets/Graphics/Father.png");
	m_FatherPosition = { m_SizeX * 0.5f, m_SpriteSize * 12 };
	setShape(m_FatherShape, m_TransparentColor, m_OwnerColor, m_OutlineThickness * 0.5f, { m_SpriteSize, m_SpriteSize }, m_FatherPosition);
	setText(m_FatherName, m_Font, m_CharacterSize, m_OwnerColor, { m_FatherPosition.x + m_SizeX * 0.1f, m_FatherPosition.y });
	setSprite(m_FatherSprite, m_FatherTexture, m_FatherPosition);

	m_MotherTexture = assetHandler.getTextureAtPath("Assets/Graphics/Mother.png");
	m_MotherPosition = { m_SizeX * 0.5f, m_SpriteSize * 13 + m_OutlineThickness * 1.5f };
	setShape(m_MotherShape, m_TransparentColor, m_OwnerColor, m_OutlineThickness * 0.5f, { m_SpriteSize, m_SpriteSize }, m_MotherPosition);
	setText(m_MotherName, m_Font, m_CharacterSize, m_OwnerColor, { m_MotherPosition.x + m_SizeX * 0.1f, m_MotherPosition.y });
	setSprite(m_MotherSprite, m_MotherTexture, m_MotherPosition);

	m_FamilyTreeTexture = assetHandler.getTextureAtPath("Assets/Graphics/Bloodline3.png");
	setSprite(m_FamilyTreeSprite, m_FamilyTreeTexture, { m_DiplomacySprites.back().getPosition().x + m_SpriteSize * 2, m_DiplomacySprites.back().getPosition().y });
	setText(m_FamilyTreeText, m_Font, m_CharacterSize, m_OwnerColor, { m_FamilyTreeSprite.getPosition().x, m_FamilyTreeSprite.getPosition().y + m_SpriteSize }, "Family Tree");
}

void CharacterWindow::start()
{
	m_Window = Window::getWindow();

	m_PlayerCharacter = &CharacterManager::get().getPlayerCharacter();
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
			m_Window->draw(m_BirthSprite);
			m_Window->draw(m_Birthday);
			m_Window->draw(m_DeadSprite);
			m_Window->draw(m_DeathDate);
		}

		m_Window->draw(m_GoldSprite);
		m_Window->draw(m_GoldText);
		m_Window->draw(m_CharacterNameText);
		m_Window->draw(m_ArmySprite);
		m_Window->draw(m_ArmyText);
		m_Window->draw(m_AgeSprite);
		m_Window->draw(m_CharacterAgeText);
		m_Window->draw(m_FamilyTreeSprite);
		if (m_ShowFamilyInfo)
		{
			m_Window->draw(m_FamilyTreeText);
		}
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

		if (m_FatherID != INVALID_CHARACTER_ID)
		{
			m_Window->draw(m_FatherShape);
			m_Window->draw(m_FatherSprite);
			if (m_ShowFatherName)
			{
				m_Window->draw(m_FatherName);
			}
		}

		if (m_MotherID != INVALID_CHARACTER_ID)
		{
			m_Window->draw(m_MotherShape);
			m_Window->draw(m_MotherSprite);
			if (m_ShowMotherName)
			{
				m_Window->draw(m_MotherName);
			}
		}

		unsigned int numberOfRelations = m_DiplomacyStrings.size();
		for (unsigned int index = 0; index < numberOfRelations; index++)
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

		for (unsigned int index = 0; index < m_TruceShapes.size(); index++)
		{
			m_Window->draw(m_TruceShapes[index]);
			m_Window->draw(m_TruceSprites[index]);
			m_Window->draw(m_TruceTexts[index]);
		}

		for (unsigned int index = 0; index < m_TraitSprites.size(); index++)
		{
			if (m_TraitsEnabled[index])
			{
				m_Window->draw(m_TraitSprites[index]);
				if (m_TraitsShowText[index])
				{
					m_Window->draw(m_TraitTexts[index]);
				}
			}
		}

		if (!m_IsPlayerCharacter && m_CurrentCharacter->m_CharacterTitle != Title::Unlanded)
		{
			unsigned int numberOfButtons = m_ButtonStrings.size();
			for (unsigned int index = 0; index < numberOfButtons; index++)
			{
				m_Window->draw(m_ButtonShapes[index]);
				m_Window->draw(m_ButtonTexts[index]);
			}
		}

		for (unsigned int index = 0; index < m_OwnedRegionShields.size(); index++)
		{
			HeraldicShieldManager::renderShield(*m_OwnedRegionShields[index], { m_OwnedRegionShapes[index].getPosition() });
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
			InputHandler::setRightMouseClicked(false);
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
			InputHandler::setLeftMouseClicked(false);
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

	m_TruceShapes.clear();
	m_TruceSprites.clear();
	m_TruceIDs.clear();
	m_TruceTexts.clear();
}

void CharacterWindow::updateParents()
{
	if (m_FatherID != INVALID_CHARACTER_ID)
	{
		Character& father = CharacterManager::get().getCharacter(m_FatherID);
		m_FatherName.setFillColor(father.m_RegionColor);
		m_FatherName.setString(father.m_Name);
		m_FatherShape.setOutlineColor(father.m_RegionColor);
	}

	if (m_MotherID != INVALID_CHARACTER_ID)
	{
		Character& mother = CharacterManager::get().getCharacter(m_MotherID);
		m_MotherName.setFillColor(mother.m_RegionColor);
		m_MotherName.setString(mother.m_Name);
		m_MotherShape.setOutlineColor(mother.m_RegionColor);
	}
}

void CharacterWindow::enableTrait(Traits trait, unsigned int& traitIndex)
{
	m_TraitsEnabled[(unsigned int)trait] = true;
	m_TraitSprites[(unsigned int)trait].setPosition(m_TraitPositions[traitIndex]);
	m_TraitTexts[(unsigned int)trait].setPosition({ m_TraitPositions[traitIndex].x, m_TraitPositions[traitIndex].y + m_SpriteSize });
	traitIndex++;
}

void CharacterWindow::updateTraits()
{
	for (unsigned int index = 0; index < m_NumberOfTraits; index++)
	{
		m_TraitsEnabled[index] = false;
		m_TraitTexts[index].setFillColor(m_OwnerColor);
	}

	unsigned int traitIndex = 0;
	if (m_Gender == Gender::Male)
	{
		enableTrait(Traits::Male, traitIndex);
	}
	else
	{
		enableTrait(Traits::Female, traitIndex);
	}

	std::stringstream stream;
	enableTrait(Traits::Fertility, traitIndex);
	stream << m_TraitStrings[(unsigned int)Traits::Fertility] << std::fixed << std::setprecision(2) << m_CurrentCharacter->m_Fertility;
	m_TraitTexts[(unsigned int)Traits::Fertility].setString(stream.str());
	stream.str(std::string());
	stream.clear();

	CharacterManager& characterManager = CharacterManager::get();

	if (characterManager.hasTrait(m_CurrentCharacterID, characterManager.getTrait(m_TraitStrings[(unsigned int)Traits::Beautiful])))
	{
		enableTrait(Traits::Beautiful, traitIndex);
	}

	if (characterManager.hasTrait(m_CurrentCharacterID, characterManager.getTrait(m_TraitStrings[(unsigned int)Traits::Ugly])))
	{
		enableTrait(Traits::Ugly, traitIndex);
	}

	if (characterManager.hasTrait(m_CurrentCharacterID, characterManager.getTrait(m_TraitStrings[(unsigned int)Traits::Sterile])))
	{
		enableTrait(Traits::Sterile, traitIndex);
	}

	if (m_CurrentCharacter->m_Spouse != INVALID_CHARACTER_ID)
	{
		enableTrait(Traits::Spouse, traitIndex);
		Character& spouse = characterManager.getCharacter(m_CurrentCharacter->m_Spouse);
		m_TraitTexts[(unsigned int)Traits::Spouse].setFillColor(spouse.m_RegionColor);
		stream << m_TraitStrings[(unsigned int)Traits::Spouse] << spouse.m_Name;
		m_TraitTexts[(unsigned int)Traits::Spouse].setString(stream.str());
		stream.str(std::string());
		stream.clear();
	}

	if (characterManager.hasTrait(m_CurrentCharacterID, characterManager.getTrait(m_TraitStrings[(unsigned int)Traits::Pregnant])))
	{
		enableTrait(Traits::Pregnant, traitIndex);
	}
}

void CharacterWindow::updateChildren()
{
	CharacterManager& characterManager = CharacterManager::get();
	unsigned int sizeChildren = m_CurrentCharacter->m_Children.size();
	for (unsigned int index = 0; index < sizeChildren; index++)
	{
		Character& child = characterManager.getCharacter(m_CurrentCharacter->m_Children[index]);

		sf::RectangleShape childShape;
		setShape(childShape, m_TransparentColor, m_OwnerColor, m_OutlineThickness * 0.5f, { m_SpriteSize, m_SpriteSize }, { m_SizeX * 0.2f, m_SpriteSize * index + m_OutlineThickness * 1.5f * index + m_SpriteSize * 12.0f });

		sf::Text childNameText;
		setText(childNameText, m_Font, m_CharacterSize, m_OwnerColor, { childShape.getPosition().x, childShape.getPosition().y + m_SpriteSize }, child.m_Name);

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
			setSprite(childSprite, m_MaleChildTexture, childShape.getPosition());
		}
		else
		{
			setSprite(childSprite, m_FemaleChildTexture, childShape.getPosition());
		}

		m_ChildrenShapes.push_back(childShape);
		m_ChildrenSprites.push_back(childSprite);
		m_ChildrenNames.push_back(childNameText);
	}
}

void CharacterWindow::updateAllies()
{
	CharacterManager& characterManager = CharacterManager::get();
	m_AlliesIDs = DiplomacyManager::get().getAlliances(m_CurrentCharacterID);
	unsigned int sizeAlliances = m_AlliesIDs.size();
	for (unsigned int index = 0; index < sizeAlliances; index++)
	{
		Character& ally = characterManager.getCharacter(m_AlliesIDs[index]);

		sf::RectangleShape allyShape;
		setShape(allyShape, m_TransparentColor, ally.m_RegionColor, m_OutlineThickness * 0.5f, { m_SpriteSize, m_SpriteSize }, { m_SizeX * 0.3f, m_SpriteSize * index + m_OutlineThickness * 1.5f * index + m_SpriteSize * 12.0f });

		sf::Sprite allySprite;
		if (ally.m_Gender == Gender::Male)
		{
			setSprite(allySprite, m_MaleCharacterTexture, allyShape.getPosition());
		}
		else
		{
			setSprite(allySprite, m_FemaleCharacterTexture, allyShape.getPosition());
		}

		m_AlliesShapes.push_back(allyShape);
		m_AlliesSprites.push_back(allySprite);
	}
}

void CharacterWindow::updateWars()
{
	CharacterManager& characterManager = CharacterManager::get();
	DiplomacyManager& diplomacyManager = DiplomacyManager::get();
	unsigned int sizeWars = diplomacyManager.getWarHandlesOfCharacter(m_CurrentCharacterID).size();
	for (unsigned int index = 0; index < sizeWars; index++)
	{
		int warHandle = diplomacyManager.getWarHandlesOfCharacter(m_CurrentCharacter->m_CharacterID)[index];
		CharacterID defenderID = diplomacyManager.getDefender(warHandle);
		CharacterID attackerID = diplomacyManager.getAttacker(warHandle);
		CharacterID opponentID;
		if (diplomacyManager.isAttacker(warHandle, m_CurrentCharacterID))
		{
			opponentID = defenderID;
		}
		else
		{
			opponentID = attackerID;
		}
		Character& opponent = characterManager.getCharacter(opponentID);

		sf::RectangleShape warShape;
		setShape(warShape, m_TransparentColor, opponent.m_RegionColor, m_OutlineThickness * 0.5f, { m_SpriteSize, m_SpriteSize }, { m_SizeX * 0.4f, m_SpriteSize * index + m_OutlineThickness * 1.5f * index + m_SpriteSize * 12.0f });

		sf::Sprite opponetSprite;
		if (opponent.m_Gender == Gender::Male)
		{
			setSprite(opponetSprite, m_MaleCharacterTexture, warShape.getPosition());
		}
		else
		{
			setSprite(opponetSprite, m_FemaleCharacterTexture, warShape.getPosition());
		}

		m_WarShapes.push_back(warShape);
		m_WarSprites.push_back(opponetSprite);
		m_WarDefenders.push_back(defenderID);
		m_WarAttackers.push_back(attackerID);
	}
}

void CharacterWindow::updateTruces()
{
	CharacterManager& characterManager = CharacterManager::get();
	DiplomacyManager& diplomacyManager = DiplomacyManager::get();
	std::vector<Truce> truces = diplomacyManager.getTruces(m_CurrentCharacterID);
	unsigned int numberOfTruces = truces.size();
	for (unsigned int index = 0; index < numberOfTruces; index++)
	{
		Truce truce = truces[index];
		Character& opponent = characterManager.getCharacter(truce.m_TruceOpponent);

		sf::RectangleShape truceShape;
		setShape(truceShape, m_TransparentColor, opponent.m_RegionColor, m_OutlineThickness * 0.5f, { m_SpriteSize, m_SpriteSize }, { m_SizeX * 0.6f, m_SpriteSize * index + m_OutlineThickness * 1.5f * index + m_SpriteSize * 12.0f });

		sf::Sprite opponetSprite;
		if (opponent.m_Gender == Gender::Male)
		{
			setSprite(opponetSprite, m_MaleCharacterTexture, truceShape.getPosition());
		}
		else
		{
			setSprite(opponetSprite, m_FemaleCharacterTexture, truceShape.getPosition());
		}
		sf::Text truceText;
		std::stringstream stream;
		stream << truce.m_StartDate.m_Day << m_Dash << truce.m_StartDate.m_Month + 1 << m_Dash << truce.m_StartDate.m_Year + 1;
		setText(truceText, m_Font, m_CharacterSize, opponent.m_RegionColor, { truceShape.getPosition().x + m_SpriteSize + m_OutlineThickness, truceShape.getPosition().y }, stream.str().c_str());
		stream.str(std::string());
		stream.clear();
		
		m_TruceTexts.push_back(truceText);
		m_TruceShapes.push_back(truceShape);
		m_TruceSprites.push_back(opponetSprite);
		m_TruceIDs.push_back(truce.m_TruceOpponent);
	}
}

void CharacterWindow::updateInfo()
{
	if (m_CurrentCharacterID != INVALID_CHARACTER_ID)
	{
		CharacterManager& characterManager = CharacterManager::get();
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
		m_FamilyTreeText.setFillColor(m_OwnerColor);

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
		updateTruces();

		Map& map = Map::get();
		unsigned int ownedRegionSize = m_CurrentCharacter->m_OwnedRegionIDs.size();
		for (unsigned int index = 0; index < ownedRegionSize; index++)
		{
			sf::RectangleShape regionShape;
			setShape(regionShape, m_TransparentColor, m_TransparentColor, 0.0f, { m_SpriteSize, m_SpriteSize }, { m_SizeX * 0.1f, m_SpriteSize * index + m_OutlineThickness * 1.5f * index + m_SpriteSize * 12.0f });

			MapRegion& mapRegion = map.getRegionById(m_CurrentCharacter->m_OwnedRegionIDs[index]);
			sf::Text regionNameText;
			setText(regionNameText, m_Font, m_CharacterSize, m_OwnerColor, { regionShape.getPosition().x, regionShape.getPosition().y + m_SpriteSize }, mapRegion.m_RegionName.c_str());

			m_OwnedRegionShapes.push_back(regionShape);
			m_OwnedRegionsTexts.push_back(regionNameText);
			m_OwnedRegionShields.push_back(&mapRegion.m_HeraldicShield);
		}

		std::stringstream stream;
		if (m_Gender == Gender::Male)
		{
			if (m_CurrentCharacter->m_CharacterTitle >= Title::Unlanded)
			{
				stream << m_CurrentCharacter->m_Name;
			}
			else
			{
				stream << m_MaleTitles[(unsigned int)m_CurrentCharacter->m_CharacterTitle] << m_CurrentCharacter->m_Name;
			}
			setSprite(m_CharacterSprite, m_MaleCharacterTexture, { m_SizeX * 0.1f, m_SpriteSize }, m_SpriteSize * 2);
		}
		else
		{
			if (m_CurrentCharacter->m_CharacterTitle >= Title::Unlanded)
			{
				stream << m_CurrentCharacter->m_Name;
			}
			else
			{
				stream << m_FemaleTitles[(unsigned int)m_CurrentCharacter->m_CharacterTitle] << m_CurrentCharacter->m_Name;
			}
			setSprite(m_CharacterSprite, m_FemaleCharacterTexture, { m_SizeX * 0.1f, m_SpriteSize }, m_SpriteSize * 2);
		}
		m_CharacterNameText.setString(stream.str());
		m_CharacterNameText.setFillColor(m_OwnerColor);
		stream.str(std::string());
		stream.clear();

		Unit& unit = UnitManager::get().getUnitOfCharacter(m_CurrentCharacterID);
		if (unit.m_Raised)
		{
			stream << unit.m_RepresentedForce << m_Dash << m_CurrentCharacter->m_MaxArmySize;
			m_ArmyText.setString(stream.str());
		}
		else
		{
			stream << 0 << m_Dash << m_CurrentCharacter->m_MaxArmySize;
			m_ArmyText.setString(stream.str());
		}
		m_ArmyText.setFillColor(m_OwnerColor);
		stream.str(std::string());
		stream.clear();

		stream << std::fixed << std::setprecision(1) << m_CurrentCharacter->m_CurrentGold;
		m_GoldText.setString(stream.str());
		m_GoldText.setFillColor(m_OwnerColor);
		stream.str(std::string());
		stream.clear();

		if (!m_Dead)
		{
			stream << Time::m_GameDate.getAge(m_CurrentCharacter->m_Birthday);
		}
		else
		{
			stream << m_CurrentCharacter->m_AgeAtDeath;
		}
		m_CharacterAgeText.setString(stream.str());
		m_CharacterAgeText.setFillColor(m_OwnerColor);
		stream.str(std::string());
		stream.clear();

		if (m_CurrentCharacter->m_Dead)
		{
			if (m_Gender == Gender::Male)
			{
				setSprite(m_BirthSprite, m_MaleChildTexture, m_BirthPosition);
			}
			else
			{
				setSprite(m_BirthSprite, m_FemaleChildTexture, m_BirthPosition);
			}

			stream << Time::m_GameDate.getDateString(m_CurrentCharacter->m_Birthday);
			m_Birthday.setString(stream.str());
			m_Birthday.setFillColor(m_OwnerColor);
			stream.str(std::string());
			stream.clear();

			stream << Time::m_GameDate.getDateString(m_CurrentCharacter->m_DeathDate);
			m_DeathDate.setString(stream.str());
			m_DeathDate.setFillColor(m_OwnerColor);
			stream.str(std::string());
			stream.clear();
		}
	}
}

void CharacterWindow::handleWindow()
{
	InputHandler::setCharacterWindowOpen(m_Visible);
	if (InputHandler::getRightMouseReleased() && !InputHandler::getPlayerUnitSelected() && m_Open && !m_Visible)
	{
		InputHandler::setRightMouseReleased(false);
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
		m_WindowShape.setSize({ m_SizeX, m_SizeY });
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
		m_WindowShape.setSize({  });
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
			InputHandler::setLeftMouseReleased(false);
			m_ShowSpouseName = true;
		}
		else
		{
			m_ShowSpouseName = false;
		}
		if (m_FatherSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
			m_ShowFatherName = true;
		}
		else
		{
			m_ShowFatherName = false;
		}
		if (m_MotherSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
			m_ShowMotherName = true;
		}
		else
		{
			m_ShowMotherName = false;
		}
		if (m_FamilyTreeSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setLeftMouseReleased(false);
			m_ShowFamilyInfo = true;
		}
		else
		{
			m_ShowFamilyInfo = false;
		}
		for (unsigned int index = 0; index < m_OwnedRegionShapes.size(); index++)
		{
			if (index + 1 > m_OwnedRegionsShowInfo.size())
			{
				m_OwnedRegionsShowInfo.push_back(false);
			}
			if (m_OwnedRegionShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				InputHandler::setLeftMouseReleased(false);
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
				InputHandler::setLeftMouseReleased(false);
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
				InputHandler::setLeftMouseReleased(false);
				m_ShowChildrenNames[index] = true;
			}
			else
			{
				m_ShowChildrenNames[index] = false;
			}
		}
		for (unsigned int index = 0; index < m_TraitSprites.size(); index++)
		{
			if (index + 1 > m_TraitsShowText.size())
			{
				m_TraitsShowText.push_back(false);
			}
			if (m_TraitSprites[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				InputHandler::setLeftMouseReleased(false);
				m_TraitsShowText[index] = true;
			}
			else
			{
				m_TraitsShowText[index] = false;
			}
		}
	}
	if (InputHandler::getRightMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		UIManager& uiManager = UIManager::get();
		if (m_FamilyTreeSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setRightMouseReleased(false);
			FamilyTreeWindow& familyTreeWindow = *uiManager.m_FamilyTreeWindow;
			familyTreeWindow.m_CurrentCharacterID = m_CurrentCharacterID;
			familyTreeWindow.openWindow();
		}
		else if (m_FatherSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setRightMouseReleased(false);
			m_CurrentCharacterID = m_FatherID;
			checkIfPlayerCharacter();
			updateInfo();
		}
		else if (m_MotherSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setRightMouseReleased(false);
			m_CurrentCharacterID = m_MotherID;
			checkIfPlayerCharacter();
			updateInfo();
		}
		CharacterManager& characterManager = CharacterManager::get();
		for (unsigned int index = 0; index < m_ChildrenShapes.size(); index++)
		{
			if (m_ChildrenShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				InputHandler::setRightMouseReleased(false);
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
				InputHandler::setRightMouseReleased(false);
				m_CurrentCharacterID = m_AlliesIDs[index];
				checkIfPlayerCharacter();
				updateInfo();
				break;
			}
		}
		DiplomacyManager& diplomacyManager = DiplomacyManager::get();
		for (unsigned int index = 0; index < m_WarShapes.size(); index++)
		{
			if (m_WarShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				InputHandler::setRightMouseReleased(false);
				std::vector<int> wars = diplomacyManager.getWarHandlesOfCharacter(m_CurrentCharacter->m_CharacterID);
				m_CurrentCharacterID = diplomacyManager.getOpposingForce(wars[index], m_CurrentCharacterID);
				checkIfPlayerCharacter();
				updateInfo();
				break;
			}
		}
		for (unsigned int index = 0; index < m_TruceShapes.size(); index++)
		{
			if (m_TruceShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				InputHandler::setRightMouseReleased(false);
				m_CurrentCharacterID = m_TruceIDs[index];
				checkIfPlayerCharacter();
				updateInfo();
				break;
			}
		}
		for (unsigned int index = 0; index < m_OwnedRegionShapes.size(); index++)
		{
			if (m_OwnedRegionShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				InputHandler::setRightMouseReleased(false);
				closeWindow();
				RegionWindow& regionWindow = *uiManager.m_RegionWindow;
				regionWindow.m_CurrentMapRegion = &Map::get().getRegionById(m_CurrentCharacter->m_OwnedRegionIDs[index]);
				regionWindow.checkIfPlayerRegion();
				regionWindow.openWindow();
				regionWindow.updateInfo();
			}
		}
	}
	if (InputHandler::getLeftMouseReleased() && !m_IsPlayerCharacter && m_CurrentCharacter->m_CharacterTitle != Title::Unlanded)
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		unsigned int numberOfButtons = m_ButtonStrings.size();
		for (unsigned int index = 0; index < numberOfButtons; index++)
		{
			if (m_ButtonShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				InputHandler::setLeftMouseReleased(false);
				switch (index)
				{
				case 0:
				{
					declareWar();
					break;
				}
				case 1:
				{
					proposeMarriage();
					break;
				}
				case 2:
				{
					proposeAlliance();
					break;
				}
				case 3:
				{
					breakAlliance();
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
		UIManager& uiManager = UIManager::get();
		for (unsigned int index = 0; index < m_WarShapes.size(); index++)
		{
			if (m_WarShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				InputHandler::setLeftMouseReleased(false);
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
	UIManager& uiManager = UIManager::get();
	for (CharacterID& allyID : DiplomacyManager::get().getAlliances(m_PlayerCharacter->m_CharacterID))
	{
		if (allyID == m_CurrentCharacterID)
		{
			uiManager.createUIEventElement(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, UIType::TriedToDeclareWarOnAlly);
			return;
		}
	}
	DiplomacyManager& diplomacyManager = DiplomacyManager::get();
	War* war = diplomacyManager.getWarAgainst(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID);
	if (war == nullptr)
	{
		if (diplomacyManager.hasTruce(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID))
		{
			uiManager.createUIEventElement(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, UIType::TruceMessage);
			return;
		}

		int warHandle = DiplomacyManager::get().createWar(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, m_CurrentRegionID);

		AIManager& aiManager = AIManager::get();
		aiManager.getWarmindOfCharacter(m_CurrentCharacterID).m_Active = true;
		aiManager.getWarmindOfCharacter(m_CurrentCharacterID).m_Opponent = m_PlayerCharacter->m_CharacterID;
		aiManager.getAIDataofCharacter(m_CurrentCharacterID).m_CurrentAction = Action::War;
		aiManager.getAIDataofCharacter(m_CurrentCharacterID).m_LastAction = Action::War;

		uiManager.createUIEventElement(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, UIType::WarDeclaration);
		uiManager.createWarIcon(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID);

		CharacterManager::get().callAllies(m_PlayerCharacter->m_CharacterID, warHandle);

		Game::m_Sound.pause();
		if (Game::m_BattleSound.getStatus() != sf::SoundSource::Playing)
		{
			Game::m_BattleSound.play();
		}
	}
}

void CharacterWindow::proposeMarriage()
{
	CharacterManager::get().marry(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID);
}

void CharacterWindow::proposeAlliance()
{
	CharacterManager::get().sendAllianceOffer(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID);
}

void CharacterWindow::breakAlliance()
{
	std::vector<unsigned int> alliances = DiplomacyManager::get().getAlliances(m_PlayerCharacter->m_CharacterID);
	for (unsigned int allyID : alliances)
	{
		if (allyID == m_CurrentCharacterID)
		{
			DiplomacyManager::get().breakAlliance(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID);
			UIManager::get().createUIEventElement(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, UIType::AllianceBroken);
		}
	}
}

void CharacterWindow::assassinate()
{
	CharacterManager& characterManager = CharacterManager::get();
	if (characterManager.getCharacter(m_CurrentCharacterID).m_Dead)
	{
		return;
	}
	if (m_PlayerCharacter->m_CurrentGold >= m_AssassinationCost)
	{
		m_PlayerCharacter->m_CurrentGold -= m_AssassinationCost;
		if (characterManager.chancePerPercent(0.5f))
		{
			UIManager::get().createUIEventElement(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, UIType::AssassinationSuccess);
			characterManager.killCharacter(m_CurrentCharacterID);
		}
		else
		{
			UIManager::get().createUIEventElement(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, UIType::AssassinationFailure);
		}
	}
	else
	{
		UIManager::get().createUIEventElement(m_PlayerCharacter->m_CharacterID, m_CurrentCharacterID, UIType::CannotAffordMessage, m_AssassinationCost);
	}
}

bool CharacterWindow::checkIfPlayerCharacter()
{
	return m_IsPlayerCharacter = m_CurrentCharacterID == m_PlayerCharacter->m_CharacterID;
}

void CharacterWindow::setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position)
{
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThickness);
	shape.setSize(size);
	shape.setPosition(position);
}

void CharacterWindow::setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string)
{
	text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setFillColor(fillColor);
	text.setPosition(position);
	text.setString(string);
}

void CharacterWindow::setSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, unsigned int spriteSize)
{
	sprite.setTexture(texture, true);
	sprite.setScale(spriteSize / sprite.getLocalBounds().width, spriteSize / sprite.getLocalBounds().height);
	sprite.setPosition(position);
}