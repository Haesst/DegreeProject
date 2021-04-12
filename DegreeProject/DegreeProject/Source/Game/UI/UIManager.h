#pragma once

#include <SFML/Graphics.hpp>
#include "Engine/Vector2D.h"
#include "Game/Data/UIElement.h"
#include "Game/Data/UIType.h"
#include "Game/Data/Types.h"
#include <unordered_map>

class CharacterWindow;
class RegionWindow;
class StatBar;
class DateBar;
class UIText;
class EventWindow;

class UIManager
{
public:
	UIManager();
	~UIManager();
	static UIManager* get();
	void start();
	void update();
	void render();
	UIID createUIWindowElement(sf::Font font, UIType type, Vector2D position, Vector2D size);
	UIID createUITextElement(sf::Font font, CharacterID charID, std::string countryName, std::vector<unsigned int> ownedRegions);
	UIID createUIEventElement(CharacterID instigatorID, UIType type);
	void AdjustOwnership(CharacterID conqueror, CharacterID loser, unsigned int regionID);
	UIElement& getUIElement(UIID id);
	UIText& getUIText(UIID id);
	CharacterWindow* m_CharacterWindow = nullptr;
private:
	static UIManager* m_Instance;
	static UIID m_UIElementsIDs;
	std::vector<UIElement> m_UIElements;
	std::unordered_map<CharacterID, UIText*> m_UITexts;
	std::vector<EventWindow*> m_EventWindows;
	RegionWindow* m_RegionWindow = nullptr;
	StatBar* m_StatBar = nullptr;
	DateBar* m_DateBar = nullptr;
};