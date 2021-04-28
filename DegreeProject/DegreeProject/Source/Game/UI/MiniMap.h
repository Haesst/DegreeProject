#pragma once

#include "SFML/Graphics.hpp"

class MiniMap
{
public:
	void start();
	void update();
	void render();

private:
	float m_RightOffset = 0.0f;
	float m_BottomOffset = -5.0f;
	float m_BorderHeightOffset = 96.0f;
	float m_ViewHeightOffset = -161.0f;

	float m_Width = 300.0f;
	float m_BorderSize = 3.0f;

	sf::RectangleShape m_MiniMapBorder;
	sf::Color m_Fillcolor = sf::Color::White;
	sf::Color m_BorderColor = sf::Color::Green;
	sf::View m_MiniMapView;
	sf::View m_ViewHolder;
};