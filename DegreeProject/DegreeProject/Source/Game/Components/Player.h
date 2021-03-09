#pragma once

#include <SFML/Graphics.hpp>

#include "ECS/Component.h"
#include "ECS/EntityManager.h"

#include "Engine/Vector2D.h"

struct Player : public Component
{
	sf::RectangleShape m_Shape;
	sf::Color m_FillColor = sf::Color::Transparent;
	sf::Color m_OutlineColor = sf::Color::White;
	float m_Size = 32.0f;
	float m_OutlineThickness = 0.0f;
	bool m_Selected = false;

	Vector2D m_Direction = Vector2D(0.0f, 0.0f);
	Vector2D m_Target = Vector2D(1920.0f, 1080.0f);
	float m_Speed = 150.0f;
};