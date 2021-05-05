#pragma once

#include <SFML/Graphics.hpp>
#include "Game/Data/Types.h"
#include "Game/Data/Titles.h"
#include "Engine/Vector2D.h"

struct Character;
enum class Gender;

class FamilyTreeWindow
{
public:

	FamilyTreeWindow(UIID id, sf::Font font, Vector2D position, Vector2D size);
	void start();
	void update();
	void render();
	void onDayChange();
	void clearInfo();
	void updateInfo();
	void handleWindow();
	void openWindow();
	void closeWindow();
	void clickButton();
	void setFamilyMember(CharacterID& characterID, unsigned int depth, float width);
	void displayFamily(CharacterID& characterID, unsigned int depth, float width);
	CharacterID& findFamilyHead(CharacterID& characterID);
	void setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position);
	void setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string = "");
	void setSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, unsigned int spriteSize = m_SpriteSize);

	float m_SizeX = 960.0f;
	float m_SizeY = 1060.0f;
	float m_PositionX = 480.0f;
	float m_PositionY = 10.0f;
	float m_OutlineThickness = 10.0f;
	bool m_Visible = false;
	int m_CharacterSize = 25;
	int m_DaySubscriptionHandle = -1;
	bool m_Open = false;
	const static int m_SpriteSize = 32;
	UIID m_OwnedUIWindow = INVALID_UI_ID;
	sf::Font m_Font;
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Red;
	sf::Color m_TransparentColor = sf::Color::Transparent;
	sf::RectangleShape m_WindowShape;

	sf::RenderWindow* m_Window = nullptr;

	Character* m_CurrentCharacter = nullptr;
	CharacterID m_CurrentCharacterID = INVALID_CHARACTER_ID;
	Gender m_Gender;
	bool m_Dead = false;
	unsigned int m_FamilySize = 0;

	std::vector<const char*> m_CharacterTitles = { "Emperor ", "King ", "Duke ", "Count ", "Baron ", "Unlanded ", "Empress ", "Queen ", "Duchess ", "Countess ", "Baroness ", "Unlanded " };
	std::vector<sf::RectangleShape> m_CharacterShapes = std::vector<sf::RectangleShape>();
	std::vector<sf::Sprite> m_CharacterSprites = std::vector<sf::Sprite>();
	std::vector<CharacterID> m_CharacterIDs = std::vector<CharacterID>();
	std::vector<sf::Text> m_CharacterInfo = std::vector<sf::Text>();
	std::vector<bool> m_ShowCharacterInfo = std::vector<bool>();
	std::vector<sf::Sprite> m_CharacterDeadSprites = std::vector<sf::Sprite>();

	const float m_PortraitScale = 0.125f;
	sf::Texture m_DeadTexture;

	sf::Text m_WindowTitle;
};