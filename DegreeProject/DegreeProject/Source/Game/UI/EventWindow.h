#pragma once

#include <SFML/Graphics.hpp>
#include "Game/Data/UIType.h"
#include "Game/Data/Types.h"
#include "Game/Data/Titles.h"
#include "Engine/Vector2D.h"

struct Character;

class EventWindow
{
public:

	EventWindow(UIID ID, sf::Font font, CharacterID instigatorID, CharacterID subjectID, UIType type, float giftAmount = 0.0f);

	void update();
	void render();
	void moveWindow();
	void closeWindow();
	void dismissRequest();
	void acceptRequest();
	void clickButton();
	void setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position);
	void setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string = "");
	void setSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, unsigned int spriteSize = m_SpriteSize);
	
	sf::RectangleShape m_WindowShape;

	sf::RectangleShape m_DismissShape;
	sf::Text m_DismissText;
	sf::Color m_DismissColor = sf::Color(210, 32, 60);
	const char* m_DismissString = "Dismiss";

	sf::RectangleShape m_AgreeShape;
	sf::Text m_AgreeText;
	sf::Color m_AgreeColor = sf::Color(67, 175, 17);
	const char* m_AgreeString = "Agree";
	sf::Color m_TransparentColor = sf::Color::Transparent;
	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Black;
	float m_SizeX = 400.0f;
	float m_SizeY = 300.0f;
	float m_PositionX = 0.0f;
	float m_PositionY = 0.0f;
	float m_OutlineThickness = 10.0f;
	sf::Font m_Font;
	int m_CharacterSize = 25;
	const static int m_SpriteSize = 64;
	sf::RenderWindow* m_Window = nullptr;
	UIID m_OwnedUIWindow = INVALID_UI_ID;
	UIType m_MessageType = UIType::WarDeclaration;
	bool m_Dismissed = false;
	bool m_MovingWindow = false;
	Vector2D m_MousePosition = Vector2D();
	std::vector<const char*> m_MaleTitles = { "Emperor ", "King ", "Duke ", "Count ", "Baron ", "Unlanded " };
	std::vector<const char*> m_FemaleTitles = { "Empress ", "Queen ", "Duchess ", "Countess ", "Baroness ", "Unlanded " };

	CharacterID m_InstigatorID = INVALID_CHARACTER_ID;
	sf::RectangleShape m_InstigatorShape;
	sf::Texture m_InstigatorTexture;
	sf::Sprite m_InstigatorSprite;
	sf::Color m_InstigatorColor = sf::Color::Black;

	CharacterID m_SubjectID = INVALID_CHARACTER_ID;
	sf::RectangleShape m_SubjectShape;
	sf::Texture m_SubjectTexture;
	sf::Sprite m_SubjectSprite;
	sf::Color m_SubjectColor = sf::Color::Black;

	sf::Texture m_MessageTypeTexture;
	sf::Sprite m_MessageTypeSprite;
	sf::Text m_MessageText;
};