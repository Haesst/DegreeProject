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
class PauseWindow;
class FamilyTreeWindow;
class MiniMap;
class EndWindow;

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
	UIID createUIEventElement(CharacterID instigatorID, CharacterID subjectID, UIType type, float giftAmount = 0.0f, std::string buildingName = "", std::string regionName = "");
	UIID createWarIcon(CharacterID attackerID, CharacterID defenderID);
	void AdjustOwnership(CharacterID conquerorID, CharacterID loserID, unsigned int regionID);
	void AdjustOwnerships(CharacterID conquerorID, CharacterID loserID, std::vector<unsigned int>& regionIDs);
	void SetRealmTextAsConquered(CharacterID characterID);
	void SetRealmNameOnText(CharacterID characterID, std::string realmName);
	UIElement& getUIElement(UIID ID);
	UIText& getUIText(UIID ID);
	FamilyTreeWindow* m_FamilyTreeWindow = nullptr;
	CharacterWindow* m_CharacterWindow = nullptr;
	RegionWindow* m_RegionWindow = nullptr;
	PauseWindow* m_PauseWindow = nullptr;
	WarWindow* m_WarWindow = nullptr;
	MainMenu* m_MainMenu = nullptr;
	StatBar* m_StatBar = nullptr;
	DateBar* m_DateBar = nullptr;
	MiniMap* m_MiniMap = nullptr;
	EndWindow* m_EndWindow = nullptr;
private:
	static UIManager* m_Instance;
	static UIID m_UIElementsIDs;
	std::vector<UIElement> m_UIElements;
	std::unordered_map<CharacterID, UIText*> m_UITexts;
	std::map<UIID, EventWindow*> m_EventWindows;
	std::map<UIID, WarIcon*> m_WarIcons;
	std::vector<CharacterID> m_UITextsToRemove;
	std::vector<UIID> m_EventWindowsToRemove;
	std::vector<UIID> m_WarIconsToRemove;
	bool m_MoveWarIcon = false;
};