#pragma once

#include <SFML/Graphics.hpp>
#include "Engine/AssetHandler.h"
#include "Engine/Window.h"

class StaticSprite
{
public:
	Vector2D m_Position;

	sf::Sprite m_Sprite;
	sf::Texture m_Texture;

	int m_Width = 0;
	int m_Height = 0;

	bool m_ShouldRender = true;

	StaticSprite(){}

	StaticSprite(const char* path, int width, int height, Vector2D position) : m_Width(width), m_Height(height), m_Position(position)
	{
		m_Texture = AssetHandler::get().loadImageFromFile(path);
		m_Sprite.setTexture(m_Texture);
		m_Sprite.setPosition(m_Position.x, m_Position.y);
		updateSize();
	}

	void start()
	{

	}

	void update()
	{
		m_Sprite.setPosition(m_Position.x, m_Position.y);
		m_Sprite.setTexture(m_Texture);
		updateSize();	
	}

	void render()
	{
		if (m_ShouldRender)
		{
			Window::getWindow()->draw(m_Sprite);
		}
	}
	
	void updateSize()
	{
		sf::FloatRect localSize = m_Sprite.getLocalBounds();

		m_Sprite.setScale(m_Width / localSize.width, m_Height / localSize.height);
	}
};