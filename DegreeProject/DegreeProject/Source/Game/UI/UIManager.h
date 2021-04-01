#pragma once

#include <SFML/Graphics.hpp>
#include "Game/Data/UIWindow.h"
#include "Game/Data/UIType.h"
#include "Engine/Vector2D.h"
#include "Game/UI/CharacterWindow.h"
#include "Game/UI/RegionWindow.h"
#include "Game/UI/StatBar.h"
#include "Game/UI/DateBar.h"

class UIManager
{
public:
	~UIManager();
	static UIManager* get();
	void start();
	void update();
	void render();
	UIWindowID createUIWindow(sf::Font font, Vector2D position, Vector2D size, UIType type);
	UIWindow& getUIWindow(UIWindowID id);
private:
	static UIManager* m_Instance;
	static UIWindowID m_UIWindowIDs;
	std::vector<UIWindow> m_UIWindows;
	CharacterWindow* m_CharacterWindow;
	RegionWindow* m_RegionWindow;
	StatBar* m_StatBar;
	DateBar* m_DateBar;
};