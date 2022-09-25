#include <Game/UI/UIWindow.h>
#include <Engine/Window.h>

UIWindow::~UIWindow()
{
	for (Button* button : m_Buttons)
	{
		delete button;
	}
	m_Buttons.clear();
}

UIWindow::UIWindow(UIID id, sf::Font font, Vector2D position, Vector2D size, unsigned int spriteSize) : Entity(position, size)
{
	m_Visible = false;
	m_Window = Window::getWindow();
	m_OwnedUIWindow = id;
	m_Font = font;
	m_SpriteSize = spriteSize;
}

void UIWindow::start()
{
}

void UIWindow::update()
{
	handleWindow();
}

void UIWindow::render()
{
	if (m_Visible)
	{
		m_Window->draw(m_BackgroundShape);
		m_Window->draw(m_WindowShape);
		for (sf::Sprite& sprite : m_Sprites)
		{
			m_Window->draw(sprite);
		}
		for (sf::Text& text : m_Texts)
		{
			m_Window->draw(text);
		}
		for (Button* button : m_Buttons)
		{
			button->render();
		}
	}
}

void UIWindow::openWindow()
{
	m_WindowShape.setOutlineColor(m_OutlineColor);
	for (sf::Text& text : m_Texts)
	{
		text.setFillColor(m_TextFillColor);
	}
	for (Button* button : m_Buttons)
	{
		button->m_Shape.setOutlineColor(m_OutlineColor);
		button->m_Text.setFillColor(m_TextFillColor);
	}
	m_Visible = true;
}

void UIWindow::closeWindow()
{
	m_Visible = false;
}

void UIWindow::clickButton()
{
}

void UIWindow::handleWindow()
{
}

void UIWindow::setShape(sf::RectangleShape& shape, sf::Color& fillColor, sf::Color& outlineColor, float outlineThickness, sf::Vector2f size, sf::Vector2f position, bool middleOrigin)
{
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThickness);
	shape.setSize(size);
	if (middleOrigin)
	{
		shape.setOrigin(shape.getLocalBounds().width * 0.5f, shape.getLocalBounds().height * 0.5f);
	}
	shape.setPosition(position);
}

void UIWindow::setText(sf::Text& text, sf::Font& font, unsigned int characterSize, sf::Color& fillColor, sf::Vector2f position, const char* string, bool middleOrigin)
{
	text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setFillColor(fillColor);
	text.setString(string);
	if (middleOrigin)
	{
		text.setOrigin(text.getLocalBounds().width * 0.5f, text.getLocalBounds().height * 0.5f);
	}
	text.setPosition(position);
}

void UIWindow::setSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position, float spriteSizeFactor)
{
	sprite.setTexture(texture, true);
	sprite.setScale(m_SpriteSize * spriteSizeFactor / sprite.getLocalBounds().width, m_SpriteSize * spriteSizeFactor / sprite.getLocalBounds().height);
	sprite.setPosition(position);
}
