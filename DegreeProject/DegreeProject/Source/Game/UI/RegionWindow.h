#pragma once

#include <SFML/Graphics.hpp>
#include "Game/Data/Types.h"
#include "Game/Data/Titles.h"
#include "Engine/Vector2D.h"

const static unsigned int NUMBER_OF_BUILDING_SLOTS = 4;

struct Character;
struct MapRegion;

class RegionWindow
{
public:

	RegionWindow(UIID id, sf::Font font, Vector2D, Vector2D size);
	
	void start();
	void update();
	void render();
	void clickOnMap();
	void onDayChange();
	void updateInfo();
	void handleWindow();
	void openWindow();
	void closeWindow();
	void clickButton();
	bool checkIfPlayerRegion();
	void displayProgressMeter(unsigned int index);
	void setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position);
	void setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string = "");
	void setSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, unsigned int spriteSize = m_SpriteSize);

	sf::RectangleShape m_WindowShape;
	sf::RectangleShape m_BuildingSlotShapes[NUMBER_OF_BUILDING_SLOTS];
	sf::RectangleShape m_RaiseArmyShape;
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Red;
	float m_SizeX = 600.0f;
	float m_SizeY = 600.0f;
	float m_OutlineThickness = 10.0f;
	sf::Text m_RegionNameText;
	sf::Text m_KingdomNameText;
	sf::Font m_Font;
	int m_CharacterSize = 25;
	bool m_Visible = false;
	bool m_Open = false;
	unsigned int m_CurrentRegionID = 0;
	int m_DaySubscriptionHandle = -1;
	sf::RectangleShape m_BuildingProgressShape[NUMBER_OF_BUILDING_SLOTS];
	sf::Sprite m_BuildingSlotSprites[NUMBER_OF_BUILDING_SLOTS];
	sf::Texture m_BuildingSlotTextures[NUMBER_OF_BUILDING_SLOTS];
	sf::Texture m_RaiseArmyTexture;
	sf::Sprite m_RaiseArmySprite;
	sf::Sprite m_CharacterSprite;
	const float m_PortraitScale = 0.25f;

	sf::Sprite m_RegionManpowerSprite;
	sf::Text m_RegionManpowerText;
	sf::Text m_RegionManpowerInfoText;
	bool m_ShowManpowerInfo = false;

	sf::Texture m_RegionTaxTexture;
	sf::Sprite m_RegionTaxSprite;
	sf::Text m_RegionTaxText;
	sf::Text m_RegionTaxInfoText;
	bool m_ShowTaxInfo = false;

	const static int m_SpriteSize = 64;
	float m_IconSlotPositionX = 0.0f;
	float m_IconSlotPositionOffset = 0.0f;
	float m_IconSlotPositionY = 0.0f;

	sf::RenderWindow* m_Window = nullptr;
	Character* m_PlayerCharacter = nullptr;
	MapRegion* m_CurrentMapRegion = nullptr;
	bool m_PlayerRegion = false;

	const char* m_TaxString = "Tax";
	const char* m_ManPowerString = "Manpower";

	float m_ProgressMeterWidth = 64.0f;
	float m_ProgressMeterBorder = 1.0f;
	float m_ProgressMeterDoubleBorder;
	UIID m_OwnedUIWindow = INVALID_UI_ID;

	sf::Vector2f m_CharacterPosition = sf::Vector2f();
};