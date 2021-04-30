#pragma once

#include "SFML/Graphics.hpp"
#include "Engine/Vector2D.h"

class MiniMap
{
public:
	void start();
	void update();
	void render();
	void setUIView(sf::View* uiView);
	void setGameView(sf::View* gameView);
	void updateGameViewRectangle();
	void setPlayerColor(sf::Color& color);

	MiniMap();

private:
	const float m_ZoomLevel = 17.5f;
	const Vector2D m_StartPosition = { 8640.0f, 1350.0f };
	const Vector2D m_SpriteSize = { 16000.0f, 3712.0f };
	float m_RightOffset = 0.0f;
	float m_BottomOffset = 60.0f;

	float m_Width = 500.0f;
	float m_Height = 200.0f;
	float m_BorderSize = 5.0f;

	float m_ViewLeftOffset = 0.0f;
	float m_ViewWidthOffset = 55.0f;
	float m_ViewTopOffset = -282.0f;
	float m_ViewHeightOffset = 476.0f;

	sf::RectangleShape m_MiniMapBorder;
	sf::RectangleShape m_GameViewRectangle;
	sf::Color m_Fillcolor = sf::Color::White;
	sf::Color m_BorderColor = sf::Color::Green;
	sf::View m_MiniMapView;
	sf::View* m_UIView;
	sf::View* m_GameView;
	sf::Vector2f m_GameViewCenter = { 0.0f, 0.0f };
};