#pragma once

#include <Game/Data/Entity.h>
#include <Game/Data/Types.h>
#include <Game/UI/Button.h>

class UIWindow : public Entity
{
public:

	sf::RenderWindow* m_Window = nullptr;
	UIID m_OwnedUIWindow = INVALID_UI_ID;
	float m_OutlineThickness;
	unsigned int m_CharacterSize;
	sf::Font m_Font;
	sf::Color m_BackgroundFillColor = sf::Color(0, 0, 0, 100);
	sf::Color m_WindowFillColor = sf::Color(255, 252, 240);
	sf::Color m_TextFillColor = sf::Color::Black;
	sf::Color m_OutlineColor = sf::Color::Black;
	sf::Color m_TransparentColor = sf::Color::Transparent;
	sf::RectangleShape m_BackgroundShape;
	sf::RectangleShape m_WindowShape;
	sf::Vector2f m_WindowPosition;
	std::vector<sf::Text> m_Texts;
	std::vector<std::string> m_TextStrings;
	unsigned int m_SpriteSize;
	std::vector<const char*> m_ButtonStrings;
	std::vector<Button*> m_Buttons;
	std::vector<sf::Sprite> m_Sprites;
	std::vector<sf::Texture> m_Textures;
	~UIWindow();
	UIWindow(UIID id, sf::Font font, Vector2D position, Vector2D size, unsigned int spriteSize = 32);
	virtual void start() override;
	virtual void update() override;
	virtual void render() override;
	virtual void openWindow();
	virtual void closeWindow();
	virtual void clickButton();
	virtual void handleWindow();
	void setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position, bool middleOrigin = false);
	void setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string = "", bool middleOrigin = false);
	void setSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, float spriteSizeFactor = 1);
};
