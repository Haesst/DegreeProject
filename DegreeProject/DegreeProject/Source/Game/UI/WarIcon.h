#pragma once

#include "Game/Data/Types.h"
#include <SFML/Graphics.hpp>
#include "Engine/Vector2D.h"
#include "Game/Data/Titles.h"
#include "Game/Data/UIType.h"
#include "Game/GameDate.h"

struct War;

class WarIcon
{
public:
	WarIcon(UIID ID, sf::Font font, unsigned int index, CharacterID attackerID, CharacterID defenderID);

	void update();
	void render();
	void deactivate();
	void activate();
	void onDayChange();
	void updateInfo();
	void clickButton();
	void updatePosition(unsigned int index);
	void setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position);
	void setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Vector2f position);
	void setSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position);
	void setWarscore(CharacterID& characterID, std::stringstream& stream);
	bool m_Active = false;
	unsigned int m_Index = 0;

private:
	UIID m_OwnerUIElement;
	sf::Font m_Font;
	float m_SizeX = 64.0f;
	float m_SizeY = 64.0f;
	float m_OutlineThickness = 5.0f;
	int m_CharacterSize = 25;
	sf::Color m_NegativeColor = sf::Color::Red;
	sf::Color m_PositiveColor = sf::Color::Green;
	const static unsigned int m_SpriteSize = 64;
	sf::Color m_WarIconOutlineColor = sf::Color(210, 32, 60);
	int m_DaySubscriptionHandle = 0;
	Date m_WarStartDate;

	const std::string m_PositiveSign = "+";
	const std::string m_PercentSign = "%";

	int m_WarHandle = -1;
	sf::RenderWindow* m_Window = nullptr;

	CharacterID m_AttackerID = INVALID_CHARACTER_ID;
	CharacterID m_DefenderID = INVALID_CHARACTER_ID;

	sf::RectangleShape m_WarIconShape;
	sf::Sprite m_WarIconSprite;
	sf::Texture m_WarIconTexture;
	sf::Text m_WarscoreText;
};
