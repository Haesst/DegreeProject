#include "UIManager.h"

UIManager* UIManager::m_Instance = nullptr;
UIID UIManager::m_UIElementsIDs = 1;

UIManager* UIManager::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new UIManager();
	}
	return m_Instance;
}

UIManager::UIManager()
{
	m_CharacterWindow = nullptr;
	m_RegionWindow = nullptr;
	m_StatBar = nullptr;
	m_DateBar = nullptr;
}

UIManager::~UIManager()
{
	delete m_CharacterWindow;
	delete m_RegionWindow;
	delete m_StatBar;
	delete m_DateBar;
	for (UIText* uiText : m_UITexts)
	{
		delete uiText;
	}
	delete m_Instance;
}

UIID UIManager::createUITextElement(sf::Font font, std::string countryName, std::vector<unsigned int> ownedRegions)
{
	UIID id = m_UIElementsIDs++;
	UIElement uiElement;
	uiElement.m_UIElementID = id;
	m_UITexts.push_back(new UIText(id, font, countryName, ownedRegions));
	m_UIElements.push_back(uiElement);
	return id;
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
		case UIType::CharacterWindow:
		{
			m_CharacterWindow = new CharacterWindow(id, font);
			break;
		}
		case UIType::RegionWindow:
		{
			m_RegionWindow = new RegionWindow(id, font);
			break;
		}
		case UIType::StatBar:
		{
			m_StatBar = new StatBar(id, font);
			break;
		}
		case UIType::DateBar:
		{
			m_DateBar = new DateBar(id, font);
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
	ASSERT(m_CharacterWindow != nullptr, "Character Window does not exist");
	ASSERT(m_RegionWindow != nullptr, "Region Window does not exist");
	ASSERT(m_StatBar != nullptr, "Stat Bar does not exist");
	ASSERT(m_DateBar != nullptr, "Date Bar does not exist");
	for (UIText* uiText : m_UITexts)
	{
		uiText->start();
	}
	m_StatBar->start();
	m_DateBar->start();
	m_CharacterWindow->start();
	m_RegionWindow->start();
}

void UIManager::update()
{
	m_CharacterWindow->update();
	m_RegionWindow->update();
	m_StatBar->update();
	m_DateBar->update();
}

void UIManager::render()
{
	for (UIText* uiText : m_UITexts)
	{
		uiText->render();
	}
	m_CharacterWindow->render();
	m_RegionWindow->render();
	m_StatBar->render();
	m_DateBar->render();
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

	for (UIText* uiText : m_UITexts)
	{
		if (uiText->m_OwnedUIElement == id)
		{
			return *uiText;
		}
	}

	ASSERT(false, "UI id not found");
	return *m_UITexts[0];
}
#pragma warning(pop)
