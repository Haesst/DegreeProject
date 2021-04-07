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
	void AdjustOwnership(CharacterID conqueror, CharacterID loser, unsigned int regionID);
	UIElement& getUIElement(UIID id);
	UIText& getUIText(UIID id);
private:
	static UIManager* m_Instance;
	static UIID m_UIElementsIDs;
	std::vector<UIElement> m_UIElements;
	std::unordered_map<CharacterID, UIText*> m_UITexts;
	CharacterWindow* m_CharacterWindow = nullptr;
	RegionWindow* m_RegionWindow = nullptr;
	StatBar* m_StatBar = nullptr;
	DateBar* m_DateBar = nullptr;
};