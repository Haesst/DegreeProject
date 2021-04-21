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
class WarWindow;
class WarIcon;
class MainMenu;

class UIManager
{
public:
	UIManager();
	~UIManager();
	static UIManager& get();
	void start();
	void update();
	void render();
	UIID createUIWindowElement(sf::Font font, UIType type, Vector2D position, Vector2D size);
	UIID createUITextElement(sf::Font font, CharacterID characterID, std::string countryName, std::vector<unsigned int> ownedRegions);
	UIID createUIEventElement(CharacterID instigatorID, CharacterID subjectID, UIType type, float giftAmount = 0.0f);
	UIID createWarIcon(CharacterID attackerID, CharacterID defenderID);
	void AdjustOwnership(CharacterID conquerorID, CharacterID loserID, unsigned int regionID);
	void SetRealmTextAsConquered(CharacterID characterID);
	void SetRealmNameOnText(CharacterID characterID, std::string realmName);
	UIElement& getUIElement(UIID ID);
	UIText& getUIText(UIID ID);
	CharacterWindow* m_CharacterWindow = nullptr;
	RegionWindow* m_RegionWindow = nullptr;
	WarWindow* m_WarWindow = nullptr;
	MainMenu* m_MainMenu = nullptr;
	StatBar* m_StatBar = nullptr;
	DateBar* m_DateBar = nullptr;
	bool m_ActiveWarIcons = false;
	bool m_ActiveEventWindows = false;
private:
	static UIManager* m_Instance;
	static UIID m_UIElementsIDs;
	std::vector<UIElement> m_UIElements;
	std::unordered_map<CharacterID, UIText*> m_UITexts;
	std::vector<EventWindow*> m_EventWindows;
	std::vector<WarIcon*> m_WarIcons;
	std::vector<CharacterID> m_UITextsToRemove;
};