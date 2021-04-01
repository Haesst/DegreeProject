#include "UIManager.h"

UIManager* UIManager::m_Instance = nullptr;
UIWindowID UIManager::m_UIWindowIDs = 1;

UIManager* UIManager::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new UIManager();
	}
	return m_Instance;
}

UIManager::~UIManager()
{
	delete m_CharacterWindow;
	delete m_RegionWindow;
	delete m_StatBar;
	delete m_DateBar;
}

UIWindowID UIManager::createUIWindow(sf::Font font, Vector2D position, Vector2D size, UIType type)
{
	UIWindowID id = m_UIWindowIDs++;
	UIWindow uiWindow;
	uiWindow.m_UIWindowID = id;
	uiWindow.m_Position = position;
	uiWindow.m_Size = size;
	uiWindow.m_Type = type;
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
	m_UIWindows.push_back(uiWindow);
	return id;
}

void UIManager::start()
{
	ASSERT(m_CharacterWindow != nullptr, "Character Window does not exist");
	ASSERT(m_RegionWindow != nullptr, "Region Window does not exist");
	ASSERT(m_StatBar != nullptr, "Stat Bar does not exist");
	ASSERT(m_DateBar != nullptr, "Date Bar does not exist");
	m_CharacterWindow->start();
	m_RegionWindow->start();
	m_StatBar->start();
	m_DateBar->start();
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
	m_CharacterWindow->render();
	m_RegionWindow->render();
	m_StatBar->render();
	m_DateBar->render();
}

UIWindow& UIManager::getUIWindow(UIWindowID id)
{
	ASSERT(id != INVALID_UI_ID, "Invalid UI id requested");

	for (UIWindow& uiWindow : m_UIWindows)
	{
		if (uiWindow.m_UIWindowID == id)
		{
			return uiWindow;
		}
	}

	ASSERT(false, "UI id not found");
}
