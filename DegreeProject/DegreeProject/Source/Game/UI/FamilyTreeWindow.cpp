#include "Game/UI/FamilyTreeWindow.h"
#include <SFML/Graphics.hpp>
#include "Engine/Window.h"
#include "Engine/InputHandler.h"
#include "Engine/AssetHandler.h"
#include "Game/Data/Character.h"
#include "Game/Systems/CharacterManager.h"
#include "Game/Data/CharacterConstants.h"
#include <sstream>
#include "Game/UI/UIManager.h"
#include "Game/UI/CharacterWindow.h"

FamilyTreeWindow::FamilyTreeWindow(UIID id, sf::Font font, Vector2D position, Vector2D size)
{
	m_OwnedUIWindow = id;
	m_Font = font;
	m_SizeX = size.x;
	m_SizeY = size.y;
	m_PositionX = position.x;
	m_PositionY = position.y;

	setShape(m_WindowShape, m_FillColor, m_OwnerColor, m_OutlineThickness, {  }, { m_PositionX, m_PositionY });
	setText(m_WindowTitle, m_Font, m_CharacterSize, m_OwnerColor, { m_PositionX + m_SizeX * 0.5f - m_SpriteSize, m_PositionY }, "Family Tree");

	AssetHandler& assetHandler = AssetHandler::get();

	m_MaleCharacterTexture = assetHandler.getTextureAtPath("Assets/Graphics/MalePortrait.jpg");
	m_FemaleCharacterTexture = assetHandler.getTextureAtPath("Assets/Graphics/FemalePortrait.jpg");

	m_MaleChildTexture = assetHandler.getTextureAtPath("Assets/Graphics/BabyMale.png");
	m_FemaleChildTexture = assetHandler.getTextureAtPath("Assets/Graphics/BabyFemale.png");

	m_DeadTexture = assetHandler.getTextureAtPath("Assets/Graphics/Dead.png");

	clearInfo();
}

void FamilyTreeWindow::start()
{
	m_Window = Window::getWindow();
}

void FamilyTreeWindow::update()
{
	if (m_Visible)
	{
		clickButton();
	}
	handleWindow();
}

void FamilyTreeWindow::render()
{
	if (m_Visible)
	{
		m_Window->draw(m_WindowShape);
		m_Window->draw(m_WindowTitle);

		for (unsigned int index = 0; index < m_CharacterShapes.size(); index++)
		{
			m_Window->draw(m_CharacterShapes[index]);
			m_Window->draw(m_CharacterSprites[index]);
			m_Window->draw(m_CharacterDeadSprites[index]);
			m_Window->draw(m_CharacterInfo[index]);
		}
	}
}

void FamilyTreeWindow::onDayChange()
{
	if (m_Visible)
	{
		clearInfo();
		updateInfo();
	}
}

void FamilyTreeWindow::clearInfo()
{
	m_CharacterShapes.clear();
	m_CharacterSprites.clear();
	m_CharacterIDs.clear();
	m_CharacterInfo.clear();
	m_ShowCharacterInfo.clear();
	m_CharacterDeadSprites.clear();
	m_FamilySize = 0;
}

void FamilyTreeWindow::displayFamily(CharacterID& familyHeadID, unsigned int depth, float width)
{
	if (familyHeadID == INVALID_CHARACTER_ID)
	{
		return;
	}
	Character& character = CharacterManager::get().getCharacter(familyHeadID);
	setFamilyMember(familyHeadID, depth, width);
	unsigned int numberOfSpouses = character.m_TotalSpouses.size();
	for (unsigned int index = 0; index < numberOfSpouses; index++)
	{
		setFamilyMember(character.m_TotalSpouses[index], depth, ++width);
	}
	displayFamily(character.m_OldestChild, depth + 1, character.m_Children.size() * 0.5f - character.m_Children.size() + 1);
	displayFamily(character.m_NextSibling, depth, width + 1);
}

CharacterID& FamilyTreeWindow::findFamilyHead(CharacterID& characterID)
{
	Character& character = CharacterManager::get().getCharacter(characterID);
	if (character.m_Father == INVALID_CHARACTER_ID)
	{
		return characterID;
	}
	findFamilyHead(character.m_Father);
}

void FamilyTreeWindow::updateInfo()
{
	if (m_CurrentCharacterID != INVALID_CHARACTER_ID)
	{
		CharacterManager& characterManager = CharacterManager::get();
		m_CurrentCharacter = &characterManager.getCharacter(m_CurrentCharacterID);
		if (!m_CurrentCharacter->m_Dead)
		{
			m_OwnerColor = m_CurrentCharacter->m_RegionColor;
		}
		else
		{
			m_OwnerColor = m_CurrentCharacter->m_ColorAtDeath;
		}
		m_WindowShape.setOutlineColor(m_OwnerColor);
		m_WindowTitle.setFillColor(m_OwnerColor);

		displayFamily(findFamilyHead(m_CurrentCharacterID), 0, 0.0f);
	}
}

void FamilyTreeWindow::handleWindow()
{
	InputHandler::setFamilyTreeWindowOpen(m_Visible);
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
		if (!m_WindowShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			closeWindow();
		}
	}
	else if (m_Visible && (InputHandler::getEscapePressed() || InputHandler::getRegionWindowOpen() || InputHandler::getWarWindowOpen()))
	{
		closeWindow();
	}
}

void FamilyTreeWindow::openWindow()
{
	if (!m_Visible)
	{
		m_WindowShape.setSize({ m_SizeX, m_SizeY });
		m_DaySubscriptionHandle = Time::m_GameDate.subscribeToDayChange([](void* data) { FamilyTreeWindow& familyTreeWindow = *static_cast<FamilyTreeWindow*>(data); familyTreeWindow.onDayChange(); }, static_cast<void*>(this));
		m_Visible = true;
		InputHandler::setFamilyTreeWindowOpen(true);
	}
}

void FamilyTreeWindow::closeWindow()
{
	if (m_Visible)
	{
		Time::m_GameDate.unsubscribeToDayChange(m_DaySubscriptionHandle);
		m_WindowShape.setSize({  });
		m_Open = false;
		m_Visible = false;
		InputHandler::setFamilyTreeWindowOpen(false);
	}
}

void FamilyTreeWindow::clickButton()
{
	if (InputHandler::getRightMouseReleased())
	{
		Vector2D mousePosition = InputHandler::getUIMousePosition();
		for (unsigned int index = 0; index < m_CharacterShapes.size(); index++)
		{
			if (m_CharacterShapes[index].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
			{
				InputHandler::setRightMouseReleased(false);
				CharacterWindow& characterWindow = *UIManager::get().m_CharacterWindow;
				characterWindow.m_CurrentCharacterID = m_CharacterIDs[index];
				characterWindow.checkIfPlayerCharacter();
				characterWindow.updateInfo();
				characterWindow.openWindow();
				break;
			}
		}
		if (!m_WindowShape.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			InputHandler::setRightMouseReleased(false);
			closeWindow();
		}
	}
}

void FamilyTreeWindow::setFamilyMember(CharacterID& characterID, unsigned int depth, float width)
{
	for (CharacterID familyID : m_CharacterIDs)
	{
		if (familyID == characterID)
		{
			return;
		}
	}
	m_CharacterIDs.push_back(characterID);
	m_ShowCharacterInfo.push_back(false);
	Character& character = CharacterManager::get().getCharacter(characterID);
	sf::Color characterColor;
	unsigned int characterTitle;
	sf::Vector2f characterPosition = { m_PositionX + m_SizeX * 0.5f - m_SpriteSize * 3.5f + m_OutlineThickness + m_SpriteSize * 7 * width, m_PositionY + m_OutlineThickness + m_SpriteSize * 2 + m_SpriteSize * 3 * depth };
	Date deathDate;
	if (!character.m_Dead)
	{
		characterColor = character.m_RegionColor;
		characterTitle = (unsigned int)character.m_CharacterTitle;
		m_CharacterDeadSprites.push_back(sf::Sprite());
	}
	else
	{
		characterColor = character.m_ColorAtDeath;
		characterTitle = (unsigned int)character.m_TitleAtDeath;
		deathDate = character.m_DeathDate;
		sf::Sprite deadSprite;
		setSprite(deadSprite, m_DeadTexture, { characterPosition.x + m_OutlineThickness + m_SpriteSize, characterPosition.y });
		m_CharacterDeadSprites.push_back(deadSprite);
	}
	sf::RectangleShape characterShape;
	setShape(characterShape, m_TransparentColor, characterColor, m_OutlineThickness * 0.5f, { m_SpriteSize, m_SpriteSize }, characterPosition);
	m_CharacterShapes.push_back(characterShape);
	sf::Sprite characterSprite;
	if (character.m_Gender == Gender::Male)
	{
		if (Time::m_GameDate.getAge(character.m_Birthday) < CharacterConstants::m_AgeOfConsent)
		{
			setSprite(characterSprite, m_MaleChildTexture, characterShape.getPosition());
		}
		else
		{
			setSprite(characterSprite, m_MaleCharacterTexture, characterShape.getPosition());
		}
	}
	else
	{
		characterTitle += (unsigned int)Title::Unlanded + 1;
		if (Time::m_GameDate.getAge(character.m_Birthday) < CharacterConstants::m_AgeOfConsent)
		{
			setSprite(characterSprite, m_FemaleChildTexture, characterShape.getPosition());
		}
		else
		{
			setSprite(characterSprite, m_FemaleCharacterTexture, characterShape.getPosition());
		}
	}
	m_CharacterSprites.push_back(characterSprite);
	std::stringstream stream;
	stream << /*m_CharacterTitles[characterTitle] <<*/ character.m_Name << "\n" << character.m_Birthday.m_Day << "-" << character.m_Birthday.m_Month << "-" << character.m_Birthday.m_Year;
	if (character.m_Dead)
	{
		stream << " to " << character.m_DeathDate.m_Day << "-" << character.m_DeathDate.m_Month << "-" << character.m_DeathDate.m_Year;
	}
	sf::Text characterText;
	setText(characterText, m_Font, m_CharacterSize, characterColor, { characterShape.getPosition().x, characterShape.getPosition().y + m_SpriteSize}, stream.str().c_str());
	m_CharacterInfo.push_back(characterText);
}

void FamilyTreeWindow::setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position)
{
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThickness);
	shape.setSize(size);
	shape.setPosition(position);
}

void FamilyTreeWindow::setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string)
{
	text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setFillColor(fillColor);
	text.setPosition(position);
	text.setString(string);
}

void FamilyTreeWindow::setSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, unsigned int spriteSize)
{
	sprite.setTexture(texture, true);
	sprite.setScale(spriteSize / sprite.getLocalBounds().width, spriteSize / sprite.getLocalBounds().height);
	sprite.setPosition(position);
}
