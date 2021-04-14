#pragma once

#include "Game/Data/Types.h"
#include <SFML/Graphics.hpp>
#include "Engine/Vector2D.h"
#include "Game/Data/Titles.h"
#include "Game/Data/UIType.h"
#include "Game/GameDate.h"

struct War;

#pragma warning(push)
#pragma warning(disable: 26812)
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
	void updateSprite(sf::Sprite& sprite, sf::Texture& texture, Vector2DInt position, unsigned int spriteSize = m_SpriteSize);

	bool m_Active = false;

private:
	UIID m_OwnerUIElement;
	sf::Font m_Font;
	int m_SizeX = 64;
	int m_SizeY = 64;
	float m_OutlineThickness = 5.0f;
	int m_CharacterSize = 25;
	sf::Text::Style m_Style = sf::Text::Regular;
	sf::Color m_NegativeColor = sf::Color::Red;
	sf::Color m_PositiveColor = sf::Color::Green;
	const static unsigned int m_SpriteSize = 64;
	unsigned int m_Index = 0;
	sf::Color m_WarIconOutlineColor = sf::Color(210, 32, 60);
	int m_DaySubscriptionHandle = 0;
	Date m_WarStartDate;

	const std::string m_PositiveSign = "+";
	const std::string m_PercentSign = "%";

	War* m_War = nullptr;
	sf::RenderWindow* m_Window = nullptr;

	CharacterID m_AttackerID = INVALID_CHARACTER_ID;
	CharacterID m_DefenderID = INVALID_CHARACTER_ID;

	sf::RectangleShape m_WarIconShape;
	sf::Sprite m_WarIconSprite;
	sf::Texture m_WarIconTexture;
	Vector2DInt m_WarIconPosition = Vector2DInt();
	sf::Text m_WarscoreText;
	Vector2DInt m_WarscoreTextPosition = Vector2DInt();
};
#pragma warning(pop)