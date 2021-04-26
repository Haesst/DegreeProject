#include "UIManager.h"
#include "Game/UI/MainMenu.h"
#include "Game/UI/CharacterWindow.h"
#include "Game/UI/RegionWindow.h"
#include "Game/UI/StatBar.h"
#include "Game/UI/DateBar.h"
#include "Game/UI/UIText.h"
#include "Game/UI/EventWindow.h"
#include "Game/UI/WarWindow.h"
#include "Game/UI/WarIcon.h"
#include "Game/UI/PauseWindow.h"
#include "Game/UI/FamilyTreeWindow.h"
#include "Game/Game.h"

UIManager* UIManager::m_Instance = nullptr;
UIID UIManager::m_UIElementsIDs = 1;

UIManager& UIManager::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new UIManager();
	}
	return *m_Instance;
}

UIManager::UIManager()
{
	m_MainMenu = nullptr;
	m_CharacterWindow = nullptr;
	m_FamilyTreeWindow = nullptr;
	m_RegionWindow = nullptr;
	m_StatBar = nullptr;
	m_DateBar = nullptr;
	m_WarWindow = nullptr;
}

UIManager::~UIManager()
{
	delete m_MainMenu;
	delete m_CharacterWindow;
	delete m_RegionWindow;
	delete m_StatBar;
	delete m_DateBar;
	delete m_WarWindow;
	delete m_FamilyTreeWindow;
	for (std::unordered_map<CharacterID, UIText*>::iterator itr = m_UITexts.begin(); itr != m_UITexts.end(); itr++)
	{
		delete itr->second;
	}
	for (unsigned int i = 0; i < m_EventWindows.size(); i++)
	{
		delete m_EventWindows[i];
	}
	for (unsigned int i = 0; i < m_WarIcons.size(); i++)
	{
		delete m_WarIcons[i];
	}
	delete m_Instance;
}

UIID UIManager::createUIEventElement(CharacterID instigatorID, CharacterID subjectID, UIType type, float giftAmount)
{
	UIID ID = m_UIElementsIDs++;
	UIElement uiElement;
	uiElement.m_Type = type;
	m_EventWindows.insert(std::pair(ID, new EventWindow(ID, Game::m_UIFont, instigatorID, subjectID, type, giftAmount)));
	m_UIElements.push_back(uiElement);
	return ID;
}

UIID UIManager::createWarIcon(CharacterID attackerID, CharacterID defenderID)
{
	UIID ID = m_UIElementsIDs++;
	UIElement uiElement;
	uiElement.m_Type = UIType::WarIcon;
	int index = m_WarIcons.size();
	m_WarIcons.insert(std::pair(ID, new WarIcon(ID, Game::m_UIFont, index, attackerID, defenderID)));
	m_UIElements.push_back(uiElement);
	return ID;
}

UIID UIManager::createUITextElement(sf::Font font, CharacterID characterID, std::string countryName, std::vector<unsigned int> ownedRegions)
{
	UIID id = m_UIElementsIDs++;
	UIElement uiElement;
	uiElement.m_UIElementID = id;
	m_UITexts.insert(std::pair(characterID, new UIText(id, font, countryName, ownedRegions)));
	m_UIElements.push_back(uiElement);
	return id;
}

void UIManager::AdjustOwnership(CharacterID conquerorID, CharacterID loserID, unsigned int regionID)
{
	Map::get().getRegionById(regionID).m_OwnerID = conquerorID;
	m_UITexts[conquerorID]->conquerRegion(regionID);
	m_UITexts[loserID]->loseRegion(regionID);
}

void UIManager::AdjustOwnerships(CharacterID conquerorID, CharacterID loserID, std::vector<unsigned int>& regionIDs)
{
	for (auto& id : regionIDs)
	{
		AdjustOwnership(conquerorID, loserID, id);
	}
}

void UIManager::SetRealmTextAsConquered(CharacterID characterID)
{
	m_UITexts[characterID]->m_Conquered = true;
}

void UIManager::SetRealmNameOnText(CharacterID characterID, std::string realmName)
{
	m_UITexts[characterID]->m_CountryName = realmName;
}

UIID UIManager::createUIWindowElement(sf::Font font, UIType type, Vector2D position, Vector2D size)
{
	UIID id = m_UIElementsIDs++;
	UIElement uiElement;
	uiElement.m_UIElementID = id;
	uiElement.m_Position = position;
	uiElement.m_Size = size;
	uiElement.m_Type = type;
	switch (type)
	{
		case UIType::MainMenu:
		{
			m_MainMenu = new MainMenu(id, font, position, size);
			break;
		}
		case UIType::PauseWindow:
		{
			m_PauseWindow = new PauseWindow(id, font, position, size);
			break;
		}
		case UIType::CharacterWindow:
		{
			m_CharacterWindow = new CharacterWindow(id, font, position, size);
			break;
		}
		case UIType::FamilyTreeWindow:
		{
			m_FamilyTreeWindow = new FamilyTreeWindow(id, font, position, size);
			break;
		}
		case UIType::RegionWindow:
		{
			m_RegionWindow = new RegionWindow(id, font, position, size);
			break;
		}
		case UIType::WarWindow:
		{
			m_WarWindow = new WarWindow(id, font, position, size);
			break;
		}
		case UIType::StatBar:
		{
			m_StatBar = new StatBar(id, font, position, size);
			break;
		}
		case UIType::DateBar:
		{
			m_DateBar = new DateBar(id, font, position, size);
			break;
		}
		default:
		{
			break;
		}
	}
	m_UIElements.push_back(uiElement);
	return id;
}

void UIManager::start()
{
	ASSERT(m_MainMenu != nullptr, "Main Menu does not exist");
	ASSERT(m_PauseWindow != nullptr, "Pause Window does not exist");
	ASSERT(m_CharacterWindow != nullptr, "Character Window does not exist");
	ASSERT(m_FamilyTreeWindow != nullptr, "Family Tree Window does not exist");
	ASSERT(m_RegionWindow != nullptr, "Region Window does not exist");
	ASSERT(m_WarWindow != nullptr, "War Window does not exist");
	ASSERT(m_StatBar != nullptr, "Stat Bar does not exist");
	ASSERT(m_DateBar != nullptr, "Date Bar does not exist");
	for (std::pair<CharacterID, UIText*> uiTextPair : m_UITexts)
	{
		uiTextPair.second->start();
	}
	m_FamilyTreeWindow->start();
	m_CharacterWindow->start();
	m_RegionWindow->start();
	m_WarWindow->start();
	m_StatBar->start();
	m_DateBar->start();
}

void UIManager::update()
{
	m_MainMenu->update();
	for (std::map<UIID, EventWindow*>::reverse_iterator eventWindowIterator = m_EventWindows.rbegin(); eventWindowIterator != m_EventWindows.rend(); ++eventWindowIterator)
	{
		if (!(*eventWindowIterator).second->m_Dismissed)
		{
			(*eventWindowIterator).second->update();
		}
		else
		{
			m_EventWindowsToRemove.push_back((*eventWindowIterator).first);
		}
	}
	for (UIID uiID : m_EventWindowsToRemove)
	{
		delete m_EventWindows[uiID];
		m_EventWindows.erase(uiID);
	}
	m_EventWindowsToRemove.clear();
	for (std::pair<CharacterID, UIText*> uiTextPair : m_UITexts)
	{
		if(!uiTextPair.second->m_Conquered)
		{
			uiTextPair.second->update();
		}
		else
		{
			m_UITextsToRemove.push_back(uiTextPair.first);
		}
	}
	for (CharacterID characterID : m_UITextsToRemove)
	{
		delete m_UITexts[characterID];
		m_UITexts.erase(characterID);
	}
	m_UITextsToRemove.clear();
	m_DateBar->update();
	m_WarWindow->update();
	for (std::pair<UIID, WarIcon*> warIconPair : m_WarIcons)
	{
		if (warIconPair.second->m_Active)
		{
			if (m_MoveWarIcon)
			{
				warIconPair.second->updatePosition(warIconPair.second->m_Index - 1);
			}
			warIconPair.second->update();
		}
		else
		{
			m_WarIconsToRemove.push_back(warIconPair.first);
			m_MoveWarIcon = true;
		}
	}
	m_MoveWarIcon = false;
	for (UIID uiID : m_WarIconsToRemove)
	{
		delete m_WarIcons[uiID];
		m_WarIcons.erase(uiID);
	}
	m_WarIconsToRemove.clear();
	m_FamilyTreeWindow->update();
	m_CharacterWindow->update();
	m_RegionWindow->update();
}

void UIManager::render()
{
	for (std::pair<CharacterID, UIText*> uiTextPair : m_UITexts)
	{
		uiTextPair.second->render();
	}
	for (std::pair<UIID, WarIcon*> warIconPair : m_WarIcons)
	{
		warIconPair.second->render();
	}
	m_FamilyTreeWindow->render();
	m_CharacterWindow->render();
	m_RegionWindow->render();
	m_WarWindow->render();
	m_StatBar->render();
	m_DateBar->render();
	for (std::pair<UIID, EventWindow*> eventWindowPair : m_EventWindows)
	{
		eventWindowPair.second->render();
	}
	m_PauseWindow->render();
	m_MainMenu->render();
}

#pragma warning(push)
#pragma warning(disable: 4239 4172)
UIElement& UIManager::getUIElement(UIID id)
{
	ASSERT(id != INVALID_UI_ID, "Invalid UI id requested");

	for (UIElement& uiElement : m_UIElements)
	{
		if (uiElement.m_UIElementID == id)
		{
			return uiElement;
		}
	}
	
	ASSERT(false, "UI id not found");
	return UIElement();
}
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable: 4239 4172)
UIText& UIManager::getUIText(UIID id)
{
	ASSERT(id != INVALID_UI_ID, "Invalid UI id requested");

	for (std::pair<CharacterID, UIText*> uiTextPair : m_UITexts)
	{
		if (uiTextPair.second->m_OwnedUIElement == id)
		{
			return *uiTextPair.second;
		}
	}

	ASSERT(false, "UI id not found");
	return *m_UITexts[0];
}
#pragma warning(pop)
