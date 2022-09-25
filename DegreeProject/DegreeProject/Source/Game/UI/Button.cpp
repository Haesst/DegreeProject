#include <Game/UI/Button.h>
#include <Engine/Window.h>
#include <Engine/AssetHandler.h>

Button::Button(sf::Font font, sf::RenderWindow* window, Vector2D position, Vector2D size) : Entity(position, size)
{
	m_Font = font;
	m_Window = window;
}

void Button::start()
{

}

void Button::update()
{

}

void Button::render()
{
	if (m_ShapeVisible)
	{
		m_Window->draw(m_Shape);
	}		
	if (m_SpriteVisible)
	{
		m_Window->draw(m_Sprite);
	}
	if (m_TextVisible)
	{
		m_Window->draw(m_Text);
	}
}

void Button::onDayChange()
{

}

void Button::clearInfo()
{

}

void Button::updateInfo()
{

}

void Button::fetchTexture(const char* texturePath, bool useAlternativeTexture)
{
	bool textureLoaded = false;
	while (!textureLoaded)
	{
		if (useAlternativeTexture)
		{
			m_AlternativeTexture = AssetHandler::get().getTextureAtPath(texturePath);
			if (m_AlternativeTexture.getSize().x != 0)
			{
				textureLoaded = true;
			}
		}
		else
		{
			m_Texture = AssetHandler::get().getTextureAtPath(texturePath);
			if (m_Texture.getSize().x != 0)
			{
				textureLoaded = true;
			}
		}
	}
}

void Button::setShape(sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position, bool middleOrigin)
{
	m_Shape.setFillColor(fillColor);
	m_Shape.setOutlineColor(outlineColor);
	m_Shape.setOutlineThickness(outlineThickness);
	m_Shape.setSize(size);
	if (middleOrigin)
	{
		m_Shape.setOrigin(m_Shape.getLocalBounds().width * 0.5f, m_Shape.getLocalBounds().height * 0.5f);
	}
	m_Shape.setPosition(position);
}

void Button::setText(sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string, bool middleOrigin)
{
	m_Text.setFont(font);
	m_Text.setCharacterSize(characterSize);
	m_Text.setFillColor(fillColor);
	m_Text.setString(string);
	if (middleOrigin)
	{
		m_Text.setOrigin(m_Text.getLocalBounds().width * 0.5f, m_Text.getLocalBounds().height * 0.5f);
	}
	m_Text.setPosition(position);
}

void Button::setSprite(const char* texturePath, sf::Vector2f position, float spriteSizeFactor)
{
	fetchTexture(texturePath);
	m_Sprite.setTexture(m_Texture, true);
	m_Sprite.setScale(m_SpriteSize * spriteSizeFactor / m_Sprite.getLocalBounds().width, m_SpriteSize * spriteSizeFactor / m_Sprite.getLocalBounds().height);
	m_Sprite.setPosition(position);
}

void Button::setSprite(sf::Texture& texture, sf::Vector2f position, float spriteSizeFactor)
{
	m_Sprite.setTexture(texture, true);
	m_Sprite.setScale(m_SpriteSize * spriteSizeFactor / m_Sprite.getLocalBounds().width, m_SpriteSize * spriteSizeFactor / m_Sprite.getLocalBounds().height);
	m_Sprite.setPosition(position);
}
