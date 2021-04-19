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
	void updateSprites();
	void updateSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, int spriteSize = m_SpriteSize);

	sf::RectangleShape m_WindowShape;

	sf::RectangleShape m_DismissShape;
	sf::Text m_DismissText;
	sf::Color m_DismissColor = sf::Color(210, 32, 60);
	sf::Vector2f m_DismissPosition = sf::Vector2f();
	const std::string m_DismissString = "Dismiss";

	sf::RectangleShape m_AgreeShape;
	sf::Text m_AgreeText;
	sf::Color m_AgreeColor = sf::Color(67, 175, 17);
	sf::Vector2f m_AgreePosition = sf::Vector2f();
	const std::string m_AgreeString = "Agree";

	sf::Color m_FillColor = sf::Color(255, 252, 240);
	sf::Color m_OwnerColor = sf::Color::Black;
	float m_SizeX = 400.0f;
	float m_SizeY = 300.0f;
	float m_PositionX = 0.0f;
	float m_PositionY = 0.0f;
	float m_OutlineThickness = 10.0f;
	sf::Font m_Font;
	int m_CharacterSize = 50;
	const static int m_SpriteSize = 64;
	sf::RenderWindow* m_Window = nullptr;
	UIID m_OwnedUIWindow = INVALID_UI_ID;
	UIType m_MessageType = UIType::WarDeclaration;
	bool m_Dismissed = false;
	bool m_MovingWindow = false;
	Vector2D m_MousePosition = Vector2D();
	std::string m_MaleTitles[(unsigned int)Title::Baron + 1];
	std::string m_FemaleTitles[(unsigned int)Title::Baron + 1];

	CharacterID m_InstigatorID = INVALID_CHARACTER_ID;
	sf::RectangleShape m_InstigatorShape;
	sf::Texture m_InstigatorTexture;
	sf::Sprite m_InstigatorSprite;
	sf::Vector2f m_InstigatorPosition = sf::Vector2f();
	sf::Color m_InstigatorColor = sf::Color::Black;

	sf::Text m_MessageText;
	sf::Vector2f m_MessageTextPosition = sf::Vector2f();

	CharacterID m_SubjectID = INVALID_CHARACTER_ID;
	sf::RectangleShape m_SubjectShape;
	sf::Texture m_SubjectTexture;
	sf::Sprite m_SubjectSprite;
	sf::Vector2f m_SubjectPosition = sf::Vector2f();
	sf::Color m_SubjectColor = sf::Color::Black;

	sf::Texture m_MessageTypeTexture;
	sf::Sprite m_MessageTypeSprite;
	sf::Vector2f m_MessageTypePosition = sf::Vector2f();
};