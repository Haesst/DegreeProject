#pragma once

#include <SFML/Graphics.hpp>
#include <Engine/Vector2D.h>
#include <Game/Data/Entity.h>

class Button : public Entity
{
public:

	sf::RectangleShape m_Shape;
	sf::Texture m_Texture;
	sf::Texture m_AlternativeTexture;
	sf::Sprite m_Sprite;
	sf::Text m_Text;
	bool m_ShapeVisible = true;
	bool m_TextVisible = true;
	bool m_SpriteVisible = true;
	unsigned int m_SpriteSize = 32;
	Button(sf::Font font, sf::RenderWindow* window, Vector2D position, Vector2D size);
	void start() override;
	void update() override;
	void render() override;
	void fetchTexture(const char* texturePath, bool useAlternativeTexture = false);
	void setShape(sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position, bool middleOrigin = false);
	void setText(sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string = "", bool middleOrigin = false);
	void setSprite(const char* texturePath, sf::Vector2f position, float spriteSizeFactor = 1.0f);
	void setSprite(sf::Texture& texture, sf::Vector2f position, float spriteSizeFactor = 1.0f);
	
private:

	sf::RenderWindow* m_Window = nullptr;
	sf::Font m_Font;
	void onDayChange();
	void clearInfo();
	void updateInfo();
};