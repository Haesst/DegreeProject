#include <Game/UI/ArmyWindow.h>
#include <Engine/AssetHandler.h>
#include <Engine/Window.h>
#include <Game/Systems/CharacterManager.h>
#include <Engine/InputHandler.h>
#include <Game/Map/Map.h>
#include <Game/Data/CharacterConstants.h>

ArmyWindow::ArmyWindow(UIID id, sf::Font font, Vector2D position, Vector2D size) : UIWindow(id, font, position, size)
{
	m_CharacterSize = 25;
	m_OutlineThickness = 10;
	m_TextStrings = { "", "" };
	m_ButtonStrings = { "No\nLeader" };
}

void ArmyWindow::start()
{
	UIWindow::start();

	m_CurrentCharacterID = CharacterManager::get().getPlayerCharacter().m_CharacterID;
	setShape(m_WindowShape, m_WindowFillColor, m_OutlineColor, m_OutlineThickness, {  }, { m_OutlineThickness, m_OutlineThickness });
	for (unsigned int index = 0; index < m_TextStrings.size(); index++)
	{
		m_Texts.push_back(sf::Text());
		setText(m_Texts[index], m_Font, m_CharacterSize, m_TextFillColor, { m_SizeX * 0.25f, (float)(m_SpriteSize + m_SpriteSize * index) }, m_TextStrings[index].c_str());
	}
	sf::Vector2f armyPosition = { m_SizeX * 0.1f, (float)(m_SpriteSize * 6) };
	setText(m_ArmyText, m_Font, m_CharacterSize, m_TextFillColor, { armyPosition.x + m_SpriteSize * 1.5f, armyPosition.y });
	m_Textures.push_back(AssetHandler::get().getTextureAtPath("Assets/Graphics/soldier unit.png"));
	for (unsigned int index = 0; index < m_Textures.size(); index++)
	{
		m_Sprites.push_back(sf::Sprite());
		setSprite(m_Sprites[index], m_Textures[index], armyPosition);
	}
	for (unsigned int index = 0; index < m_ButtonStrings.size(); index++)
	{
		m_Buttons.push_back(new Button(m_Font, m_Window, { m_SizeX * 0.1f, 32 }, { 64, 64 }));
		m_Buttons[index]->setShape(m_TransparentColor, m_OutlineColor, m_OutlineThickness * 0.5f, { 64, 64 }, { m_SizeX * 0.1f, 32 });
		m_Buttons[index]->setText(m_Font, m_CharacterSize, m_OutlineColor, { m_SizeX * 0.1f, 32 }, m_ButtonStrings[index]);
		m_Buttons[index]->m_SpriteVisible = false;
	}
}

void ArmyWindow::update()
{
	UIWindow::update();

	if (m_Visible)
	{
		clickButton();
	}
}

void ArmyWindow::render()
{
	UIWindow::render();

	if (m_Visible)
	{
		m_Window->draw(m_ArmyText);
	}
}

void ArmyWindow::onDayChange()
{
	if (m_Visible)
	{
		updateInfo();
	}
}

void ArmyWindow::updateInfo()
{
	if (m_CurrentCharacterID != INVALID_CHARACTER_ID)
	{
		Character& character = CharacterManager::get().getCharacter(m_CurrentCharacterID);
		m_CurrentCharacterID = character.m_CharacterID;
		m_OutlineColor = character.m_RegionColor;
		m_TextFillColor = m_OutlineColor;

		m_WindowShape.setOutlineColor(m_OutlineColor);
		 
		m_TextStrings.clear();
		std::string title;
		if (character.m_Gender == Gender::Male)
		{
			title = CharacterConstants::m_MaleTitles[(int)character.m_CharacterTitle];
		}
		else
		{
			title = CharacterConstants::m_FemaleTitles[(int)character.m_CharacterTitle];
		}
		m_TextStrings.push_back(title + character.m_Name);
		m_TextStrings.push_back(m_ArmyOf + character.m_KingdomName);

		for (unsigned int index = 0; index < m_TextStrings.size(); index++)
		{
			setText(m_Texts[index], m_Font, m_CharacterSize, m_TextFillColor, m_Texts[index].getPosition(), m_TextStrings[index].c_str());
		}

		const char* portraitPath = character.m_PortraitPath.c_str();
		for (Button* button : m_Buttons)
		{
			button->setSprite(portraitPath, { m_SizeX * 0.1f, (float)(button->m_SpriteSize) }, 2.0f);
			button->m_Shape.setOutlineColor(m_OutlineColor);
			button->m_Text.setFillColor(m_TextFillColor);
		}
	}
}

void ArmyWindow::openWindow()
{
	if (!m_Visible)
	{
		m_WindowShape.setSize({ m_SizeX, m_SizeY });
		m_DaySubscriptionHandle = Time::m_GameDate.subscribeToDayChange([](void* data) { ArmyWindow& armyWindow = *static_cast<ArmyWindow*>(data); armyWindow.onDayChange(); }, static_cast<void*>(this));
	}	
	
	UIWindow::openWindow();
}

void ArmyWindow::closeWindow()
{
	if (m_Visible)
	{
		Time::m_GameDate.unsubscribeToDayChange(m_DaySubscriptionHandle);
		m_WindowShape.setSize({  });
	}	
	
	UIWindow::closeWindow();
}

void ArmyWindow::clickButton()
{
	UIWindow::clickButton();

	if (InputHandler::getLeftMouseReleased())
	{
 		Vector2D mousePositionUI = InputHandler::getUIMousePosition();
		for (Button* button : m_Buttons)
		{
			if (button->m_Sprite.getGlobalBounds().contains(mousePositionUI.x, mousePositionUI.y))
			{
				button->m_SpriteVisible = !button->m_SpriteVisible;
				button->m_TextVisible = !button->m_TextVisible;
				Character& character = CharacterManager::get().getCharacter(m_CurrentCharacterID);
				character.m_LeadingArmy = !character.m_LeadingArmy;
				InputHandler::setLeftMouseReleased(false);
			}
		}
	}
}
